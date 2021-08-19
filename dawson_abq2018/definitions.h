#ifndef CERRF_FADDEEVAS_DAWSON_APPROX_H__
#define CERRF_FADDEEVAS_DAWSON_APPROX_H__

#include "common/definitions.h"

#if !defined( CERRF_INTEGER_TYPE )
    #define CERRF_INTEGER_TYPE int
#endif /* !defined( CERRF_INTEGER_TYPE ) */

#if !defined( CERRF_DAWSON_N_FOURIER )
    #define CERRF_DAWSON_N_FOURIER 24
#endif /* CERRF_DAWSON_N_FOURIER */

#if !defined( CERRF_DAWSON_SIGMA )
    #define CERRF_DAWSON_SIGMA 1.5
#endif /* !defined( CERRF_DAWSON_SIGMA ) */

#if !defined( CERRF_DAWSON_TAU_M )
    #define CERRF_DAWSON_TAU_M 12.0
#endif /* !defined( CERRF_DAWSON_TAU_M ) */

#if !defined( CERRF_DAWSON_SIGMA_SQU )
    #define CERRF_DAWSON_SIGMA_SQU 2.25
#endif /* !defined( CERRF_DAWSON_SIGMA_SQU ) */

#if !defined( CERRF_DAWSON_FOUR_SIGMA_SQU )
    #define CERRF_DAWSON_FOUR_SIGMA_SQU 9.0
#endif /* !defined( CERRF_DAWSON_FOUR_SIGMA_SQU ) */

#if !defined( CERRF_DAWSON_EXP_SIGMA_SQU_OVER_TAU_M )
    #define CERRF_DAWSON_EXP_SIGMA_SQU_OVER_TAU_M 0.790644653029877143379197420375978201980852080638532250841954068
#endif /* !defined( CERRF_DAWSON_EXP_SIGMA_SQU_OVER_TAU_M ) */

#if !defined( CERRF_DAWSON_COEFF_DEC )
    #if defined( _GPUCODE )
        #if defined( __OPENCL_C_VERSION__ )
            #define CERRF_DAWSON_COEFF_DEC __global
        #else
            #define CERRF_DAWSON_COEFF_DEC
        #endif
    #else
        #define CERRF_DAWSON_COEFF_DEC
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_DAWSON_COEFF_DEC ) */

#endif /* CERRF_FADDEEVAS_DAWSON_APPROX_H__ */
