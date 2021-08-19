#ifndef FADDEEVAS_CERNLIB_REVISED_C_H__
#define FADDEEVAS_CERNLIB_REVISED_C_H__

#if !defined( CERRF_NO_SYSTEM_INCLUDES )
#if !defined( __cplusplus )
    #include <assert.h>
    #include <stdbool.h>
    #include <stddef.h>
    #include <math.h>
#else /* defined( __cplusplus ) */
    #include <cassert>
    #include <cstddef>
    #include <cmath>
#endif /* !defined( __cplusplus ) */
#endif /* !defined( CERRF_NO_SYSTEM_INCLUDES ) */

#if !defined( CERRF_NO_INCLUDES )
    #if defined( CERRF_USE_DAWSON_FUNCTION ) && \
               ( CERRF_USE_DAWSON_FUNCTION == 1 )

        #if !defined( CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 )
            #define CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 1
        #endif /* !defined( CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 ) */

        #include "dawson/definitions.h"
        #include "dawson/dawson.h"

        #if defined( CERRF_USE_DAWSON_COEFF_TABLES ) && \
                   ( CERRF_USE_DAWSON_COEFF_TABLES == 1 )
            #include "dawson/coefficients.h"
        #endif /* defined( CERRF_USE_DAWSON_COEFF_TABLES ) */

    #endif /* ( CERRF_USE_DAWSON_FUNCTION == 1 ) */
#endif /* */

#if !defined( CERRF_STATIC )
    #if !defined( _GPUCODE )
        #define CERRF_STATIC static
    #else /* !defined( _GPUCODE ) */
        #define CERRF_STATIC
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_STATIC ) */

#if !defined( CERRF_EXTERN )
    #if !defined( _GPUCODE )
        #define CERRF_EXTERN extern
    #else /* !defined( _GPUCODE ) */
        #define CERRF_EXTERN
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_EXTERN ) */

#if !defined( CERRF_FN )
    #define CERRF_FN
#endif /* !defined( CERRF_FN ) */

#if !defined( CERRF_INLINE )
    #define CERRF_INLINE inline
#endif /* !defined( CERRF_INLINE ) */

#if !defined( CERRF_ASSERT )
    #if !defined( _GPUCODE )
        #define CERRF_ASSERT( cond ) assert( ( cond ) )
    #else /* !defined( _GPUCODE ) */
        #define CERRF_ASSERT( cond )
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_ASSERT ) */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if !defined( CERRF_REAL_TYPE )
    #define CERRF_REAL_TYPE double
#endif /* CERRF_REAL_TYPE */

#if !defined( CERRF_DOUBLE_EPS )
    #define  CERRF_REAL_EPS 2.22044604925031e-16
#endif /* !defined( CERRF_DOUBLE_EPS ) */

#if !defined( CERRF_REAL_MIN )
    #define CERRF_REAL_MIN 2.22507385850720e-308
#endif /* !defined( CERRF_REAL_MIN ) */

#if !defined( CERRF_REAL_MAX )
    #define CERRF_REAL_MAX 1.79769313486232e+308
#endif /* !defined( CERRF_REAL_MAX ) */

#if !defined( CERRF_PI )
    #define CERRF_PI 3.141592653589793238462643383279502884197169399375105820974944592
#endif /* !defined( CERRF_PI ) */

#if !defined( CERRF_PI_SQU )
    #define CERRF_PI_SQU 9.869604401089358618834490999876151135313699407240790626413349376371
#endif /* !defined( CERRF_PI_SQU ) */

#if !defined( CERRF_SQRT_PI )
    #define CERRF_SQRT_PI 1.77245385090551602729816748334114518279754945612238712821380779
#endif /* !defined( CERRF_SQRT_PI ) */

#if !defined( CERRF_SQRT_TWO )
    #define CERRF_SQRT_TWO 1.414213562373095048801688724209698078569671875376948073176679738
#endif /* !defined( CERRF_SQRT_TWO ) */

#if !defined( CERRF_TWO_OVER_SQRT_PI )
    #define CERRF_TWO_OVER_SQRT_PI 1.128379167095512573896158903121545171688101258657997713688171443
#endif /* CERRF_TWO_OVER_SQRT_PI */

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

