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

#if !defined( CERRF_DEVICE_FN )
    #define CERRF_DEVICE_FN __device__
#endif /* !defined( CERRF_DEVICE_FN ) */

#if !defined( CERRF_HOST_FN )
    #define CERRF_HOST_FN __host__
#endif /* !defined( CERRF_HOST_FN ) */

#if !defined( CERRF_FN )
    #define CERRF_FN __host__ __device__
#endif /* !defined( CERRF_FN ) */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if !defined( CERRF_TWO_OVER_SQRT_PI )
    #define CERRF_TWO_OVER_SQRT_PI 1.1283791670955125739
#endif /* !defined( CERRF_TWO_OVER_SQRT_PI ) */

#if !defined( CERRF_SQRT_TWO )
    #define CERRF_SQRT_TWO 1.4142135623730950488
#endif /* !defined( CERRF_SQRT_TWO ) */

#if !defined( CERRF_X_LIMIT )
    #define CERRF_X_LIMIT 5.33
#endif /* !defined( CERRF_X_LIMIT ) */

#if !defined( CERRF_Y_LIMIT )
    #define CERRF_Y_LIMIT 4.29
#endif /* !defined( CERRF_Y_LIMIT ) */

#if !defined( CERRF_H0 )
    #define CERRF_H0 1.6
#endif /* !defined( CERRF_H0 ) */

#if !defined( CERRF_NU_0 )
    #define CERRF_NU_0 10
#endif /* !defined( CERRF_NU_0 ) */

#if !defined( CERRF_N0 )
    #define CERRF_N0 7
#endif /* !defined( CERRF_N0 ) */

#if !defined( CERRF_NU_1 )
    #define CERRF_NU_1 21
#endif /* !defined( CERRF_NU_1 ) */

#if !defined( CERRF_N1 )
    #define CERRF_N1 23
#endif /* !defined( CERRF_N1 ) */

#if !defined( CERRF_GAUSS_HERMITE_NU )
    #define CERRF_GAUSS_HERMITE_NU 9
#endif /* !defined( CERRF_GAUSS_HERMITE_NU ) */

#if !defined( CERRF_DOUBLE_EPS )
    #define  CERRF_DOUBLE_EPS 2.22e-16
#endif /* !defined( CERRF_DOUBLE_EPS ) */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

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


