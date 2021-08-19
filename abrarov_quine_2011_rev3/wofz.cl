#define CERRF_RESTRICT restrict
#define CERRF_RESULT_DEC __global

#include "abrarov_quine_2011_rev3/definitions.h"
#include "common/definitions.h"
#if defined( CERRF_USE_DAWSON_FUNCTION ) && \
           ( CERRF_USE_DAWSON_FUNCTION == 1 )
    #include "dawson/definitions.h"
    #include "dawson/dawson.h"
#endif /* ( CERRF_USE_DAWSON_FUNCTION == 1 ) */

/*

 CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_FOURIER_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT fourier_coeff,
    CERRF_TAYLOR_COEFF_DEC CERRF_REAL_TYPE const*  CERRF_RESTRICT taylor_coeff
) CERRF_NOEXCEPT;

*/

__kernel void eval_wofz(
    __global CERRF_REAL_TYPE const* CERRF_RESTRICT in_real,
    __global CERRF_REAL_TYPE const* CERRF_RESTRICT in_imag,
    __global CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    __global CERRF_REAL_TYPE* CERRF_RESTRICT out_y, long int const nn,
    CERRF_FOURIER_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT fourier_coeff,
    CERRF_TAYLOR_COEFF_DEC  CERRF_REAL_TYPE const* CERRF_RESTRICT taylor_coeff )
{
    long int const idx = get_global_id( 0 );

    if( idx < nn )
    {
        typedef CERRF_REAL_TYPE real_type;

        real_type in_x = in_real[ idx ];
        real_type in_y = in_imag[ idx ];

        #if defined( __cplusplus )
        using std::sqrt;
        using std::sin;
        using std::cos;
        using std::exp;
        using std::printf;
        #if defined( CERRF_USE_SAFE_ROUND_TRUNC ) && \
                ( CERRF_USE_SAFE_ROUND_TRUNC == 1 )
        using std::round;
        #endif /* defined( CERRF_USE_SAFE_ROUND_TRUNC ) */

        #if defined( CERRF_USE_CRLIBM ) && ( CERRF_USE_CRLIBM == 1 )
        using ::crlibm_init;
        using ::cos_rn;
        using ::sin_rn;
        using ::exp_rn;
        #endif

        #if defined( CERRF_USE_SINCOS ) && ( CERRF_USE_SINCOS == 1 )
        using ::sincos;
        #endif /* defined( CERRF_USE_SINCOS ) && ( CERRF_USE_SINCOS == 1 ) */
        #endif

        real_type const sign_x = ( real_type )( in_x >= ( real_type )0.0 ) -
                                 ( real_type )( in_x <  ( real_type )0.0 );

        real_type const sign_y = ( real_type )( in_y >= ( real_type )0.0 ) -
                                 ( real_type )( in_y <  ( real_type )0.0 );

        real_type wz_re        = ( real_type )0.0;
        real_type wz_im        = ( real_type )0.0;
        real_type temp         = ( real_type )0.0;

        int N_POLE = -1;

        in_x       = in_x * sign_x;
        in_y       = in_y * sign_y;

        CERRF_ASSERT( out_x           != NULL );
        CERRF_ASSERT( out_y           != NULL );
        CERRF_ASSERT( fourier_coeff   != NULL );
        CERRF_ASSERT( taylor_coeff    != NULL );

        if( ( in_y < (  ( real_type )CERRF_MIN_POLE_DIST ) ) &&
            ( in_x < (  ( real_type )CERRF_N_FOURIER *
                        ( real_type )CERRF_PI_OVER_TM +
                        ( real_type )CERRF_MIN_POLE_DIST ) ) )
        {
            real_type dist_to_pole_squ = in_y * in_y;

            #if !defined( CERRF_USE_SAFE_ROUND_TRUNC ) || \
                        ( CERRF_USE_SAFE_ROUND_TRUNC != 1 )
            N_POLE = ( int )( in_x * ( real_type )CERRF_TM_OVER_PI +
                                    ( real_type )0.5 );
            #else /* defined( CERRF_USE_SAFE_ROUND_TRUNC ) */
            N_POLE = ( int )round( in_x * ( real_type )CERRF_TM_OVER_PI );
            #endif /* !defined( CERRF_USE_SAFE_ROUND_TRUNC ) */

            temp   = in_x - ( real_type )CERRF_PI_OVER_TM * ( real_type )N_POLE;
            dist_to_pole_squ += temp * temp;
            if( dist_to_pole_squ >= ( real_type )CERRF_MIN_POLE_DIST_SQU ) N_POLE = -1;

            CERRF_ASSERT( ( N_POLE == -1 ) || ( N_POLE >= ( int )0 ) );
            CERRF_ASSERT( N_POLE <= ( int )CERRF_N_FOURIER );
        }

        #if defined( CERRF_USE_CONTINUOUS_FRACTION ) && ( CERRF_USE_CONTINUOUS_FRACTION == 1 )
        if( ( in_x * in_x ) + ( in_y * in_y ) >=
            ( real_type )CERRF_CONTINUOUS_FRACTION_LIMIT_SQU )
        {
            real_type rx = ( real_type )0.0;
            real_type ry = ( real_type )0.0;
            real_type nn = ( real_type )CERRF_CONTINUOUS_FRACTION_K;
            int n = ( int )CERRF_CONTINUOUS_FRACTION_K;

            for( ; n > 0 ; --n, nn -= ( real_type )1.0 )
            {
                wz_re = in_y + nn * rx;
                wz_im = in_x - nn * ry;
                temp  = ( wz_re * wz_re + wz_im * wz_im );
                rx    = ( real_type )0.5 * wz_re;
                ry    = ( real_type )0.5 * wz_im;
                temp  = ( real_type )1.0 / temp;
                rx   *= temp;
                ry   *= temp;
            }

            wz_re = ( real_type )CERRF_TWO_OVER_SQRT_PI * rx;
            wz_im = ( real_type )CERRF_TWO_OVER_SQRT_PI * ry;

        }
        else
        #endif
        #if defined( CERRF_USE_DAWSON_FUNCTION ) && ( CERRF_USE_DAWSON_FUNCTION == 1 )
        if( ( in_x >= ( real_type )CERRF_DAWSON_X_MIN ) &&
            ( in_x <= ( real_type )CERRF_DAWSON_X_MAX ) &&
            ( in_y <= ( real_type )CERRF_DAWSON_Y_MAX ) )
        {
            dawson_cerrf( in_x, in_y, &wz_re, &wz_im );
        }
        else
        #endif
        if( N_POLE == -1 )
        {
            real_type exp_cos_tm_x = ( real_type )0.0;
            real_type exp_sin_tm_x = ( real_type )0.0;

            real_type sum_re       = ( real_type )0.0;
            real_type sum_im       = ( real_type )0.0;

            real_type sn_re        = ( real_type )0.0;
            real_type sn_im        = ( real_type )0.0;

            real_type mm           = ( real_type )1.0;
            real_type c3           = ( real_type )0.0;
            real_type c4           = ( real_type )1.0;

            real_type const c1  = ( real_type )CERRF_TM_SQU *
                                ( in_x + in_y ) * ( in_x - in_y );

            real_type const c2  = ( real_type )2.0 *
                                ( real_type )CERRF_TM_SQU * in_x * in_y;

            real_type const c2_squ = c2 * c2;

            int nn = 1;

            temp = ( real_type )CERRF_TM * in_x;

            #if defined( CERRF_USE_CRLIBM ) && ( CERRF_USE_CRLIBM == 1 )
            exp_sin_tm_x = sin_rn( temp );
            exp_cos_tm_x = cos_rn( temp );
            #elif defined( CERRF_USE_SINCOS ) && ( CERRF_USE_SINCOS == 1 )
            sincos( temp, &exp_sin_tm_x, &exp_cos_tm_x );
            #else  /* !defined( CERRF_USE_SINCOS ) || ( CERRF_USE_SINCOS != 1 ) */
            exp_sin_tm_x = sin( temp );
            exp_cos_tm_x = cos( temp );
            #endif /* defined( CERRF_USE_SINCOS ) && ( CERRF_USE_SINCOS == 1 ) */

            temp = ( real_type )CERRF_TM * in_y;

            #if defined( CERRF_USE_CRLIBM ) && ( CERRF_USE_CRLIBM == 1 )
            temp = exp_rn( -temp );
            #else
            temp = exp( -temp );
            #endif
            exp_sin_tm_x *= temp;
            exp_cos_tm_x *= temp;

            c4    -= exp_cos_tm_x;

            temp   = ( in_x * in_x ) + ( in_y * in_y );
            temp  *= ( real_type )CERRF_TM;
            wz_re  = ( in_y * c4 ) + ( in_x * exp_sin_tm_x );
            wz_im  = ( in_x * c4 ) - ( in_y * exp_sin_tm_x );
            temp   = ( real_type )1.0 / temp;
            wz_re *= temp;
            wz_im *= temp;

            for( ; nn < CERRF_N_FOURIER ; ++nn, mm += ( real_type )1.0 )
            {
                exp_sin_tm_x = -exp_sin_tm_x;
                exp_cos_tm_x = -exp_cos_tm_x;
                c4           =  exp_cos_tm_x - ( real_type )1.0;
                c3           =  ( mm * mm * ( real_type )CERRF_PI_SQU ) - c1;
                temp         =  c2_squ + ( c3 * c3 );

                sn_re        = ( c3 * c4 ) - ( c2 * exp_sin_tm_x );
                sn_im        = ( c2 * c4 ) + ( c3 * exp_sin_tm_x );

                sn_re       *= ( real_type )fourier_coeff[ nn ];
                sn_im       *= ( real_type )fourier_coeff[ nn ];

                temp         = ( real_type )1.0 / temp;
                sum_re      += sn_re * temp;
                sum_im      += sn_im * temp;
            }

            /* normalize the sum + apply common pre-factor i * z */

            temp    = ( -in_x * sum_im ) - ( in_y * sum_re );
            sum_im  = (  in_x * sum_re ) - ( in_y * sum_im );

            wz_re  += temp   * ( real_type )CERRF_TM_SQU_OVER_SQRT_PI;
            wz_im  += sum_im * ( real_type )CERRF_TM_SQU_OVER_SQRT_PI;
        }
        else
        {
            real_type const dz_re = temp;
            real_type dz_nn_re    = temp;
            real_type dz_nn_im    = in_y;

            int ii = 2;
            int jj = 2 * N_POLE * ( int )CERRF_N_TAYLOR;

            CERRF_ASSERT( N_POLE >= 0 );
            CERRF_ASSERT( N_POLE <= ( int )CERRF_N_FOURIER );

            /* wz = Re(b0) + i * Im(b0) */
            wz_re  =  taylor_coeff[ jj++ ];
            wz_im  =  taylor_coeff[ jj++ ];

            /* wz += b1 * ( dz_re + i * in_y ) */
            wz_re += taylor_coeff[ jj   ] * dz_nn_re;
            wz_im += taylor_coeff[ jj++ ] * dz_nn_im;

            wz_re -= taylor_coeff[ jj   ] * dz_nn_im;
            wz_im += taylor_coeff[ jj++ ] * dz_nn_re;

            for( ; ii < ( int )CERRF_N_TAYLOR ; ++ii )
            {
                temp      = dz_nn_re * dz_re - dz_nn_im * in_y;
                dz_nn_im *= dz_re;
                dz_nn_im += dz_nn_re * in_y;
                dz_nn_re  = temp;

                wz_re += taylor_coeff[ jj   ] * dz_nn_re;
                wz_im += taylor_coeff[ jj++ ] * dz_nn_im;

                wz_re -= taylor_coeff[ jj   ] * dz_nn_im;
                wz_im += taylor_coeff[ jj++ ] * dz_nn_re;
            }
        }

        if( sign_y < ( real_type )0.0 )  /* Quadrants 3 & 4 */
        {
            real_type const phase = ( real_type )2.0 * in_x * in_y;
            real_type const factor = ( real_type )2.0 * exp(
                ( in_y + in_x ) * ( in_y - in_x ) );

            #if defined( CERRF_USE_SINCOS ) && ( CERRF_USE_SINCOS == 1 )
            real_type sin_phase = ( real_type )0.0;
            real_type cos_phase = ( real_type )0.0;
            sincos( phase, &sin_phase, &cos_phase );

            wz_re = factor * cos_phase - wz_re;
            wz_im = factor * sin_phase + wz_im;

            #else /* !defined( CERRF_USE_SINCOS ) || ( CERRF_USE_SINCOS != 1 )*/

            wz_re = factor * cos( phase ) - wz_re;
            wz_im = factor * sin( phase ) + wz_im;

            #endif /* defined( CERRF_USE_SINCOS ) && ( CERRF_USE_SINCOS == 1 ) */
        }

        *out_x = wz_re;
        *out_y = wz_im;
    }
}