#if !defined( CERRF_USE_DAWSON_APPROX_MAX_Y )
    #define CERRF_USE_DAWSON_APPROX_MAX_Y 0.5
#endif /* !defined( CERRF_USE_DAWSON_APPROX_MAX_Y ) */

#if !defined( CERRF_USE_DAWSON_APPROX_MIN_X )
    #define CERRF_USE_DAWSON_APPROX_MIN_X 0.0
#endif /* !defined( CERRF_USE_DAWSON_APPROX_MIN_X ) */

#if !defined( CERRF_USE_DAWSON_APPROX_MAX_X )
    #define CERRF_USE_DAWSON_APPROX_MAX_X 9.0
#endif /* !defined( CERRF_USE_DAWSON_APPROX_MAX_X ) */


#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */


CERRF_STATIC CERRF_FN double power_n( double x, unsigned int n );
CERRF_STATIC CERRF_FN void cerrf_rev2( double x, double y,
    double* __restrict__ out_x, double* __restrict__ out_y );

/* ------------------------------------------------------------------------- */

#if !defined( _GPUCODE )

CERRF_EXTERN double power_n_ext( double const x, unsigned int const n );

CERRF_EXTERN void cerrf_rev2_ext( double const x, double const y,
    double* __restrict__ out_x, double* __restrict__ out_y );

#endif /* !defined( _GPUCODE ) */

/* ************************************************************************* */

CERRF_INLINE double power_n( double x, unsigned int n )
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

