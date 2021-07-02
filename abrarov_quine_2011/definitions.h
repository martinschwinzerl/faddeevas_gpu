#ifndef CERRF_ABRAROV_QUINE_2011_DEFINITIONS_H__
#define CERRF_ABRAROV_QUINE_2011_DEFINITIONS_H__

#if !defined( CERRF_NO_INCLUDES )
    #include "common/definitions.h"
#endif /* !defined( CERRF_NO_INCLUDES ) */

#if !defined( CERRF_TAYLOR_COEFF_DEC )
    #if defined( _GPUCODE )
        #if defined( __OPENCL_C_VERSION__ )
            #define CERRF_TAYLOR_COEFF_DEC __constant
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
            #define CERRF_FOURIER_COEFF_DEC __constant
        #else
            #define CERRF_FOURIER_COEFF_DEC
        #endif
    #else
        #define CERRF_FOURIER_COEFF_DEC
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_FOURIER_COEFF_DEC ) */


#if !defined( CERRF_RESULT_DEC )
    #if defined( _GPUCODE )
        #if defined( __OPENCL_C_VERSION__ )
            #define CERRF_RESULT_DEC __global
        #else
            #define CERRF_RESULT_DEC
        #endif
    #else
        #define CERRF_RESULT_DEC
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_RESULT_DEC ) */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if !defined( CERRF_INV_TWO_SQRT_PI )
    #define CERRF_INV_TWO_SQRT_PI 0.28209479177387814347403972578
#endif /* !defined( CERRF_INV_TWO_SQRT_PI ) */

#if !defined( CERRF_SQRT_TWO )
    #define CERRF_SQRT_TWO 1.4142135623730950488
#endif /* !defined( CERRF_SQRT_TWO ) */

#if !defined( CERRF_N_FOURIER )
    #define CERRF_N_FOURIER 24
#endif /* !defined( CERRF_N_FOURIER ) */

#if !defined( CERRF_N_TAYLOR )
    #define CERRF_N_TAYLOR 18
#endif /* !defined( CERRF_N_TAYLOR ) */

/* CERRF_NUM_A0_TAYLOR_COEFF = 2 * CERRF_N_TAYLOR */
#if !defined( CERRF_NUM_A0_TAYLOR_COEFF )
    #define CERRF_NUM_A0_TAYLOR_COEFF 21
#endif /* !defined( CERRF_NUM_A0_TAYLOR_COEFF ) */

/* CERRF_NUM_TAYLOR_COEFF = CERRF_N_FOURIER * 2 * CERRF_N_TAYLOR */
#if !defined( CERRF_NUM_TAYLOR_COEFF )
    #define CERRF_NUM_TAYLOR_COEFF 864
#endif /* !defined( CERRF_NUM_TAYLOR_COEFF ) */

#if !defined( CERRF_TM )
    #define CERRF_TM 12.0
#endif /* !defined( CERRF_TM ) */

#if !defined( CERRF_TM_SQU )
    #define CERRF_TM_SQU 144.0
#endif /* !defined( CERRF_TM_SQU ) */

#if !defined( CERRF_MIN_POLE_DIST_SQU )
    #define CERRF_MIN_POLE_DIST_SQU 9e-6
#endif /* !defined( CERRF_MIN_POLE_DIST_SQU ) */

#endif /* CERRF_ABRAROV_QUINE_2011_DEFINITIONS_H__ */
