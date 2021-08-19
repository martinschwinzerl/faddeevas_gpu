#ifndef CERRF_DAWSON_ABQ2018_COEFFICIENTS_H__
#define CERRF_DAWSON_ABQ2018_COEFFICIENTS_H__

#include "dawson_abq2018/definitions.h"

#if !defined( _GPUCODE ) && defined( __cplusplus )
extern "C" {
#endif /* !defined( _GPUCODE ) && defined( __cplusplus ) */

CERRF_EXTERN CERRF_REAL_TYPE const CERRF_DAWSON_ALPHA_N[ CERRF_DAWSON_N_FOURIER ];
CERRF_EXTERN CERRF_REAL_TYPE const CERRF_DAWSON_BETA_N[ CERRF_DAWSON_N_FOURIER ];
CERRF_EXTERN CERRF_REAL_TYPE const CERRF_DAWSON_GAMMA_N[ CERRF_DAWSON_N_FOURIER ];

#if !defined( _GPUCODE ) && defined( __cplusplus )
}
#endif /* !defined( _GPUCODE ) && defined( __cplusplus ) */
#endif /* CERRF_DAWSON_ABQ2018_COEFFICIENTS_H__ */
