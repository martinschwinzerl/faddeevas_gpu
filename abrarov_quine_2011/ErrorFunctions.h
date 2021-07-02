#ifndef FADDEEVAS_ABRAROV_QUINE_2011_H__
#define FADDEEVAS_ABRAROV_QUINE_2011_H__

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
    #include "common/definitions.h"
    #include "abrarov_quine_2011/definitions.h"
    #include "abrarov_quine_2011/coefficients.h"
#endif /* !defined( CERRF_NO_INCLUDES ) */

/* ************************************************************************* */

#if defined( __cplusplus ) && !defined( _GPUCODE )
extern "C" {
#endif /* defined( __cplusplus ) */

CERRF_STATIC CERRF_FN void cerf_abrarov_quine(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_FOURIER_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT fourier_coeff,
    CERRF_TAYLOR_COEFF_DEC CERRF_REAL_TYPE  const* CERRF_RESTRICT taylor_coeff
) CERRF_NOEXCEPT;

CERRF_STATIC CERRF_FN void cerfc_abrarov_quine(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_FOURIER_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT fourier_coeff,
    CERRF_TAYLOR_COEFF_DEC CERRF_REAL_TYPE const*  CERRF_RESTRICT taylor_coeff
) CERRF_NOEXCEPT;

CERRF_STATIC CERRF_FN int wofz_abrarov_quine(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_FOURIER_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT fourier_coeff,
    CERRF_TAYLOR_COEFF_DEC CERRF_REAL_TYPE const*  CERRF_RESTRICT taylor_coeff
) CERRF_NOEXCEPT;

CERRF_STATIC CERRF_FN int wofz_abrarov_quine_default(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y ) CERRF_NOEXCEPT;

#if !defined( _GPUCODE )

CERRF_EXTERN CERRF_HOST_FN void cerf_abrarov_quine_ext(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y ) CERRF_NOEXCEPT;

CERRF_EXTERN CERRF_HOST_FN void cerfc_abrarov_quine_ext(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y ) CERRF_NOEXCEPT;

CERRF_EXTERN CERRF_HOST_FN int wofz_abrarov_quine_ext(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y ) CERRF_NOEXCEPT;

#endif /* !defined( _GPUCODE ) */

#if defined( __cplusplus ) && !defined( _GPUCODE )
}
#endif /* defined( __cplusplus ) */

