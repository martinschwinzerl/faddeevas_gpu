#ifndef CERRF_FADDEEVAS_DAWSON_APPROX_H__
#define CERRF_FADDEEVAS_DAWSON_APPROX_H__

#include "common/definitions.h"

#if !defined( CERRF_INTEGER_TYPE )
    #define CERRF_INTEGER_TYPE int
#endif /* !defined( CERRF_INTEGER_TYPE ) */

#if !defined( CERRF_DAWSON_XN_MIN )
    #define CERRF_DAWSON_XN_MIN 0.0
#endif /* !defined( CERRF_DAWSON_XN_MIN ) */

#if !defined( CERRF_DAWSON_DX )
    #define CERRF_DAWSON_DX 0.21052631578947367
#endif /* !defined( CERRF_DAWSON_DX ) */

#if !defined( CERRF_DAWSON_N_XN )
    #define CERRF_DAWSON_N_XN 96
#endif /* !defined( CERRF_DAWSON_N_XN ) */

/* CERRF_DAWSON_X_MAX = CERRF_DAWSON_XN_MIN */
#if !defined( CERRF_DAWSON_X_MIN )
    #define CERRF_DAWSON_X_MIN 0.0
#endif /* !defined( CERRF_DAWSON_X_MIN ) */

/* CERRF_DAWSON_X_MAX = CERRF_DAWSON_XN_MIN + (
 *       CERRF_DAWSON_N_XN - 1 ) * CERRF_DAWSON_DX */
#if !defined( CERRF_DAWSON_X_MAX )
    #define CERRF_DAWSON_X_MAX 20.21052631578947
#endif /* !defined( CERRF_DAWSON_X_MAX ) */

#if !defined( CERRF_DAWSON_Y_MAX )
    #define CERRF_DAWSON_Y_MAX 0.5
#endif /* !defined( CERRF_DAWSON_Y_MAX ) */

#if !defined( CERRF_DAWSON_MAX_N_TAYLOR )
    #if !defined( CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 ) || \
                ( CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 != 1 )
        #define CERRF_DAWSON_MAX_N_TAYLOR 25
    #else /* ( CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 == 1 ) */
        #define CERRF_DAWSON_MAX_N_TAYLOR 19
    #endif /* CERRF_DAWSON_TARGET_ACCURACY_ABS_D10*/
#endif /* CERRF_DAWSON_MAX_N_TAYLOR */

/* CERRF_DAWSON_MAX_N_TAYLOR * CERRF_DAWSON_N_XN */
#if !defined( CERRF_DAWSON_NUM_TAYLOR_COEFF )
    #if !defined( CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 ) || \
                ( CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 != 1 )
        #define CERRF_DAWSON_NUM_TAYLOR_COEFF 2400
    #else /* ( CERRF_DAWSON_TARGET_ACCURACY_ABS_D10 == 1 ) */
        #define CERRF_DAWSON_NUM_TAYLOR_COEFF 1824
    #endif /* CERRF_DAWSON_TARGET_ACCURACY_ABS_D10*/
#endif /* !defined( CERRF_DAWSON_NUM_TAYLOR_COEFF ) */

#if !defined( CERRF_DAWSON_TAYLOR_COEFF_DEC )
    #if defined( _GPUCODE )
        #if defined( __OPENCL_C_VERSION__ )
            #define CERRF_DAWSON_TAYLOR_COEFF_DEC __global
        #else
            #define CERRF_DAWSON_TAYLOR_COEFF_DEC
        #endif
    #else
        #define CERRF_DAWSON_TAYLOR_COEFF_DEC
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_DAWSON_TAYLOR_COEFF_DEC ) */

#if !defined( CERRF_DAWSON_N_TAYLOR_DEC )
    #if defined( _GPUCODE )
        #if defined( __OPENCL_C_VERSION__ )
            #define CERRF_DAWSON_N_TAYLOR_DEC __global
        #else
            #define CERRF_DAWSON_N_TAYLOR_DEC
        #endif
    #else
        #define CERRF_DAWSON_N_TAYLOR_DEC
    #endif /* !defined( _GPUCODE ) */
#endif /* !defined( CERRF_DAWSON_N_TAYLOR_DEC ) */

#endif /* CERRF_FADDEEVAS_DAWSON_APPROX_H__ */
