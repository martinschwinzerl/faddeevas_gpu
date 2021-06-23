#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#include <CL/cl2.hpp>
#include "toml/toml.hpp"

#include "common/definitions.h"
#include "common/benchmark_item.h"

int main( int const argc, char* argv[] )
{
    typedef CERRF_REAL_TYPE real_type;

    std::string path_to_config_file( "./config.toml" );

    if( argc >= 2 )
    {
        path_to_config_file = argv[ 1 ];
    }

    auto config = toml::parse_file( path_to_config_file );

    faddeevas::BenchmarkRun run_config( config );

    std::ostringstream a2str;
    ::time_t now = ::time( 0 );
    ::tm* gmtm = ::gmtime( &now );
    assert( gmtm != nullptr );

    a2str << std::setfill( '0' )
          << std::setw( 4 ) << ( gmtm->tm_year + 1900 )
          << std::setw( 2 ) << ( gmtm->tm_mon + 1 )
          << std::setw( 2 ) << gmtm->tm_mday
          << "_"
          << std::setw( 2 ) << gmtm->tm_hour
          << std::setw( 2 ) << gmtm->tm_min
          << std::setw( 2 ) << gmtm->tm_sec
          << std::setfill( ' ' );

    std::string const datetime_str = a2str.str();
    a2str.str( "" );

    a2str << run_config.path_to_output;
    a2str << run_config.name
          << "_opencl_pid" << run_config.platform_id
          << "_did" << run_config.device_id
          << "_" << datetime_str;

    std::string path_to_output_log = a2str.str();
    path_to_output_log += ".log";

    std::string path_to_output_result = a2str.str();
    path_to_output_result += ".times";

    std::ofstream log_file( path_to_output_log );
    log_file << "# " << datetime_str << " :: start from config file "
             << path_to_config_file << "\r\n"
             << "# name: " << run_config.name << "\r\n"
             << "# path_to_kernel: " << run_config.path_to_kernel << "\r\n"
             << "# kernel_file_name: " << run_config.kernel_file_name << "\r\n"
             << "# kernel_fn_name: " << run_config.kernel_file_name << "\r\n"
             << "# compile_options_pre: " << run_config.compile_options_pre << "\r\n"
             << "# compile_options_post: " << run_config.compile_options_post << "\r\n"
             << "#\r\n";

    if( run_config.default_init_x_arg.get() != nullptr )
    {
        log_file << "# default_init_x_arg: "
                 << run_config.default_init_x_arg->m_config_name << "\r\n"
                 << "#                   : "
                 << run_config.default_init_x_arg->m_config_str << "\r\n"
                 << "#\r\n";
    }

    if( run_config.default_init_y_arg.get() != nullptr )
    {
        log_file << "# default_init_y_arg: "
                 << run_config.default_init_y_arg->m_config_name << "\r\n"
                 << "#                   : "
                 << run_config.default_init_y_arg->m_config_str << "\r\n"
                 << "#\r\n";
    }

    log_file << "# number of configurations: " << run_config.items.size() << "\r\n";


    std::ofstream result_file( path_to_output_result );
    a2str.str( "" );

    a2str << "#   num. evaluations"
          << "    num. repetitions"
          << "     workgroup  size"
          << "    median time [us]"
          << "       min time [us]"
          << "  10-perc. time [us]"
          << "  90-perc. time [us]"
          << "       max time [us]"
          << "\r\n" << "#\r\n";

    std::cout << a2str.str();
    result_file << a2str.str();
    a2str.str( "" );

    /* ********************************************************************  */
    /* Init device, command queue, and context */

    std::vector< cl::Platform > platforms;
    cl::Platform::get( &platforms );

    std::vector< cl::Device > devices;
    platforms.at( run_config.platform_id ).getDevices(
        CL_DEVICE_TYPE_ALL, &devices );

    cl::Device device = devices.at( run_config.device_id );
    cl::Context ctx( device );
    cl::CommandQueue queue( ctx, device, CL_QUEUE_PROFILING_ENABLE );

    log_file << "# platform : "
             << platforms.at( run_config.platform_id ).getInfo< CL_PLATFORM_NAME >() << "\r\n"
             << "# device   : " << device.getInfo< CL_DEVICE_NAME >() << "\r\n"
             << "#          : " << device.getInfo< CL_DEVICE_VENDOR >() << "\r\n"
             << "#          : " << device.getInfo< CL_DEVICE_VENDOR_ID >() << "\r\n";

    /* ---------------------------------------------------------------------- */
    /* prepare program and kernel */

    a2str << run_config.path_to_kernel;
    if( run_config.path_to_kernel.back() != '/' ) a2str << "/";
    a2str << run_config.kernel_file_name;
    std::string const path_to_kernel_file = a2str.str();
    a2str.str( "" );

    std::string kernel_src;
    std::ifstream in_kernel( path_to_kernel_file );

    if( in_kernel.is_open() )
    {
        a2str << in_kernel.rdbuf();
        kernel_src = a2str.str();
        a2str.str( "" );
    }

    if( !run_config.compile_options_pre.empty() )
    {
        a2str << run_config.compile_options_pre << " ";
    }

    a2str << "-D_GPUCODE=1 -DCERRF_NO_SYSTEM_INCLUDES=1";

    if( !run_config.compile_options_post.empty() )
    {
        a2str  << " " << run_config.compile_options_post;
    }

    if( !run_config.include_dirs.empty() )
    {
        for( auto const& idir : run_config.include_dirs )
        {
            a2str << " -I" << idir;
        }
    }

    std::string const kernel_compile_options = a2str.str();
    a2str.str( "" );
    log_file << "# compile options string: " << kernel_compile_options << "\r\n";

    cl::Program program( ctx, kernel_src );
    ::cl_int ret = CL_SUCCESS;

    try
    {
        ret = program.build( kernel_compile_options.c_str() );
    }
    catch( cl::BuildError const& e )
    {
        std::cout << "error     : " << e.what() << "\r\n";
        std::cout << "build log : "
                  << program.getBuildInfo< CL_PROGRAM_BUILD_LOG >( device )
                  << "\r\n" << std::endl;

        throw;
    }

    assert( ret == CL_SUCCESS );
    cl::Kernel kernel( program, run_config.kernel_fn_name.c_str() );

    int const kernel_wg_size = kernel.getWorkGroupInfo<
        CL_KERNEL_WORK_GROUP_SIZE >( device );

    int const kernel_wg_multiple = kernel.getWorkGroupInfo<
        CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE >( device );

    ::cl_ulong const kernel_private_memory = kernel.getWorkGroupInfo<
        CL_KERNEL_PRIVATE_MEM_SIZE >( device );

    ::cl_ulong const kernel_local_memory = kernel.getWorkGroupInfo<
        CL_KERNEL_LOCAL_MEM_SIZE >( device );

    log_file << "# max. kernel work-group size     : " << kernel_wg_size << "\r\n"
             << "# kernel work-group size multiple : " << kernel_wg_multiple << "\r\n"
             << "# kernel private memory [Bytes]   : " << kernel_private_memory << " \r\n"
             << "# kernel local memory [Bytes]     : " << kernel_local_memory << "\r\n"
             << "# \r\n";


    for( auto const& item : run_config.items )
    {
        now  = ::time( 0 );
        gmtm = ::gmtime( &now );
        assert( gmtm != nullptr );

        int wg_size = item.workgroup_size;

        if( wg_size == 0 ) wg_size = kernel_wg_size;
        if( wg_size == 0 ) wg_size = kernel_wg_multiple;

        int num_wg = item.num_evaluations / wg_size;
        int num_wi = num_wg * wg_size;

        if( num_wi < item.num_evaluations )
        {
            num_wi += wg_size;
            ++num_wg;
        }

        assert( num_wi >= item.num_evaluations );

        a2str << std::setfill( '0' )
              << std::setw( 4 ) << ( gmtm->tm_year + 1900 )
              << std::setw( 2 ) << ( gmtm->tm_mon + 1 )
              << std::setw( 2 ) << gmtm->tm_mday
              << "_"
              << std::setw( 2 ) << gmtm->tm_hour
              << std::setw( 2 ) << gmtm->tm_min
              << std::setw( 2 ) << gmtm->tm_sec
              << std::setfill( ' ' );

        log_file << "# " << a2str.str() << " :: "
            << "num_evaluations = " << item.num_evaluations << ", "
            << "num_repetitions = " << item.num_repetitions << ", "
            << "workgroup_size  = " << wg_size << "\r\n"
            << "# " << a2str.str() << " :: num_wi = " << num_wi
            << ", num_wg = " << num_wg << "\r\n";

        std::vector< real_type > in_x( item.num_evaluations, real_type{ 0. } );
        std::vector< real_type > in_y( item.num_evaluations, real_type{ 0. } );

        if( item.init_x_arg.get() != nullptr )
        {
            item.init_x_arg->init( in_x.data(), in_x.data() + in_x.size() );
        }
        else if( run_config.default_init_x_arg.get() != nullptr )
        {
            run_config.default_init_x_arg->init(
                in_x.data(), in_x.data() + in_x.size() );
        }

        if( item.init_y_arg.get() != nullptr )
        {
            item.init_y_arg->init( in_y.data(), in_y.data() + in_y.size() );
        }
        else if( run_config.default_init_y_arg.get() != nullptr )
        {
            run_config.default_init_y_arg->init(
                in_y.data(), in_y.data() + in_x.size() );
        }

        std::vector< real_type > out_x( item.num_evaluations, real_type{ 0. } );
        std::vector< real_type > out_y( item.num_evaluations, real_type{ 0. } );

        auto const buf_size = sizeof( real_type ) * item.num_evaluations;

        /* ----------------------------------------------------------------- */

        cl::Buffer in_x_arg( ctx, CL_MEM_READ_WRITE, buf_size, nullptr );
        cl::Buffer in_y_arg( ctx, CL_MEM_READ_WRITE, buf_size, nullptr );

        cl::Buffer out_x_arg( ctx, CL_MEM_READ_WRITE, buf_size, nullptr );
        cl::Buffer out_y_arg( ctx, CL_MEM_READ_WRITE, buf_size, nullptr );

        ret = queue.enqueueWriteBuffer( in_x_arg, CL_TRUE, std::size_t{ 0 },
            in_x.size() * sizeof( real_type ), in_x.data() );

        assert( ret == CL_SUCCESS );

        ret = queue.enqueueWriteBuffer( in_y_arg, CL_TRUE, std::size_t{ 0 },
            in_y.size() * sizeof( real_type ), in_y.data() );

        assert( ret == CL_SUCCESS );

        ::cl_long const nn = item.num_evaluations;

        kernel.setArg( 0, in_x_arg );
        kernel.setArg( 1, in_y_arg );
        kernel.setArg( 2, out_x_arg );
        kernel.setArg( 3, out_y_arg );
        kernel.setArg( 4, nn );

        std::vector< double > times( item.num_repetitions, double{ 0.0 } );

        log_file << "#      run"
                 << "     num evaluations"
                 << "           time [us]"
                 << "  num rep."
                 << "      workgroup size"
                 << "\r\n";

        for( int rr = 0 ; rr < item.num_repetitions ; ++rr )
        {
            cl::Event ev;

            ret = queue.enqueueNDRangeKernel(
                kernel, cl::NullRange, num_wi, wg_size, nullptr, &ev );
            assert( ret == CL_SUCCESS );

            queue.flush();

            ::cl_ulong when_started = ::cl_ulong{ 0 };
            ::cl_ulong when_stopped = ::cl_ulong{ 0 };

            ev.wait();

            ret = ev.getProfilingInfo( CL_PROFILING_COMMAND_START, &when_started );
            assert( ret == CL_SUCCESS );

            ret = ev.getProfilingInfo( CL_PROFILING_COMMAND_END, &when_stopped );
            assert( ret == CL_SUCCESS );

            times[ rr ]  = static_cast< double >( when_stopped - when_started );
            times[ rr ] *= double{ 1e-3 }; // convert to microseconds

            log_file << std::setw( 10 ) << rr
                     << std::setw( 20 ) << item.num_evaluations
                     << std::setw( 20 ) << times[ rr ]
                     << std::setw( 10 ) << item.num_repetitions
                     << std::setw( 20 ) << wg_size << "\r\n";
        }

        ret = queue.enqueueReadBuffer( out_x_arg, CL_TRUE, std::size_t{ 0 },
            out_x.size() * sizeof( real_type ), out_x.data() );

        assert( ret == CL_SUCCESS );

        ret = queue.enqueueReadBuffer( out_y_arg, CL_TRUE, std::size_t{ 0 },
            out_y.size() * sizeof( real_type ), out_y.data() );

        std::sort( times.begin(), times.end() );

        a2str.str( "" );
        a2str << std::setw( 20 ) << item.num_evaluations
              << std::setw( 20 ) << item.num_repetitions
              << std::setw( 20 ) << wg_size
              << std::setw( 20 ) << times[ ( times.size() + 1u ) / 2u ]
              << std::setw( 20 ) << times.front()
              << std::setw( 20 ) << times.at( ( int )std::floor( times.size() * 0.1 + 0.5 ) )
              << std::setw( 20 ) << times.at( ( int )std::floor( times.size() * 0.9 + 0.5 ) )
              << std::setw( 20 ) << times.back()
              << "\r\n";

        result_file << a2str.str();
        std::cout   << a2str.str();
    }

    return 0;
}
