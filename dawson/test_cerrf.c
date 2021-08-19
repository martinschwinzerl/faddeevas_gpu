#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dawson/definitions.h"
#include "dawson/coefficients.h"
#include "dawson/dawson.h"

int main()
{
    double x = ( double )6.3;
    double y = ( double )1e-8;

    double out_x = ( double )0.0;
    double out_y = ( double )0.0;

    dawson_cerrf( x, y, &out_x, &out_y );
    printf( "in: dawson_cerrf( %+f + i * %+f ) = %+30.18f %+30.18f * i\r\n",
               x, y, out_x, out_y );

    dawson_cerrf_coeff( x, y, &out_x, &out_y,
                        &CERRF_DAWSON_N_TAYLOR[ 0 ],
                        &CERRF_DAWSON_TAYLOR_COEFF[ 0 ] );
    printf( "in: dawson_cerrf_coeff( %+f + i * %+f ) = %+30.18f %+30.18f * i\r\n",
               x, y, out_x, out_y );

	fflush( stdout );
	return 0;
}

