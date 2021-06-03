#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <CL/cl2.hpp>

#include "run_cerrf.h"

int main( int const argc, char* argv[] )
{
    double const XLIM = double{ 5.33 };
    double const YLIM = double{ 4.29 };

    int platform_id = 1;
    int device_id = 0;

    int NUM_EVALUATIONS = 50 * 1024;
    int NUM_REPETITIONS = 10;

    std::ostringstream a2str;
    std::string include_dir = "../cernlib_base/";
    std::string path_kernel_file = "../cernlib_base/cerrf.cl";

    /* ********************************************************************* */

    if( argc >= 2 ) { platform_id = std::atoi( argv[ 1 ] );

        if( argc >= 3 ) { device_id = std::atoi( argv[ 2 ] );

            if( argc >= 4 ) { NUM_EVALUATIONS = std::atoi( argv[ 3 ] );

                if( argc >= 5 ) { NUM_REPETITIONS = std::atoi( argv[ 4 ] );

                    if( argc >= 6 )
                    {
                        a2str << "../cernlib_" << argv[ 5 ] << "/";
                        include_dir = a2str.str();

                        a2str << "cerrf.cl";
                        path_kernel_file = a2str.str();
                        a2str.str( "" );
                    }
                }
            }
        }
    }
    else
    {
        std::cout << "Usage: " << argv[ 0 ]
                  << " [PLATFORM_ID [DEVICE_ID [NUM_EVALUATIONS"
                  << " [NUM_REPETITIONS [KERNEL_BASE_DIR]]]]]\r\n"
                  << std::endl;

        return 0;
    }

    /* ********************************************************************* */
    /* init arguments for evaluating the cerrf function randomly */

    std::mt19937_64 prng( 20210527 );

    std::uniform_real_distribution< double > x_uniform_dist(
        -std::sqrt( double{ 2.0 } ) * XLIM, std::sqrt( double{ 2.0 } ) * XLIM );

    std::uniform_real_distribution< double > y_uniform_dist(
        -std::sqrt( double{ 2.0 } ) * YLIM, std::sqrt( double{ 2.0 } ) * YLIM );

    std::vector< double > x_uniform( NUM_EVALUATIONS, double{ 0.0 } );
    std::vector< double > y_uniform( NUM_EVALUATIONS, double{ 0.0 } );

    for( int ii = 0 ; ii < NUM_EVALUATIONS ; ++ii )
    {
        x_uniform[ ii ] = x_uniform_dist( prng );
        y_uniform[ ii ] = y_uniform_dist( prng );
    }

    std::vector< double > x_result( NUM_EVALUATIONS, double{ 0.0 } );
    std::vector< double > y_result( NUM_EVALUATIONS, double{ 0.0 } );

    /* --------------------------------------------------------------------- */
    /* Init device, command queue, and context */

    std::vector< cl::Platform > platforms;
    cl::Platform::get( &platforms );

    assert( !platforms.empty() );
    assert( platforms.size() > platform_id );

    std::vector< cl::Device > devices;
    platforms[ platform_id ].getDevices( CL_DEVICE_TYPE_ALL, &devices );

    assert( !devices.empty() );
    assert( devices.size() > device_id );
    cl::Device device = devices[ device_id ];

    cl::Context ctx( device );
    cl::CommandQueue queue( ctx, device, CL_QUEUE_PROFILING_ENABLE );

    /* ---------------------------------------------------------------------- */
    /* prepare program and kernel */

    std::string kernel_src;
    std::ifstream in_kernel( path_kernel_file );

    if( in_kernel.is_open() )
    {
        a2str << in_kernel.rdbuf();
        kernel_src = a2str.str();
        a2str.str( "" );
    }

    a2str << "-D_GPUCODE=1 -DCERRF_NO_SYSTEM_INCLUDES=1";
    if( std::strlen( CERRF_TOSTRING( CERRF_COMPILE_OPTIONS ) ) > 0u )
    {
        a2str << " " << CERRF_TOSTRING( CERRF_COMPILE_OPTIONS );
    }

    a2str << " -I " << include_dir;

    std::string const kernel_compile_options = a2str.str();
    a2str.str( "" );

    cl::Program program( ctx, kernel_src );
    ::cl_int ret = program.build( kernel_compile_options.c_str() );

    assert( ret == CL_SUCCESS );
    cl::Kernel kernel( program, "eval_cerrf" );

    /* --------------------------------------------------------------------- */
    /* prepare buffers */

    auto const buf_size = sizeof( double ) * NUM_EVALUATIONS;

    cl::Buffer in_x_arg( ctx, CL_MEM_READ_WRITE, buf_size, nullptr );
    cl::Buffer in_y_arg( ctx, CL_MEM_READ_WRITE, buf_size, nullptr );

    cl::Buffer out_x_arg( ctx, CL_MEM_READ_WRITE, buf_size, nullptr );
    cl::Buffer out_y_arg( ctx, CL_MEM_READ_WRITE, buf_size, nullptr );

    ret = queue.enqueueWriteBuffer( in_x_arg, CL_TRUE, std::size_t{ 0 },
        x_uniform.size() * sizeof( double ), x_uniform.data() );

    assert( ret == CL_SUCCESS );

    ret = queue.enqueueWriteBuffer( in_y_arg, CL_TRUE, std::size_t{ 0 },
        y_uniform.size() * sizeof( double ), y_uniform.data() );

    assert( ret == CL_SUCCESS );

    /* --------------------------------------------------------------------- */
    /* assign arguments to kernel */

    ::cl_long const nn = NUM_EVALUATIONS;

    kernel.setArg( 0, in_x_arg );
    kernel.setArg( 1, in_y_arg );
    kernel.setArg( 2, out_x_arg );
    kernel.setArg( 3, out_y_arg );
    kernel.setArg( 4, nn );

    /* --------------------------------------------------------------------- */
    /* repeat the execution of the kernel NUM_REPETITIONS times */

    std::vector< double > times( NUM_REPETITIONS, double{ 0.0 } );

    for( int rr = 0 ; rr < NUM_REPETITIONS ; ++rr )
    {
        cl::Event ev;

        ret = queue.enqueueNDRangeKernel(
            kernel, cl::NullRange, NUM_EVALUATIONS,
                cl::NullRange, nullptr, &ev );
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
    }

    ret = queue.enqueueReadBuffer( out_x_arg, CL_TRUE, std::size_t{ 0 },
        x_result.size() * sizeof( double ), x_result.data() );

    assert( ret == CL_SUCCESS );

    ret = queue.enqueueReadBuffer( out_y_arg, CL_TRUE, std::size_t{ 0 },
        y_result.size() * sizeof( double ), y_result.data() );

    /* --------------------------------------------------------------------- */

    assert( ret == CL_SUCCESS );
    std::sort( times.begin(), times.end() );

    std::cout << "fastest run: " << times[ 0 ] << " microseconds\r\n"
              << "slowest run: " << times[ NUM_REPETITIONS - 1 ]
              << " microseconds\r\n"
              << "median     : " << times[ NUM_REPETITIONS / 2 ]
              << " microseconds\r\n"
              << std::endl;

    return 0;
}
