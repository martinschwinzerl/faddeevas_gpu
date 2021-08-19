#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dawson_abq2018/definitions.h"
#include "dawson_abq2018/coefficients.h"
#include "dawson_abq2018/dawson.h"

int main()
{
    double x = ( double )2.0;
    double y = ( double )1e-5;

    double out_x = ( double )0.0;
    double out_y = ( double )0.0;

    Dawson_abq2018( x, y, &out_x, &out_y );
    printf( "in: Dawson_abq2018( %+e + i * %+e ) = %+30.18e %+30.18e * i\r\n",
               x, y, out_x, out_y );

    Dawson_abq2018_coeff( x, y, &out_x, &out_y,
                        &CERRF_DAWSON_ALPHA_N[ 0 ],
                        &CERRF_DAWSON_BETA_N[ 0 ],
                        &CERRF_DAWSON_GAMMA_N[ 0 ] );
    printf( "in: dawson_cerrf_coeff( %+e + i * %+e ) = %+30.18e %+30.18e * i\r\n",
               x, y, out_x, out_y );

	fflush( stdout );
	return 0;
}

