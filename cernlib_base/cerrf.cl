__kernel void eval_cerrf(
    __global double const* restrict in_x,
    __global double const* restrict in_y,
    __global double* restrict out_x,
    __global double* restrict out_y, long int const nn )
{
    long int const idx = get_global_id( 0 );

    if( idx < nn )
    {
        double const in_real = in_x[ idx ];
        double const in_imag = in_y[ idx ];
        double const cfactor = ( double )1.12837916709551;
        double const xLim = 5.33; double const yLim = 4.29;
        double h, q, Saux, Sx, Sy, Tn, Tx, Ty, Wx, Wy, xh, xl, x, yh, y;
        double Rx [33]; double Ry [33];
        int n, nc, nu;

        x = fabs(in_real); y = fabs(in_imag);

        if (y < yLim && x < xLim){
            q = (1.0 - y / yLim) * sqrt(1.0 - (x / xLim) * (x / xLim));
            h  = 1.0 / (3.2 * q);
            nc = 7 + (int) (23.0 * q);
            xl = pow(h, (double) (1 - nc));
            xh = y + 0.5 / h;
            yh = x;
            nu = 10 + (int) (21.0 * q);
            Rx[nu] = 0.;
            Ry[nu] = 0.;
            for (n = nu; n > 0; n--){
                Tx = xh + n * Rx[n];
                Ty = yh - n * Ry[n];
                Tn = Tx*Tx + Ty*Ty;
                Rx[n-1] = 0.5 * Tx / Tn;
                Ry[n-1] = 0.5 * Ty / Tn;
                }
            Sx = 0.;
            Sy = 0.;
            for (n = nc; n>0; n--){
                Saux = Sx + xl;
                Sx = Rx[n-1] * Saux - Ry[n-1] * Sy;
                Sy = Rx[n-1] * Sy + Ry[n-1] * Saux;
                xl = h * xl;
            };
            Wx = cfactor * Sx;
            Wy = cfactor * Sy;
        }
        else{
            xh = y;
            yh = x;
            Rx[0] = 0.;
            Ry[0] = 0.;
            for (n = 9; n>0; n--){
                Tx = xh + n * Rx[0];
                Ty = yh - n * Ry[0];
                Tn = Tx * Tx + Ty * Ty;
                Rx[0] = 0.5 * Tx / Tn;
                Ry[0] = 0.5 * Ty / Tn;
            };
            Wx = cfactor * Rx[0];
            Wy = cfactor * Ry[0];
        }
        if (y == 0.) {Wx = exp(-x * x);}
        if (in_imag < 0.){
            Wx =   2.0 * exp(y * y - x * x) * cos(2.0 * x * y) - Wx;
            Wy = - 2.0 * exp(y * y - x * x) * sin(2.0 * x * y) - Wy;
            if (in_real > 0.) {Wy = -Wy;}
        }
        else if (in_real < 0.) {Wy = -Wy;}

        out_x[ idx ] = Wx;
        out_y[ idx ] = Wy;
    }
}