#if defined( __cplusplus ) && !defined( _GPUCODE )
extern "C" {
#endif /* defined( __cplusplus ) */

CERRF_INLINE int wofz_abrarov_quine(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_FOURIER_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT fourier_coeff,
    CERRF_TAYLOR_COEFF_DEC CERRF_REAL_TYPE  const*  CERRF_RESTRICT taylor_coeff
) CERRF_NOEXCEPT
{
    typedef CERRF_REAL_TYPE real_type;

    #if defined( __cplusplus )
    using std::sqrt;
    using std::sin;
    using std::cos;
    using std::exp;
    using std::ceil;
    #endif

    real_type const sign_x = ( real_type )( in_x >= ( real_type )0.0 ) -
                             ( real_type )( in_x <  ( real_type )0.0 );

    real_type const sign_y = ( real_type )( in_y >= ( real_type )0.0 ) -
                             ( real_type )( in_y <  ( real_type )0.0 );

    real_type wz_re = ( real_type )0.0;
    real_type wz_im = ( real_type )0.0;

    real_type const y_squ = in_y * in_y;
    real_type const x_squ = in_x * in_x;
    real_type const x_squ_plus_y_squ = x_squ + y_squ;

    in_x *= sign_x;
    in_y *= sign_y;

    CERRF_ASSERT( out_x           != NULL );
    CERRF_ASSERT( out_y           != NULL );
    CERRF_ASSERT( fourier_coeff   != NULL );
    CERRF_ASSERT( taylor_coeff    != NULL );

    if( x_squ_plus_y_squ < ( real_type )CERRF_TM_SQU )
    {
        real_type const pi_over_two_tm = ( real_type )CERRF_PI /
            ( ( real_type )2.0 * ( real_type )CERRF_TM );

        real_type const x_div_pi_over_tm = ( sign_x * in_x ) / (
            ( real_type )2.0 * pi_over_two_tm );

        int const N_POLE = ( x_div_pi_over_tm > pi_over_two_tm )
            ? ( int )ceil( x_div_pi_over_tm - pi_over_two_tm ) : ( int )0;

        real_type temp = ( sign_x * in_x ) -
            ( real_type )N_POLE * ( real_type )2.0 * pi_over_two_tm;

        real_type const dist_to_pole_squ = temp * temp + y_squ;

        bool const needs_taylor_expansion_around_pole = (
            ( N_POLE < ( int )CERRF_N_FOURIER ) && ( N_POLE >= ( int )0 ) &&
            ( dist_to_pole_squ <= ( real_type )CERRF_MIN_POLE_DIST_SQU ) );

        real_type const y_tm = in_y * ( real_type )CERRF_TM;
        real_type const y_tm_squ = y_tm * y_tm;
        real_type const exp_minus_y_tm = exp( -y_tm );

        real_type const x_tm = in_x * ( real_type )CERRF_TM;

        #if defined( CERRF_USE_SINCOS )

        real_type cos_x_tm = ( real_type )0.0;
        real_type sin_x_tm = ( real_type )0.0;

        sincos( x_tm, &sin_x_tm, &cos_x_tm );

        #else /* !defined( CERRF_USE_SINCOS ) */

        real_type const cos_x_tm = cos( x_tm );
        real_type const sin_x_tm = sin( x_tm );

        #endif /* !defined( CERRF_USE_SINCOS ) */

        /* seed n_pi and cos_n_pi to have to proper values for nn = 0 */
        real_type n_pi     = -( real_type )CERRF_PI;
        real_type cos_n_pi = -( real_type )1.0;

        real_type an_times_tm = ( real_type )0.0;
        real_type denom = ( real_type )0.0;
        real_type const exp_my_tm_cos_x_tm = exp_minus_y_tm * cos_x_tm;
        real_type const exp_my_tm_sin_x_tm = exp_minus_y_tm * sin_x_tm;

        int nn = 0;

        CERRF_ASSERT( N_POLE >= ( int )0 );

        if( !needs_taylor_expansion_around_pole )
        {
            denom = -fourier_coeff[ 0 ] / x_squ_plus_y_squ;

            wz_re = denom * ( in_y - in_y * exp_my_tm_cos_x_tm +
                                    in_x * exp_my_tm_sin_x_tm );

            wz_im = denom * ( in_x - in_x * exp_my_tm_cos_x_tm -
                                    in_y * exp_my_tm_sin_x_tm );

            for( ; nn < ( int )CERRF_N_FOURIER ; ++nn )
            {
                n_pi        += ( real_type )CERRF_PI;
                cos_n_pi     = -cos_n_pi;
                an_times_tm  = ( real_type )CERRF_TM * fourier_coeff[ nn ];

                temp  = x_tm + n_pi;
                denom = an_times_tm / ( y_tm_squ + temp * temp );

                wz_re += denom * ( cos_n_pi * temp * exp_my_tm_sin_x_tm + y_tm -
                                   cos_n_pi * y_tm * exp_my_tm_cos_x_tm );

                wz_im += denom * ( temp - cos_n_pi * temp * exp_my_tm_cos_x_tm
                                        - cos_n_pi * y_tm * exp_my_tm_sin_x_tm );

                temp  = x_tm - n_pi;
                denom = an_times_tm / ( y_tm_squ + temp * temp );

                wz_re += denom * ( y_tm + cos_n_pi * temp * exp_my_tm_sin_x_tm -
                                          cos_n_pi * y_tm * exp_my_tm_cos_x_tm );

                wz_im += denom * ( temp - cos_n_pi * temp * exp_my_tm_cos_x_tm -
                                          cos_n_pi * y_tm * exp_my_tm_sin_x_tm );
            }

            wz_re *= ( real_type )CERRF_INV_TWO_SQRT_PI;
            wz_im *= ( real_type )CERRF_INV_TWO_SQRT_PI;
        }
        else
        {
            real_type const dz_im     = in_y;
            real_type const dz_im_squ = dz_im * dz_im;
            real_type const dz_im_3   = dz_im_squ * dz_im;

            real_type const dz_re = in_x - ( ( ( real_type )N_POLE * (
                real_type )CERRF_PI ) / ( real_type )CERRF_TM );

            real_type const dz_re_squ = dz_re * dz_re;
            real_type const dz_re_3   = dz_re_squ * dz_re;

            real_type const dz_re_squ_minus_dz_im_squ = (
                dz_re + dz_im ) * ( dz_re - dz_im );

            int jj = N_POLE * 2 * 6;

            /* const term */

            wz_re = taylor_coeff[ jj++ ];
            wz_im = taylor_coeff[ jj++ ];

            /* linear term */

            wz_re += taylor_coeff[ jj   ] * dz_re;
            wz_im += taylor_coeff[ jj++ ] * dz_im;

            wz_re -= taylor_coeff[ jj   ] * dz_im;
            wz_im += taylor_coeff[ jj++ ] * dz_re;

            /* quadratic term */

            wz_re += taylor_coeff[ jj   ] * dz_re_squ_minus_dz_im_squ;
            wz_im += taylor_coeff[ jj++ ] * ( real_type )2.0 * dz_re * dz_im;

            wz_re -= taylor_coeff[ jj   ] * ( real_type )2.0 * dz_re * dz_im;
            wz_im += taylor_coeff[ jj++ ] * dz_re_squ_minus_dz_im_squ;

            /* cubic term */

            wz_re += taylor_coeff[ jj   ] * ( dz_re_3 -
                ( real_type )3.0 * dz_im_squ * dz_re );

            wz_im -= taylor_coeff[ jj++ ] * ( dz_im_3 -
                ( real_type )3.0 * dz_re_squ * dz_im );

            wz_re += taylor_coeff[ jj   ] * ( dz_im_3 -
                ( real_type )3.0 * dz_re_squ * dz_im );

            wz_im += taylor_coeff[ jj++ ] * ( dz_re_3 -
                ( real_type )3.0 * dz_im_squ * dz_re );

            /* fourth order term */

            wz_re += taylor_coeff[ jj   ] * ( dz_im_squ * dz_im_squ -
                ( real_type )6.0 * dz_im_squ * dz_re_squ +
                dz_re_squ * dz_re_squ );

            wz_im += taylor_coeff[ jj++ ] * ( real_type )4.0 * (
                dz_re_3 * dz_im - dz_im_3 * dz_re );

            wz_re += taylor_coeff[ jj   ] * ( real_type )4.0 * (
                dz_im_3 * dz_re - dz_re_3 * dz_im );

            wz_im += taylor_coeff[ jj++ ] * ( dz_im_squ * dz_im_squ -
                ( real_type )6.0 * dz_im_squ * dz_re_squ +
                dz_re_squ * dz_re_squ );

            /* fifth order term */

            wz_re += taylor_coeff[ jj   ] * ( dz_im_squ * dz_im_3 -
                ( real_type )10.0 * dz_im_squ * dz_re_3 +
                ( real_type )5.0  * dz_im_squ * dz_im_squ * dz_re );

            wz_im += taylor_coeff[ jj++ ] * ( dz_im_3 * dz_im_squ -
                ( real_type )10.0 * dz_im_3 * dz_re_squ +
                ( real_type )5.0  * dz_re_squ * dz_re_squ * dz_im );

            wz_re -= taylor_coeff[ jj ] * ( dz_im_3 * dz_im_squ -
                ( real_type )10.0 * dz_im_3 * dz_re_squ +
                ( real_type )5.0  * dz_re_squ * dz_re_squ * dz_im );

            wz_im += taylor_coeff[ jj ] * ( dz_re_3 * dz_re_squ -
                ( real_type )10.0 * dz_re_3 * dz_im_squ +
                ( real_type )5.0  * dz_im_squ * dz_im_squ * dz_re );
        }
    }
    else
    {
        real_type const zsqu_re  = ( in_x + in_y ) * ( in_x - in_y );
        real_type const zsqu_im  = ( real_type )2.0 * in_x * in_y;

        real_type cont_frac_re   = ( real_type )1.0;
        real_type cont_frac_im   = ( real_type )0.0;
        real_type cont_frac_norm = ( real_type )1.0;

        unsigned k = 9u;

        for( ; k ; --k )
        {
            cont_frac_re = +( ( real_type )k / ( real_type )2. ) *
                cont_frac_re / cont_frac_norm;

            cont_frac_im = -( ( real_type )k / ( real_type )2. ) *
                cont_frac_im / cont_frac_norm;

            cont_frac_re -= ( k & 1 ) ? zsqu_re : -( real_type )1.0;
            cont_frac_im -= ( k & 1 ) ? zsqu_im :  ( real_type )0.0;
            cont_frac_norm = cont_frac_re * cont_frac_re +
                             cont_frac_im * cont_frac_im;
        }

        cont_frac_norm = ( real_type )1.0 / (
            cont_frac_norm * ( real_type )CERRF_SQRT_PI );

        wz_re =  ( in_y * cont_frac_re - in_x * cont_frac_im ) * cont_frac_norm;
        wz_im = -( in_x * cont_frac_re + in_y * cont_frac_im ) * cont_frac_norm;
    }

    *out_x = wz_re;
    *out_y = wz_im;

    return 0;
}

CERRF_INLINE int wofz_abrarov_quine_default(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y ) CERRF_NOEXCEPT
{
    return wofz_abrarov_quine( in_x, in_y, out_x, out_y,
        &CERRF_FOURIER_COEFF[ 0 ], &CERRF_ALT_TAYLOR_COEFF[ 0 ] );
}

CERRF_INLINE void cerf_abrarov_quine(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_FOURIER_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT fourier_coeff,
    CERRF_TAYLOR_COEFF_DEC CERRF_REAL_TYPE const*  CERRF_RESTRICT taylor_coeff
) CERRF_NOEXCEPT
{
    typedef CERRF_REAL_TYPE real_type;

    #if defined( __cplusplus )
    using std::exp;
    using std::cos;
    using std::sin;
    #endif

    real_type const c_exp = exp( in_y * in_y - in_x * in_x );
    real_type const c_phase = -( real_type )2.0 * in_x + in_y;

    real_type const z_re = c_exp * cos( c_phase );
    real_type const z_im = c_exp * sin( c_phase );

    CERRF_RESULT_DEC real_type temp_re = ( real_type )0.0;
    CERRF_RESULT_DEC real_type temp_im = ( real_type )0.0;

    CERRF_ASSERT( out_x != NULL );
    CERRF_ASSERT( out_y != NULL );

    if( in_x >= ( real_type )0.0 )
    {
        wofz_abrarov_quine( -in_y, in_x, &temp_re, &temp_im,
            fourier_coeff, taylor_coeff );

        *out_x =  ( real_type )1.0 - z_re * temp_re + z_im * temp_im;
        *out_y = -( z_re * temp_im + z_im * temp_re );
    }
    else
    {
        wofz_abrarov_quine( in_y, -in_x, &temp_re, &temp_im,
            fourier_coeff, taylor_coeff );

        *out_x = ( real_type )1.0 + z_re * temp_re - z_im * temp_im;
        *out_y = z_re * temp_im + z_im * temp_re;
    }
}

CERRF_INLINE void cerfc_abrarov_quine(
    CERRF_REAL_TYPE in_x, CERRF_REAL_TYPE in_y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_FOURIER_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT fourier_coeff,
    CERRF_TAYLOR_COEFF_DEC CERRF_REAL_TYPE const*  CERRF_RESTRICT taylor_coeff
) CERRF_NOEXCEPT
{
    typedef CERRF_REAL_TYPE real_type;

    #if defined( __cplusplus )
    using std::exp;
    using std::cos;
    using std::sin;
    #endif

    real_type const c_exp = exp( in_y * in_y - in_x * in_x );
    real_type const c_phase = -( real_type )2.0 * in_x + in_y;

    real_type const z_re = c_exp * cos( c_phase );
    real_type const z_im = c_exp * sin( c_phase );

    CERRF_RESULT_DEC real_type temp_re = ( real_type )0.0;
    CERRF_RESULT_DEC real_type temp_im = ( real_type )0.0;

    CERRF_ASSERT( out_x != NULL );
    CERRF_ASSERT( out_y != NULL );

    if( in_x >= ( real_type )0.0 )
    {
        wofz_abrarov_quine( -in_y, in_x, &temp_re, &temp_im,
            fourier_coeff, taylor_coeff );

        *out_x = z_re * temp_re - z_im * temp_im;
        *out_y = z_re * temp_im + z_im * temp_re;
    }
    else
    {
        wofz_abrarov_quine( in_y, -in_x, &temp_re, &temp_im,
            fourier_coeff, taylor_coeff );

        *out_x =  ( real_type )2.0 - z_re * temp_re + z_im * temp_im;
        *out_y = -( z_re * temp_im + z_im * temp_re );
    }
}


#if defined( __cplusplus ) && !defined( _GPUCODE )
}
#endif /* defined( __cplusplus ) */
#endif /* FADDEEVAS_ABRAROV_QUINE_2011_H__ */
