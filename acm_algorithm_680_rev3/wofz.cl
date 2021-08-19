#define CERRF_RESTRICT restrict
#define CERRF_RESULT_DEC __global

#include "common/definitions.h"
#include "common/helper_functions.h"
#include "acm_algorithm_680_rev3/definitions.h"

#if defined( CERRF_USE_DAWSON_FUNCTION ) && \
           ( CERRF_USE_DAWSON_FUNCTION == 1 )
    #include "dawson/definitions.h"
    #include "dawson/dawson.h"
#endif /* ( CERRF_USE_DAWSON_FUNCTION == 1 ) */


__kernel void eval_wofz(
    __global double const* CERRF_RESTRICT in_x,
    __global double const* CERRF_RESTRICT in_y,
    CERRF_RESULT_DEC double* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC double* CERRF_RESTRICT out_y,
    long int const nn )
{
    typedef CERRF_REAL_TYPE real_type;
    long int const idx = get_global_id( 0 );

    if( idx < nn )
    {
        real_type x = in_x[ idx ];
        real_type y = in_y[ idx ];

        #if defined( __cplusplus )
        using std::sqrt;
        using std::log;
        using std::round;
        using std::floor;
        #if !defined( CERRF_USE_SINCOS )
        using std::cos;
        using std::sin;
        #else /* defined( CERRF_USE_SINCOS ) */
        using ::sincos
        #endif /* !defined( CERRF_USE_SINCOS ) */
        using std::exp;
        #endif /* defined( __cplusplus ) */

        real_type wz_re = ( real_type )0.0;
        real_type wz_im = ( real_type )0.0;

        real_type const sign_re = ( real_type )( x >= ( real_type )0.0 ) -
                                ( real_type )( x <  ( real_type )0.0 );

        real_type const sign_im = ( real_type )( y >= ( real_type )0.0 ) -
                                ( real_type )( y <  ( real_type )0.0 );

        real_type q_rho_squ = ( real_type )0.0;
        real_type xs    = x / ( real_type )CERRF_X_LIMIT;
        real_type ys    = y / ( real_type )CERRF_Y_LIMIT;

        real_type x_quad, y_quad, exp_minus_x_quad, factor_cos, factor_sin;

        bool y_is_zero  = false;
        bool use_power_series = false;

        x  *= sign_re;
        xs *= sign_re;

        y  *= sign_im;
        ys *= sign_im;

        #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
        bool const use_dawson_approx = (
            ( y <= ( real_type )CERRF_USE_DAWSON_APPROX_MAX_Y ) &&
            ( x >= ( real_type )CERRF_USE_DAWSON_APPROX_MIN_X ) &&
            ( x <= ( real_type )CERRF_USE_DAWSON_APPROX_MAX_X ) );
        #endif /* ( CERRF_USE_DAWSON_FUNCTION == 1 ) */

        if( ( x > ( real_type )CERRF_REAL_MAX_X ) ||
            ( y > ( real_type )CERRF_REAL_MAX_Y ) )
            return;

        q_rho_squ  = xs * xs;
        q_rho_squ += ys * ys;

        y_is_zero = ( y == ( real_type )0.0 );
        use_power_series = (
            #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
            ( !use_dawson_approx ) &&
            #endif /* ( CERRF_USE_DAWSON_FUNCTION == 1 ) */
            ( q_rho_squ < ( real_type )0.0852640 ) );

        x_quad = ( ( use_power_series ) || ( sign_im < ( real_type )0.0 ) )
            ? ( x - y ) * ( x + y ) : ( real_type )0.0;

        y_quad = ( ( use_power_series ) || ( sign_im < ( real_type )0.0 ) )
            ? ( real_type )2.0 * x * y : ( real_type )0.0;

        exp_minus_x_quad = ( ( use_power_series ) || ( sign_im < ( real_type )0.0 ) )
            ? exp( -x_quad ) : ( real_type )1.0;

        #if !defined( CERRF_USE_SINCOS )

        factor_cos = ( ( use_power_series ) || ( sign_im < ( real_type )0.0 ) )
            ? exp_minus_x_quad * cos( y_quad ) : ( real_type )1.0;

        factor_sin = ( ( use_power_series ) || ( sign_im < ( real_type )0.0 ) )
            ? exp_minus_x_quad * sin( y_quad ) : ( real_type )1.0;

        #else /* defined( CERRF_USE_SINCOS ) */

        if( ( use_power_series ) || ( sign_im < ( real_type )0.0 ) )
        {
            factor_sin = sincos( y_quad, &factor_cos );
        }
        else
        {
            factor_sin = factor_cos = ( real_type )1.0;
        }

        factor_cos *= exp_minus_x_quad;
        factor_sin *= exp_minus_x_quad;

        #endif /* !defined( CERRF_USE_SINCOS ) */

        if( use_power_series )
        {
            real_type temp = ( real_type )0.0;
            real_type const q_rho = sqrt( q_rho_squ ) * (
                ( real_type )CERRF_QRHO_C0 - ( real_type )CERRF_QRHO_C1 * ys );

            #if defined( CERRF_USE_SAFE_ROUND )
            int const N = ( int )floor( ( real_type )CERRF_N0 + ( real_type )CERRF_N1 * q_rho );
            #else
            int const N = ( int )( ( real_type )CERRF_N0 + ( real_type )CERRF_N1 * q_rho );
            #endif /* defined( CERRF_USE_SAFE_ROUND ) */

            int ii = N;

            real_type kk = ( real_type )N;
            real_type jj = ( real_type )2.0 * kk + ( real_type )1.0;
            real_type uu = ( real_type )0.0;
            real_type vv = ( real_type )0.0;

            wz_re  = ( real_type )1.0 / jj;

            for( ; ii > 0 ; --ii, kk -= ( real_type )1.0 )
            {
                real_type const c1 = ( wz_re * x_quad ) - ( wz_im * y_quad );
                real_type const c2 = ( wz_re * y_quad ) + ( wz_im * x_quad );
                real_type const inv_kk  = ( real_type )1.0 / kk;
                jj   -= ( real_type )2.0;
                temp  = c1 * inv_kk;
                wz_im = c2 * inv_kk;
                wz_re = temp + ( real_type )1.0 / jj;
            }

            uu  = ( real_type )1.0;
            uu -= ( real_type )CERRF_TWO_OVER_SQRT_PI * ( wz_re * y + wz_im * x );
            vv  = ( real_type )CERRF_TWO_OVER_SQRT_PI * ( wz_re * x - wz_im * y );

            wz_re = +uu * factor_cos + vv * factor_sin;
            wz_im = -uu * factor_sin + vv * factor_cos;
        }
        #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
        else if( !use_dawson_approx )
        #else /* !CERRF_USE_DAWSON_FUNCTION */
        else
        #endif /* CERRF_USE_DAWSON_FUNCTION */
        {
            bool use_cont_fraction = ( ( real_type )q_rho_squ >= (
                real_type )CERRF_QRHO_SQU_CONTINUED_FRACTION_LIMIT );

            real_type const q_rho = ( use_cont_fraction )
                ? sqrt( q_rho_squ )
                : ( ( real_type )1.0 - ys ) * sqrt( ( real_type )1.0 - q_rho_squ );

            real_type const h = ( use_cont_fraction )
                ? ( real_type )0.0 : ( real_type )CERRF_H1 * q_rho;

            real_type const two_h = ( real_type )2.0 * h;
            real_type const inv_two_h = ( use_cont_fraction )
                ? ( real_type )1.0 : ( real_type )1.0 / two_h;

            #if CERRF_USE_SAFE_ROUND

            int const NU = ( use_cont_fraction )
                ? ( int )round( ( real_type )CERRF_NU0_CONT_FRACTION +
                    ( real_type )CERRF_NU1_CONT_FRACTION / (
                        ( real_type )CERRF_NU2_CONT_FRACTION +
                        ( real_type )CERRF_NU3_CONT_FRACTION * q_rho ) )
                : ( int )round( ( real_type )CERRF_NU0 +
                                ( real_type )CERRF_NU1 * q_rho );

            int const KAPN = ( use_cont_fraction )
                ? ( int )-1
                : ( int )round( ( real_type )CERRF_KAPN0 +
                                ( real_type )CERRF_KAPN1 * q_rho );

            #else

            int const NU = ( use_cont_fraction )
                ? ( int )( ( real_type )CERRF_NU0_CONT_FRACTION + ( real_type )0.5 +
                    ( real_type )CERRF_NU1_CONT_FRACTION / (
                        ( real_type )CERRF_NU2_CONT_FRACTION +
                        ( real_type )CERRF_NU3_CONT_FRACTION * q_rho ) )
                : ( int )floor( ( real_type )CERRF_NU0 + ( real_type )0.5 +
                                ( real_type )CERRF_NU1 * q_rho );

            int const KAPN = ( use_cont_fraction )
                ? ( int )-1
                : ( int )( ( real_type )CERRF_KAPN0 + ( real_type )0.5 +
                        ( real_type )CERRF_KAPN1 * q_rho );

            #endif

            int n = NU;

            real_type nn_plus_1 = ( real_type )n + ( real_type )1.0;
            real_type rx = ( real_type )0.0;
            real_type ry = ( real_type )0.0;
            real_type sx = ( real_type )0.0;
            real_type sy = ( real_type )0.0;
            real_type q_lambda = ( real_type )0.0;

            use_cont_fraction = ( ( h == ( real_type )0.0 ) || ( KAPN == 0 ) );

            q_lambda = ( !use_cont_fraction )
                ? power_n( two_h, ( unsigned int )KAPN ) : ( real_type )1.0;

            for( ; n > KAPN ; --n, nn_plus_1 -= ( real_type )1.0 )
            {
                real_type const tx = y + h + nn_plus_1 * rx;
                real_type const ty = x - nn_plus_1 * ry;
                real_type temp = ( tx * tx ) + ( ty * ty );
                rx   = ( real_type )0.5 * tx;
                ry   = ( real_type )0.5 * ty;
                temp = ( real_type )1.0 / temp;
                rx  *= temp;
                ry  *= temp;
            }

            for( ; n >= 0 ; --n, nn_plus_1 -= ( real_type )1.0 )
            {
                real_type const ty = x - nn_plus_1 * ry;
                real_type tx       = y + h + nn_plus_1 * rx;
                real_type temp     = ( tx * tx ) + ( ty * ty );
                rx                 = ( real_type )0.5 * tx;
                ry                 = ( real_type )0.5 * ty;
                temp  = ( real_type )1.0 / temp;
                rx   *= temp;
                ry   *= temp;

                tx        = q_lambda + sx;
                q_lambda *= inv_two_h;
                sx        = ( rx * tx ) - ( ry * sy );
                sy        = ( ry * tx ) + ( rx * sy );
            }

            wz_re = ( real_type )CERRF_TWO_OVER_SQRT_PI * ( ( use_cont_fraction ) ? rx : sx );
            wz_im = ( real_type )CERRF_TWO_OVER_SQRT_PI * ( ( use_cont_fraction ) ? ry : sy );
        }
        #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
        else
        {
            dawson_cerrf( x, y, &wz_re, &wz_im );
        }
        #else  /* ( CERRF_USE_DAWSON_FUNCTION != 1 ) */
        if( y_is_zero )
        {
            real_type const x_squ = x * x;
            wz_re = ( x_squ < ( real_type )CERRF_MAX_REAL_MAX_EXP )
                ? exp( -x_squ ) : ( real_type )0.0;
        }
        #endif /* ( CERRF_USE_DAWSON_FUNCTION != 1 ) */

        if( y_is_zero )
        {
            real_type const x_squ = x * x;
            wz_re = ( x_squ < ( real_type )CERRF_MAX_REAL_MAX_EXP )
                ? exp( -x_squ ) : ( real_type )0.0;
        }

        if( sign_im < ( double )0.0 )
        {
            wz_re = ( real_type )2.0 * factor_cos - wz_re;
            wz_im = ( real_type )2.0 * factor_sin + wz_im;
        }

        out_x[ idx ] = wz_re;
        out_y[ idx ] = sign_re * wz_im;
    }
}