CERRF_DEVICE_FN double power_n( double x, unsigned int n )
{
    double x_n = x;

    unsigned int const n_div_16 = n >> 4u;
    unsigned int const n_mod_16 = n - ( n_div_16 << 4u );
    CERRF_ASSERT( n < 128u );
    CERRF_ASSERT( n_div_16 * 16u + n_mod_16 == n );
    CERRF_ASSERT( ( sqrt( x * x ) > ( double )CERRF_DOUBLE_EPS ) || ( n > 0u ) );

    switch( n_mod_16 )
    {
        case  0: { x_n = ( double )1.0; break; }
        case  1: { break; }
        case  2: { x_n *= x;                                       break; }
        case  3: { x_n *= x * x;                                   break; }
        case  4: { x_n *= x;     x_n *= x_n;                       break; }
        case  5: { x_n *= x;     x_n *= x_n * x;                   break; }
        case  6: { x_n *= x * x; x_n *= x_n;                       break; }
        case  7: { x_n *= x * x; x_n *= x_n * x;                   break; }
        case  8: { x_n *= x;     x_n *= x_n;     x_n *= x_n;       break; }
        case  9: { x_n *= x * x; x_n *= x_n * x_n;                 break; }
        case 10: { x_n *= x * x; x_n *= x_n * x_n * x;             break; }
        case 11: { x_n *= x;     x_n *= x_n * x; x_n *= x_n * x;   break; }
        case 12: { x_n *= x * x; x_n *= x_n;     x_n *= x_n;       break; }
        case 13: { x_n *= x * x; x_n *= x_n;     x_n *= x_n * x;   break; }
        case 14: { x_n *= x * x; x_n *= x_n * x; x_n *= x_n;       break; }
        case 15: { x_n *= x;     x_n *= x_n * x; x_n *= x_n * x_n; break; }

        default:
        {
            CERRF_ASSERT( n > 15u );
            x_n = ( double )0.0;
        }
    };

    if( n_div_16 > 0u ) { x *= x; x *= x; x *= x; x *= x; }

    switch( n_div_16 )
    {
        case  0: { x_n  = ( n_mod_16 != 0u ) ? x_n : ( double )1.0; break; }
        case  1: { x_n *= x;                                        break; }
        case  2: { x   *= x; x_n *= x;                              break; }
        case  3: { x_n *= x * x * x;                                break; }
        case  4: { x   *= x; x *= x; x_n *= x;                      break; }
        case  5: { x_n *= x; x *= x; x *= x; x_n *= x;              break; }
        case  6: { x   *= x * x; x *= x; x_n *= x;                  break; }
        case  7: { x_n *= x; x *= x * x; x *= x; x_n *= x;          break; }
        default:
        {
            CERRF_ASSERT( n < 128u );
            CERRF_ASSERT( n > 0u );
            x_n = ( double )0.0;
        }
    };

    return x_n;
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
        if( idx < nn )
    {
        double x = in_x[ idx ];
        double y = in_y[ idx ];

        /**
        this function calculates the double precision complex error function based on the
        algorithm of the FORTRAN function written at CERN by K. Koelbig, Program C335, 1970.

        See also M. Bassetti and G.A. Erskine, "Closed expression for the electric field of a
        two-dimensional Gaussian charge density", CERN-ISR-TH/80-06;
        */

        double const sign_x = ( double )( x >= ( double )0.0 ) -
                              ( double )( x <  ( double )0.0 );

        double const sign_y = ( double )( y >= ( double )0.0 ) -
                              ( double )( y <  ( double )0.0 );

        double inv_h2   = ( double )1.0;
        double temp     = y * sign_y;
        double y_plus_h = temp;

        double Rx, Ry, Sx, Sy, Wx, Wy, h2_n;

        int nu = ( int )CERRF_GAUSS_HERMITE_NU;
        int N  = 0;

        bool z_is_in_r0, use_taylor_sum;
        Ry = Sx = Sy = h2_n = ( double )0.0;

        y  = temp;
        x *= sign_x;

        z_is_in_r0 = ( ( y < ( double )CERRF_Y_LIMIT ) &&
                    ( x < ( double )CERRF_X_LIMIT ) );

        if( z_is_in_r0 )
        {
            temp = x / ( double )CERRF_X_LIMIT;
            temp = ( ( double )1.0 - y / ( double )CERRF_Y_LIMIT ) *
                sqrt( ( double )1.0 - temp * temp );


            nu   = ( y > ( double )CERRF_DOUBLE_EPS )
                ? ( int )CERRF_NU_0 + ( int )( ( double )CERRF_NU_1 * temp )
                : ( int )0;

            N    = ( int )CERRF_N0 + ( int )( ( double )CERRF_N1 * temp );
            h2_n = ( double )2.0 * ( double )CERRF_H0 * temp;

            CERRF_ASSERT( h2_n > ( double )CERRF_DOUBLE_EPS );
            inv_h2 = ( double )1.0 / h2_n;
            CERRF_ASSERT( inv_h2 > ( double )CERRF_DOUBLE_EPS );
            y_plus_h += ( double )0.5 * h2_n;
            h2_n = power_n( h2_n, N - 1 );
        }

        Rx = ( y > ( double )CERRF_DOUBLE_EPS )
        ? ( double )0.0
        : exp( -x * x ) / ( double )CERRF_TWO_OVER_SQRT_PI;

        use_taylor_sum = ( z_is_in_r0 ) && ( h2_n > ( double )CERRF_DOUBLE_EPS );

        for( int n = nu ; n > 0 ; n-- )
        {
            Wx     = y_plus_h + n * Rx;
            Wy     = x - n * Ry;
            temp   = ( double )0.5 / ( Wx * Wx + Wy * Wy );
            Rx     = temp * Wx;
            Ry     = temp * Wy;

            if( ( use_taylor_sum ) && ( n <= N ) )
            {
                temp   = h2_n + Sx;
                h2_n   = h2_n * inv_h2;
                Sx     = Rx * temp - Ry * Sy;
                Sy     = Ry * temp + Rx * Sy;
            }
        }

        /* Wx, Wy ... result for z|Q1 = |x| + i |y| ... in first quadrant! */
        Wx = ( double )CERRF_TWO_OVER_SQRT_PI * ( ( use_taylor_sum ) ? Sx : Rx );
        Wy = ( double )CERRF_TWO_OVER_SQRT_PI * ( ( use_taylor_sum ) ? Sy : Ry );

        if( sign_y < ( double )0.0 )  /* Quadrants 3 & 4 */
        {
            double const factor = ( double )2.0 * exp( y * y - x * x );
            temp = ( double )2.0 * x * y;

            Wx = factor * cos( temp ) - Wx;
            Wy = factor * sin( temp ) + Wy;
        }

        out_x[ idx ] = Wx;
        out_y[ idx ] = sign_x * Wy;
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