__kernel void eval_wofz(
    __global double const* restrict in_x,
    __global double const* restrict in_y,
    __global double* restrict out_x,
    __global double* restrict out_y, long int const nn )
{
    typedef CERRF_REAL_TYPE real_type;

    long int const idx = get_global_id( 0 );

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

        real_type const sign_x = ( real_type )( x >= ( real_type )0.0 ) -
                                 ( real_type )( x <  ( real_type )0.0 );

        real_type const sign_y = ( real_type )( y >= ( real_type )0.0 ) -
                                 ( real_type )( y <  ( real_type )0.0 );

        real_type inv_h2   = ( real_type )1.0;
        real_type temp     = y * sign_y;
        real_type y_plus_h = temp;

        real_type Rx, Ry, Sx, Sy, Wx, Wy, h2_n, nn;

        int nu = ( int )CERRF_GAUSS_HERMITE_NU;
        int N  = 0;
        int n  = 0;

        y  = temp;
        x *= sign_x;

        #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
        bool const use_dawson_approx = (
            ( y <= ( real_type )CERRF_USE_DAWSON_APPROX_MAX_Y ) &&
            ( x >= ( real_type )CERRF_USE_DAWSON_APPROX_MIN_X ) &&
            ( x <= ( real_type )CERRF_USE_DAWSON_APPROX_MAX_X ) );
        #endif /* CERRF_USE_DAWSON_FUNCTION */

        bool use_taylor_sum = (
            #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
            ( !use_dawson_approx ) &&
            #endif /* ( CERRF_USE_DAWSON_FUNCTION == 1 ) */
            ( y < ( real_type )CERRF_Y_LIMIT ) &&
            ( x < ( real_type )CERRF_X_LIMIT ) );

        CERRF_ASSERT( ( out_x != NULL ) && ( out_y != NULL ) );
        Ry = Sx = Sy = h2_n = ( real_type )0.0;

        if( use_taylor_sum )
        {
            #if !defined( CERRF_NO_GZ_WEIGHT_FN )
            /* calculate g(z) = sqrt( 1 - (x/x0)^2 ) * ( 1 - y/y0 ) */
            temp  = x * ( ( real_type )1. / ( real_type )CERRF_X_LIMIT );
            temp  = ( ( real_type )1.0 +  temp ) * ( ( real_type )1.0 - temp );
            temp  = sqrt( temp );
            temp *= ( real_type )1. - y * ( ( real_type )1. / ( real_type )CERRF_Y_LIMIT );
            /*now: temp = g(z) */
            #else /* !defined( CERRF_NO_GZ_WEIGHT_FN ) */
            temp = ( real_type )1.;
            #endif /* defined( CERRF_NO_GZ_WEIGHT_FN ) */

            nu       = ( y > ( real_type )CERRF_REAL_EPS )
                    ? ( int )CERRF_NU_0 + ( int )( ( real_type )CERRF_NU_1 * temp )
                    : ( int )0;

            N         = ( int )CERRF_N0 + ( int )( ( real_type )CERRF_N1 * temp );
            h2_n      = ( real_type )CERRF_H0 * temp;
            y_plus_h += h2_n;
            h2_n     *= ( real_type )2.0;
            inv_h2    = ( real_type )1.0 / h2_n;
            h2_n      = power_n( h2_n, N - 1 );

            use_taylor_sum = ( h2_n > ( real_type )CERRF_REAL_EPS );
        }

        Rx = ( y > ( real_type )CERRF_REAL_EPS )
        ? ( real_type )0.0
        : exp( -x * x ) / ( real_type )CERRF_TWO_OVER_SQRT_PI;

        #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
        n  = ( !use_dawson_approx ) ? nu : 0;
        #else /* ( CERRF_USE_DAWSON_FUNCTION != 1 ) */
        n  = nu;
        #endif /* ( CERRF_USE_DAWSON_FUNCTION == 1 ) */

        nn = ( real_type )n;

        for( ; n > N ; --n, nn -= ( real_type )1.0 )
        {
            Wx     = y_plus_h + nn * Rx;
            Wy     = x - nn * Ry;
            temp   = ( Wx * Wx + Wy * Wy );
            Rx     = ( real_type )0.5 * Wx;
            Ry     = ( real_type )0.5 * Wy;
            temp   = ( real_type )1.0 / temp;
            Rx    *= temp;
            Ry    *= temp;
        }

        for( ; n > 0 ; --n, nn -= ( real_type )1.0 )
        {
            Wx     = y_plus_h + nn * Rx;
            Wy     = x - nn * Ry;
            temp   = ( Wx * Wx + Wy * Wy );
            Rx     = ( real_type )0.5 * Wx;
            Ry     = ( real_type )0.5 * Wy;
            temp   = ( real_type )1.0 / temp;
            Rx    *= temp;
            Ry    *= temp;

            Wx     = h2_n + Sx;
            h2_n  *= inv_h2;
            Sx     = Rx * Wx - Ry * Sy;
            Sy     = Ry * Wx + Rx * Sy;
        }

        /* Wx, Wy ... result for z|Q1 = |x| + i |y| ... in first quadrant! */
        if( use_taylor_sum )
        {
            Wx = ( real_type )CERRF_TWO_OVER_SQRT_PI * Sx;
            Wy = ( real_type )CERRF_TWO_OVER_SQRT_PI * Sy;
        }
        #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
        else if( !use_dawson_approx )
        #else
        else
        #endif
        {
            Wx = ( real_type )CERRF_TWO_OVER_SQRT_PI * Rx;
            Wy = ( real_type )CERRF_TWO_OVER_SQRT_PI * Ry;
        }
        #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
        else
        {
            #if !defined( CERRF_USE_DAWSON_COEFF_TABLES ) || \
                        ( CERRF_USE_DAWSON_COEFF_TABLES != 1 )
            dawson_cerrf( x, y, &Wx, &Wy );
            #else /* ( CERRF_USE_DAWSON_COEFF_TABLES == 1 ) */
            dawson_cerrf_coeff( x, y, &Wx, &Wy,
                                &CERRF_DAWSON_N_TAYLOR[ 0 ],
                                &CERRF_DAWSON_TAYLOR_COEFF[ 0 ] );
            #endif /* defined( CERRF_USE_DAWSON_COEFF_TABLES ) */

        }
        #endif /* ( CERRF_USE_DAWSON_FUNCTION == 1 ) */

        if( sign_y < ( real_type )0.0 )  /* Quadrants 3 & 4 */
        {
            real_type const exp_arg  = ( y - x ) * ( y + x );
            real_type exp_sin_arg, exp_cos_arg;
            temp = ( real_type )2.0 * exp( exp_arg );
            exp_sin_arg = sincos( ( real_type )2.0 * x * y, &exp_cos_arg );
            exp_sin_arg *= temp;
            exp_cos_arg *= temp;

            Wx = exp_cos_arg - Wx;
            Wy = exp_sin_arg + Wy;
        }

        out_x[ idx ] = Wx;
        out_y[ idx ] = sign_x * Wy;
    }
}

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */
#endif /* FADDEEVAS_CERNLIB_REVISED_C_H__ */
