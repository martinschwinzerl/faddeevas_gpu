#ifndef CERRF_ABRAROV_QUINE_2011_DEFINITIONS_REV_3_H__
#define CERRF_ABRAROV_QUINE_2011_DEFINITIONS_REV_3_H__

#if !defined( CERRF_NO_INCLUDES )
    #include "common/definitions.h"
#endif /* !defined( CERRF_NO_INCLUDES ) */

#if !defined( CERRF_TAYLOR_COEFF_DEC )
    #if defined( _GPUCODE )
        #if defined( __OPENCL_C_VERSION__ )
            #define CERRF_TAYLOR_COEFF_DEC __global
        #else
            #define CERRF_TAYLOR_COEFF_DEC
        #endif
    #else
        #define CERRF_TAYLOR_COEFF_DEC
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_TAYLOR_COEFF_DEC ) */

#if !defined( CERRF_FOURIER_COEFF_DEC )
    #if defined( _GPUCODE )
        #if defined( __OPENCL_C_VERSION__ )
            #define CERRF_FOURIER_COEFF_DEC __global
        #else
            #define CERRF_FOURIER_COEFF_DEC
        #endif
    #else
        #define CERRF_FOURIER_COEFF_DEC
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_FOURIER_COEFF_DEC ) */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#if !defined( CERRF_N_FOURIER )
    #define CERRF_N_FOURIER 24
#endif /* !defined( CERRF_N_FOURIER ) */

#if !defined( CERRF_N_TAYLOR )
    #define CERRF_N_TAYLOR 6
#endif /* !defined( CERRF_N_TAYLOR ) */

#if !defined( CERRF_MIN_POLE_DIST )
    #define CERRF_MIN_POLE_DIST 3e-3
#endif /* !defined( CERRF_MIN_POLE_DIST ) */

#if !defined( CERRF_MIN_POLE_DIST_SQU )
    #define CERRF_MIN_POLE_DIST_SQU 9e-6
#endif /* !defined( CERRF_MIN_POLE_DIST_SQU ) */

/* CERRF_NUM_TAYLOR_COEFF = 2 * ( 1 + CERRF_N_FOURIER ) * CERRF_N_TAYLOR */

#if !defined( CERRF_NUM_TAYLOR_COEFF )
    #define CERRF_NUM_TAYLOR_COEFF 288
#endif /* !defined( CERRF_NUM_TAYLOR_COEFF ) */

#if !defined( CERRF_TM )
    #define CERRF_TM 12.0
#endif /* !defined( CERRF_TM ) */

#if !defined( CERRF_TM_SQU )
    #define CERRF_TM_SQU 144.0
#endif /* !defined( CERRF_TM_SQU ) */

#if !defined( CERRF_PI_OVER_TM )
    #define CERRF_PI_OVER_TM 0.261799387799149436538553615273291907016430783281258818414578716
#endif /* !defined( CERRF_PI_OVER_TM ) */

#if !defined( CERRF_TM_OVER_PI )
    #define CERRF_TM_OVER_PI 3.819718634205488058453210320940344688827031497770954769944016257
#endif /* !defined( CERRF_TM_OVER_PI ) */

/* tm * tm / sqrt( pi ) */
#if !defined( CERRF_TM_SQU_OVER_SQRT_PI )
    #define CERRF_TM_SQU_OVER_SQRT_PI 81.24330003087690532052344102475125236154329062337583538554834392607
#endif /* !defined( CERRF_TM_SQU_OVER_SQRT_PI ) */

#if !defined( CERRF_CONTINUOUS_FRACTION_LIMIT_SQU )
    #define CERRF_CONTINUOUS_FRACTION_LIMIT_SQU 225.0
#endif /* CERRF_CONTINUOUS_FRACTION_LIMIT_SQU */

#if !defined( CERRF_CONTINUOUS_FRACTION_K )
    #define CERRF_CONTINUOUS_FRACTION_K 9
#endif /* CERRF_CONTINUOUS_FRACTION_K */

#if !defined( CERRF_USE_DAWSON_APPROX_MAX_Y )
    #define CERRF_USE_DAWSON_APPROX_MAX_Y 0.5
#endif /* !defined( CERRF_USE_DAWSON_APPROX_MAX_Y ) */

#if !defined( CERRF_USE_DAWSON_APPROX_MIN_X )
    #define CERRF_USE_DAWSON_APPROX_MIN_X 0.0
#endif /* !defined( CERRF_USE_DAWSON_APPROX_MIN_X ) */

#if !defined( CERRF_USE_DAWSON_APPROX_MAX_X )
    #define CERRF_USE_DAWSON_APPROX_MAX_X 15.01
#endif /* !defined( CERRF_USE_DAWSON_APPROX_MAX_X ) */

#endif /* CERRF_ABRAROV_QUINE_2011_DEFINITIONS_REV_3_H__ */
