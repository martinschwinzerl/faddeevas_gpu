#include "cerrf_impl.h"

__kernel void eval_cerrf(
    __global double const* restrict in_x,
    __global double const* restrict in_y,
    __global double* restrict out_x,
    __global double* restrict out_y, long int const nn )
{
    long int const idx = get_global_id( 0 );

    if( idx < nn )
    {
        double result_x = ( double )0.0;
        double result_y = ( double )0.0;

        cerrf_base_impl( in_x[ idx ], in_y[ idx ], &result_x, &result_y );

        out_x[ idx ] = result_x;
        out_y[ idx ] = result_y;
    }
}
