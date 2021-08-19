#ifndef CERRF_DAWSON_TAYLOR_COEFFICIENTS_H__
#define CERRF_DAWSON_TAYLOR_COEFFICIENTS_H__

#include "dawson/definitions.h"

#if !defined( _GPUCODE ) && defined( __cplusplus )
extern "C" {
#endif /* !defined( _GPUCODE ) && defined( __cplusplus ) */

CERRF_EXTERN CERRF_REAL_TYPE const CERRF_DAWSON_XN[ CERRF_DAWSON_N_XN ];
CERRF_EXTERN CERRF_REAL_TYPE const CERRF_DAWSON_FZ_XN_0[ CERRF_DAWSON_N_XN ];
CERRF_EXTERN CERRF_INTEGER_TYPE const CERRF_DAWSON_N_TAYLOR[ CERRF_DAWSON_N_XN ];

CERRF_EXTERN CERRF_REAL_TYPE const CERRF_DAWSON_TAYLOR_COEFF[
    CERRF_DAWSON_NUM_TAYLOR_COEFF ];

#if !defined( _GPUCODE ) && defined( __cplusplus )
}
#endif /* !defined( _GPUCODE ) && defined( __cplusplus ) */

#endif /* CERRF_DAWSON_TAYLOR_COEFFICIENTS_H__ */
