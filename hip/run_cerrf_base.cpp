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

#include <hip/hip_runtime.h>

#ifndef __HIP_PLATFORM_NVCC__
    #include "hip/hip_ext.h"
#endif

#include "hip/cerrf_base_impl.h"

bool event_timing = false;

/********************* UTILS ****************************************** */
#define check_error(status)                                              \
do {                                                                     \
    hipError_t err = status;                                             \
    if( err != hipSuccess ) {                                            \
        std::cerr << "Error: " << hipGetErrorString( err ) << std::endl; \
        exit( err );                                                     \
    }                                                                    \
} while( 0 )

#define GPUInfo( string, ... )              \
    { printf( string "\n", ##__VA_ARGS__ ); }

#define GPUFailedMsg( x ) GPUFailedMsgA( x, __FILE__, __LINE__ )
#define GPUFailedMsgI( x ) GPUFailedMsgAI( x, __FILE__, __LINE__ )
#define GPUError( ... ) GPUInfo( __VA_ARGS__  )

static int GPUFailedMsgAI(
    const long long int error, const char* file, int line)
{
  // Check for HIP Error and in the case of an error display the corresponding error string
  if( error == hipSuccess ) { return (0); }
  GPUError( "HIP Error: %lld / %s (%s:%d)", error,
            hipGetErrorString( ( hipError_t )error ), file, line );
  return 1;
}

static void GPUFailedMsgA(
    const long long int error, const char* file, int line )
{   if (GPUFailedMsgAI(error, file, line)) {
        throw std::runtime_error("HIP Failure"); }
}

/* ************************************************************************* */

__global__ void cerrf_eval(
    double const* __restrict__ in_x,
    double const* __restrict__ in_y,
    double* __restrict__ out_x,
    double* __restrict__ out_y, long int const nn )
{
    long int const idx = threadIdx.x + blockIdx.x * blockDim.x;

    if( idx < nn )
    {
        double const in_real = in_x[ idx ];
        double const in_imag = in_y[ idx ];
        double const cfactor = ( double )1.12837916709551;
        double const xLim = 5.33; double const yLim = 4.29;
        double h, q, Saux, Sx, Sy, Tn, Tx, Ty, Wx, Wy, xh, xl, x, yh, y;
        double Rx [33]; double Ry [33];
        int n, nc, nu;

        x = fabs(in_real); y = fabs(in_imag);

        if (y < yLim && x < xLim){
            q = (1.0 - y / yLim) * sqrt(1.0 - (x / xLim) * (x / xLim));
            h  = 1.0 / (3.2 * q);
            nc = 7 + (int) (23.0 * q);
            xl = pow(h, (double) (1 - nc));
            xh = y + 0.5 / h;
            yh = x;
            nu = 10 + (int) (21.0 * q);
            Rx[nu] = 0.;
            Ry[nu] = 0.;
            for (n = nu; n > 0; n--){
                Tx = xh + n * Rx[n];
                Ty = yh - n * Ry[n];
                Tn = Tx*Tx + Ty*Ty;
                Rx[n-1] = 0.5 * Tx / Tn;
                Ry[n-1] = 0.5 * Ty / Tn;
                }
            Sx = 0.;
            Sy = 0.;
            for (n = nc; n>0; n--){
                Saux = Sx + xl;
                Sx = Rx[n-1] * Saux - Ry[n-1] * Sy;
                Sy = Rx[n-1] * Sy + Ry[n-1] * Saux;
                xl = h * xl;
            };
            Wx = cfactor * Sx;
            Wy = cfactor * Sy;
        }
        else{
            xh = y;
            yh = x;
            Rx[0] = 0.;
            Ry[0] = 0.;
            for (n = 9; n>0; n--){
                Tx = xh + n * Rx[0];
                Ty = yh - n * Ry[0];
                Tn = Tx * Tx + Ty * Ty;
                Rx[0] = 0.5 * Tx / Tn;
                Ry[0] = 0.5 * Ty / Tn;
            };
            Wx = cfactor * Rx[0];
            Wy = cfactor * Ry[0];
        }
        if (y == 0.) {Wx = exp(-x * x);}
        if (in_imag < 0.){
            Wx =   2.0 * exp(y * y - x * x) * cos(2.0 * x * y) - Wx;
            Wy = - 2.0 * exp(y * y - x * x) * sin(2.0 * x * y) - Wy;
            if (in_real > 0.) {Wy = -Wy;}
        }
        else if (in_real < 0.) {Wy = -Wy;}

        out_x[ idx ] = Wx;
        out_y[ idx ] = Wy;
    }
}

void parseArguments( int argc, char *argv[] )
{
    for( int i = 1; i < argc; i++ )
    {
        if( !std::string( "--event-timing" ).compare( argv[ i ] ) ||
            !std::string( "-e" ).compare( argv[ i ] ) )
        {
            event_timing = true;
        }
    }
}


int main( int argc, char* argv[] )
{
    parseArguments( argc, argv );

    double const XLIM = double{ 5.33 };
    double const YLIM = double{ 4.29 };

    int platform_id = 1;
    int device_id = 0;

    long int NUM_EVALUATIONS = 50 * 1024;
    long int NUM_REPETITIONS = 10;

    if( argc >= 2 ) { NUM_EVALUATIONS = std::atol( argv[ 1 ] );
        if( argc >= 3 ) { NUM_REPETITIONS = std::atol( argv[ 2 ] ); }
    }
    else
    {
        std::cout << "Usage: " << argv[ 0 ]
                  << " [NUM_EVALUATIONS [NUM_REPETITIONS]]\r\n\r\n";

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


    /* ********************************************************************* */
    /* Debug functions */
    hipDeviceProp_t hipDeviceProp;
    int mDeviceId = 0; //TODO fix-me
    int debugLevel = 1;

    GPUFailedMsgI( hipGetDeviceProperties( &hipDeviceProp, mDeviceId ) );

    if( debugLevel >= 1 )
    {
        GPUInfo( "Using HIP Device %s with Properties:", hipDeviceProp.name );
        GPUInfo( "\ttotalGlobalMem = %lld", (unsigned long long int)hipDeviceProp.totalGlobalMem );
        GPUInfo( "\tsharedMemPerBlock = %lld", (unsigned long long int)hipDeviceProp.sharedMemPerBlock );
        GPUInfo( "\tregsPerBlock = %d", hipDeviceProp.regsPerBlock );
        GPUInfo( "\twarpSize = %d", hipDeviceProp.warpSize );
        GPUInfo( "\tmaxThreadsPerBlock = %d", hipDeviceProp.maxThreadsPerBlock );
        GPUInfo( "\tmaxThreadsDim = %d %d %d", hipDeviceProp.maxThreadsDim[0], hipDeviceProp.maxThreadsDim[1], hipDeviceProp.maxThreadsDim[2] );
        GPUInfo( "\tmaxGridSize = %d %d %d", hipDeviceProp.maxGridSize[0], hipDeviceProp.maxGridSize[1], hipDeviceProp.maxGridSize[2] );
        GPUInfo( "\ttotalConstMem = %lld", (unsigned long long int)hipDeviceProp.totalConstMem );
        GPUInfo( "\tmajor = %d", hipDeviceProp.major );
        GPUInfo( "\tminor = %d", hipDeviceProp.minor );
        GPUInfo( "\tclockRate = %d", hipDeviceProp.clockRate );
        GPUInfo( "\tmemoryClockRate = %d", hipDeviceProp.memoryClockRate );
        GPUInfo( "\tmultiProcessorCount = %d", hipDeviceProp.multiProcessorCount );
        GPUInfo( "\tPCIBusID = %d", hipDeviceProp.pciBusID );
        GPUInfo( " ");
    }

    /* ********************************************************************** */
    /* Allocate buffers on the device */

    double* in_x_arg = nullptr;
    double* in_y_arg = nullptr;

    double* out_x_arg = nullptr;
    double* out_y_arg = nullptr;

    auto const buffer_size = sizeof( double ) * NUM_EVALUATIONS;

    auto status = ::hipMalloc( ( void** )&in_x_arg, buffer_size );
    assert( status == hipSuccess );

    status = ::hipMalloc( ( void** )&in_y_arg, buffer_size );
    assert( status == hipSuccess );

    status = ::hipMalloc( ( void** )&out_x_arg, buffer_size );
    assert( status == hipSuccess );

    status = ::hipMalloc( ( void** )&out_y_arg, buffer_size );
    assert( status == hipSuccess );

    /* Copy x_uniform and y_uniform to in_x_arg and in_y_arg */

    status = ::hipMemcpy( in_x_arg, x_uniform.data(), buffer_size, ::hipMemcpyHostToDevice );
    assert( status == hipSuccess );

    status = ::hipMemcpy( in_y_arg, y_uniform.data(), buffer_size, ::hipMemcpyHostToDevice );
    assert( status == hipSuccess );

    /* ******************************************************************** */
    /* Estimate block size */

    int BLOCK_SIZE = 0;
    int MIN_GRID_SIZE = 0;
    int MAX_GRID_SIZE = 0;

    GPUFailedMsg( hipOccupancyMaxPotentialBlockSize(
        &MIN_GRID_SIZE, &BLOCK_SIZE, cerrf_eval, 0, 0 ) );

    GPUFailedMsg( hipOccupancyMaxActiveBlocksPerMultiprocessor(
        &MAX_GRID_SIZE, cerrf_eval, BLOCK_SIZE, 0 ) );

    assert( BLOCK_SIZE > 0 );
    int const GRID_SIZE = ( NUM_EVALUATIONS + BLOCK_SIZE - 1 ) / BLOCK_SIZE;

    GPUInfo( "Kernel: %50s Block size: %4d, Maximum active blocks: %3d, "
             "Suggested blocks: %3d",
             "cerrf_eval", BLOCK_SIZE, MAX_GRID_SIZE, MIN_GRID_SIZE );

    /* ******************************************************************** */
    /* Run kernel: */

    ::hipDeviceProp_t props;
    int device = 0;

    status = ::hipGetDevice( &device );
    assert( status == hipSuccess );

    status = ::hipGetDeviceProperties( &props, device );
    assert( status == hipSuccess );

    char pci_bus_id_str[] =
    {
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
        '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
    };

    status = ::hipDeviceGetPCIBusId( pci_bus_id_str, 32, device );
    assert( status == hipSuccess );

    /* Prepare cuda events to estimate the elapsed wall time */

    ::hipEvent_t start;
    status = ::hipEventCreate( &start );
    assert( status == hipSuccess );

    ::hipEvent_t stop;
    status = ::hipEventCreate( &stop );
    assert( status == hipSuccess );

    std::vector< float > times( NUM_EVALUATIONS, float{ 0. } );

    for( long int rr = 0 ; rr < NUM_REPETITIONS ; ++rr )
    {
        /* Run kernel */
        if( !event_timing )
        {
            status = ::hipEventRecord( start );
            assert( status == hipSuccess );

            hipLaunchKernelGGL( cerrf_eval, dim3( GRID_SIZE ),
                dim3( BLOCK_SIZE ), 0, 0, in_x_arg, in_y_arg, out_x_arg,
                    out_y_arg, NUM_EVALUATIONS );

            status = ::hipDeviceSynchronize();

            /* Estimate wall time */
            status = ::hipEventRecord( stop );
            assert( status == hipSuccess );

            status = ::hipEventSynchronize( stop );
            assert( status == hipSuccess );
        }
        else
        {
            #ifndef __HIP_PLATFORM_NVCC__
            GPUInfo("Event Timing enabled...");
            hipExtLaunchKernelGGL(cerrf_eval, dim3( GRID_SIZE ), dim3( BLOCK_SIZE ),
                0, 0,  start, stop, 0, in_x_arg, in_y_arg, out_x_arg, out_y_arg,
                    NUM_EVALUATIONS );
            check_error( ::hipEventSynchronize( stop ) );
            #endif
        }

        float wtime = float{ 0.0 };
        check_error( ::hipEventElapsedTime( &wtime, start, stop ) );
        times[ rr ] = wtime;
    }

    status = ::hipMemcpy( x_result.data(), out_x_arg,
        buffer_size, ::hipMemcpyDeviceToHost );
    assert( status == hipSuccess );

    status = ::hipMemcpy( y_result.data(), out_y_arg,
        buffer_size, ::hipMemcpyDeviceToHost );
    assert( status == hipSuccess );

    std::sort( times.begin(), times.end() );
    std::cout << "slowest run : " << times.back()  << " msec\r\n"
              << "median      : " << times[ times.size() / 2 ] << " msec\r\n"
              << "fastest run : " << times.front() << " msec\r\n"
              << std::endl;

    /* ********************************************************************* */
    /* Cleaning up, Freeing resources */

    ::hipFree( in_x_arg );
    ::hipFree( in_y_arg );
    in_x_arg = nullptr;
    in_y_arg = nullptr;

    ::hipFree( out_x_arg );
    ::hipFree( out_y_arg );
    out_x_arg = nullptr;
    out_y_arg = nullptr;

    ::hipEventDestroy( start );
    ::hipEventDestroy( stop );

    return 0;
}
