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

__kernel void eval_wofz(
    __global double const* restrict in_x, __global double const* restrict in_y,
    __global double* restrict out_x, __global double* restrict out_y, long int const nn )
{
    long int const idx = get_global_id( 0 );
    if( idx < nn ) {
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

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */
#endif /* FADDEEVAS_CERNLIB_REVISED_C_H__ */
