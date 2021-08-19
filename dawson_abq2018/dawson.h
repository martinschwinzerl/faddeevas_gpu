#ifndef CERRF_DAWSON_H__
#define CERRF_DAWSON_H__

#include "common/definitions.h"
#include "dawson_abq2018/definitions.h"

CERRF_STATIC CERRF_FN CERRF_REAL_TYPE Dawson_abq2018_alpha_n( int const n ) CERRF_NOEXCEPT;
CERRF_STATIC CERRF_FN CERRF_REAL_TYPE Dawson_abq2018_beta_n( int const n ) CERRF_NOEXCEPT;
CERRF_STATIC CERRF_FN CERRF_REAL_TYPE Dawson_abq2018_gamma_n( int const n ) CERRF_NOEXCEPT;

CERRF_STATIC CERRF_FN void Dawson_abq2018(
    CERRF_REAL_TYPE const x, CERRF_REAL_TYPE const y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y  ) CERRF_NOEXCEPT;

CERRF_STATIC CERRF_FN void Dawson_abq2018_coeff(
    CERRF_REAL_TYPE const x, CERRF_REAL_TYPE const y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_DAWSON_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT alpha_n,
    CERRF_DAWSON_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT beta_n,
    CERRF_DAWSON_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT gamma_n
) CERRF_NOEXCEPT;

/* ************************************************************************* */

CERRF_INLINE void Dawson_abq2018(
    CERRF_REAL_TYPE const x, CERRF_REAL_TYPE const y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y  ) CERRF_NOEXCEPT
{
    typedef CERRF_REAL_TYPE real_type;

    #if defined( __cplusplus )
    using std::cos;
    using std::sin;
    using std::exp;
    #endif /* defined( __cplusplus ) */

    int kk = 0;

    real_type temp              = ( y - x ) * ( y + x );
    real_type const zeta_re     = ( real_type )CERRF_DAWSON_SIGMA_SQU - temp;
    real_type const zeta_im     = ( real_type )2.0 * x * y;
    real_type const zeta_im_squ = zeta_im * zeta_im;
    real_type theta_re          =  zeta_re;
    real_type theta_im          = -zeta_im;
    real_type Wx                =  cos( zeta_im );
    real_type Wy                = -sin( zeta_im );

    temp = exp( temp );
    Wx *= temp;
    Wy *= temp;

    temp = ( zeta_re * zeta_re ) + ( zeta_im * zeta_im );
    temp = ( real_type )1.0 / temp;

    theta_re *= temp;
    theta_im *= temp;

    for( ; kk < ( int )CERRF_DAWSON_N_FOURIER ; ++kk )
    {
        real_type const gamma_kk = Dawson_abq2018_gamma_n( kk );
        real_type const d_zeta_gamma_re = zeta_re - gamma_kk;

        real_type const beta_kk  = Dawson_abq2018_beta_n( kk );
        real_type const nom_im   = beta_kk * zeta_im;
        real_type const nom_re   =
            Dawson_abq2018_alpha_n( kk ) + ( beta_kk * d_zeta_gamma_re );

        real_type const denom_im = ( real_type )2.0 * d_zeta_gamma_re * zeta_im;
        real_type const denom_re = ( d_zeta_gamma_re * d_zeta_gamma_re )
            + ( real_type )CERRF_DAWSON_FOUR_SIGMA_SQU * gamma_kk - zeta_im_squ;

        real_type d_theta_re = ( nom_re   * denom_re ) + ( nom_im   * denom_im );
        real_type d_theta_im = ( nom_im   * denom_re ) - ( nom_re   * denom_im );
        temp                 = ( denom_re * denom_re ) + ( denom_im * denom_im );
        temp                 = ( real_type )1.0 / temp;

        d_theta_re *= temp;
        d_theta_im *= temp;

        theta_re   += d_theta_re;
        theta_im   += d_theta_im;
    }

    theta_re *= ( real_type )CERRF_DAWSON_EXP_SIGMA_SQU_OVER_TAU_M;
    theta_im *= ( real_type )CERRF_DAWSON_EXP_SIGMA_SQU_OVER_TAU_M;

    Wx -= y * theta_re + x * theta_im;
    Wy += x * theta_re - y * theta_im;

    *out_x = Wx;
    *out_y = Wy;
}

CERRF_STATIC CERRF_FN void Dawson_abq2018_coeff(
    CERRF_REAL_TYPE const x, CERRF_REAL_TYPE const y,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_x,
    CERRF_RESULT_DEC CERRF_REAL_TYPE* CERRF_RESTRICT out_y,
    CERRF_DAWSON_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT alpha_n,
    CERRF_DAWSON_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT beta_n,
    CERRF_DAWSON_COEFF_DEC CERRF_REAL_TYPE const* CERRF_RESTRICT gamma_n
) CERRF_NOEXCEPT
{
    typedef CERRF_REAL_TYPE real_type;

    #if defined( __cplusplus )
    using std::cos;
    using std::sin;
    using std::exp;
    #endif /* defined( __cplusplus ) */

    int kk = 0;

    real_type temp              = ( y - x ) * ( y + x );
    real_type const zeta_re     = ( real_type )CERRF_DAWSON_SIGMA_SQU - temp;
    real_type const zeta_im     = ( real_type )2.0 * x * y;
    real_type const zeta_im_squ = zeta_im * zeta_im;
    real_type theta_re          =  zeta_re;
    real_type theta_im          = -zeta_im;
    real_type Wx                =  cos( zeta_im );
    real_type Wy                = -sin( zeta_im );

    temp = exp( temp );
    Wx *= temp;
    Wy *= temp;

    temp = ( zeta_re * zeta_re ) + ( zeta_im * zeta_im );
    temp = ( real_type )1.0 / temp;

    theta_re *= temp;
    theta_im *= temp;

    for( ; kk < ( int )CERRF_DAWSON_N_FOURIER ; ++kk )
    {
        real_type const gamma_kk = gamma_n[ kk ];
        real_type const d_zeta_gamma_re = zeta_re - gamma_kk;

        real_type const beta_kk  = beta_n[ kk ];
        real_type const nom_im   = beta_kk * zeta_im;
        real_type const nom_re   = alpha_n[ kk ] + ( beta_kk * d_zeta_gamma_re );

        real_type const denom_im = ( real_type )2.0 * d_zeta_gamma_re * zeta_im;
        real_type const denom_re = ( d_zeta_gamma_re * d_zeta_gamma_re )
            + ( real_type )CERRF_DAWSON_FOUR_SIGMA_SQU * gamma_kk - zeta_im_squ;

        real_type d_theta_re = ( nom_re   * denom_re ) + ( nom_im   * denom_im );
        real_type d_theta_im = ( nom_im   * denom_re ) - ( nom_re   * denom_im );
        temp                 = ( denom_re * denom_re ) + ( denom_im * denom_im );
        temp                 = ( real_type )1.0 / temp;

        d_theta_re *= temp;
        d_theta_im *= temp;

        theta_re   += d_theta_re;
        theta_im   += d_theta_im;
    }

    theta_re *= ( real_type )CERRF_DAWSON_EXP_SIGMA_SQU_OVER_TAU_M;
    theta_im *= ( real_type )CERRF_DAWSON_EXP_SIGMA_SQU_OVER_TAU_M;

    Wx -= y * theta_re + x * theta_im;
    Wy += x * theta_re - y * theta_im;

    *out_x = Wx;
    *out_y = Wy;
}


CERRF_INLINE CERRF_REAL_TYPE Dawson_abq2018_alpha_n( int const n ) CERRF_NOEXCEPT {
    typedef CERRF_REAL_TYPE real_type;
    real_type alpha_n;
    switch( n ) {
        case  0: { alpha_n = ( real_type )+1.03714339207647376283082639253034415073287;               break; }
        case  1: { alpha_n = ( real_type )+2.38828183050571993585769485480969826853981;               break; }
        case  2: { alpha_n = ( real_type )+1.79817296254174254144210619483869224063789;               break; }
        case  3: { alpha_n = ( real_type )+5.17577096399262836922283445672098595036381e-65;           break; }
        case  4: { alpha_n = ( real_type )-1.00097451567547994645686826650908960245486;               break; }
        case  5: { alpha_n = ( real_type )-0.799268035509407090553349691677816492509182;              break; }
        case  6: { alpha_n = ( real_type )-0.27049875343390049214899342811775827240418;               break; }
        case  7: { alpha_n = ( real_type )-7.71371279180514720760225903441109487862459e-66;           break; }
        case  8: { alpha_n = ( real_type )+0.0387967506006028349088470158918305381432332;             break; }
        case  9: { alpha_n = ( real_type )+0.0165772476559632551534566759701091862012358;             break; }
        case 10: { alpha_n = ( real_type )+0.00305705004407479114271989750810132309259258;            break; }
        case 11: { alpha_n = ( real_type )-1.92499779101926618659986450390249040866655e-69;           break; }
        case 12: { alpha_n = ( real_type )-0.00013461133727483959229054732588505972227612;            break; }
        case 13: { alpha_n = ( real_type )-0.0000322179058494556939364795861557076687047462;          break; }
        case 14: { alpha_n = ( real_type )-0.0000033444859256964122549615147568123253131302;          break; }
        case 15: { alpha_n = ( real_type )-5.94616764240319618171621316050344791209275e-71;           break; }
        case 16: { alpha_n = ( real_type )+0.0000000471691734513512641046687875715242005220105;       break; }
        case 17: { alpha_n = ( real_type )+0.00000000641481653114344728321019118072408988520186;      break; }
        case 18: { alpha_n = ( real_type )+0.000000000379145216212020046115448087416531788360071;     break; }
        case 19: { alpha_n = ( real_type )+1.84527502333701536160615914241772158208524e-75;           break; }
        case 20: { alpha_n = ( real_type )-0.00000000000174174797155425672222708434630166293140238;   break; }
        case 21: { alpha_n = ( real_type )-0.000000000000135444848968957501184130053164707871658925;  break; }
        case 22: { alpha_n = ( real_type )-4.58225546143355145946331250175521038118693e-15;           break; }
        case 23: { alpha_n = ( real_type )+1.06548180295019836560097477300916529281009e-81;           break; }
        default: { alpha_n = ( real_type )0.0; }
    };
    return alpha_n;
}

CERRF_INLINE CERRF_REAL_TYPE Dawson_abq2018_beta_n( int const n ) CERRF_NOEXCEPT {
    typedef CERRF_REAL_TYPE real_type;
    real_type beta_n;
    switch( n ) {
        case  0: { beta_n = ( real_type )+1.3205319803525317889574584772544498265796;                break; }
        case  1: { beta_n = ( real_type )+1.87494302748481565376822149907899012801034e-65;           break; }
        case  2: { beta_n = ( real_type )-0.763168308060575673245854469874322649182891;              break; }
        case  3: { beta_n = ( real_type )-0.667994371972263570831605142031126176373404;              break; }
        case  4: { beta_n = ( real_type )-0.254896067326029610544127927252386925191548;              break; }
        case  5: { beta_n = ( real_type )+1.67442291904006733991769628821645818794233e-67;           break; }
        case  6: { beta_n = ( real_type )+0.0492013870962297838485917510514106158394991;             break; }
        case  7: { beta_n = ( real_type )+0.0248886434880101951244693084405325786926403;             break; }
        case  8: { beta_n = ( real_type )+0.00548861745243502255887999763048794580013454;            break; }
        case  9: { beta_n = ( real_type )+4.99727886017797457212733777717262735651038e-68;           break; }
        case 10: { beta_n = ( real_type )-0.000353850636940945099868381085592234343147835;           break; }
        case 11: { beta_n = ( real_type )-0.000103446372407624612290930180764787391157539;           break; }
        case 12: { beta_n = ( real_type )-0.0000131840367529237074968093182100479113017216;          break; }
        case 13: { beta_n = ( real_type )+8.0943522298744016924502214635554497889121e-71;            break; }
        case 14: { beta_n = ( real_type )+0.000000283888782493790605258175565801484372656832;        break; }
        case 15: { beta_n = ( real_type )+0.0000000479639476365189015486503942669827857021939;       break; }
        case 16: { beta_n = ( real_type )+0.00000000353280334886661211473371724039666248972125;      break; }
        case 17: { beta_n = ( real_type )+3.31254557798891482701937157635939244038159e-74;           break; }
        case 18: { beta_n = ( real_type )-0.0000000000254075096041214329698236318494801271270332;    break; }
        case 19: { beta_n = ( real_type )-0.00000000000248084819467357031695577950690451281878628;   break; }
        case 20: { beta_n = ( real_type )-0.000000000000105602971159292138934921550313132681307261;  break; }
        case 21: { beta_n = ( real_type )-1.70115485754614803750168846948639672406006e-79;           break; }
        case 22: { beta_n = ( real_type )+2.53665602503298603487195800812676285645367e-16;           break; }
        case 23: { beta_n = ( real_type )+1.43143316703720822727312109934620124923693e-17;           break; }
        default: { beta_n = ( real_type )0.0; }
    };
    return beta_n;
}

CERRF_INLINE CERRF_REAL_TYPE Dawson_abq2018_gamma_n( int const n ) CERRF_NOEXCEPT {
    typedef CERRF_REAL_TYPE real_type;
    real_type gamma_n;
    switch( n ) {
        case  0: { gamma_n = ( real_type )+0.0685389194520094348530172986102510495507896;             break; }
        case  1: { gamma_n = ( real_type )+0.274155677808037739412069194441004198203158;              break; }
        case  2: { gamma_n = ( real_type )+0.616850275068084913677155687492259445957106;              break; }
        case  3: { gamma_n = ( real_type )+1.09662271123215095764827677776401679281263;               break; }
        case  4: { gamma_n = ( real_type )+1.71347298630023587132543246525627623876974;               break; }
        case  5: { gamma_n = ( real_type )+2.46740110027233965470862274996903778382842;               break; }
        case  6: { gamma_n = ( real_type )+3.35840705314846230779784763190230142798869;               break; }
        case  7: { gamma_n = ( real_type )+4.38649084492860383059310711105606717125053;               break; }
        case  8: { gamma_n = ( real_type )+5.55165247561276422309440118743033501361396;               break; }
        case  9: { gamma_n = ( real_type )+6.85389194520094348530172986102510495507896;               break; }
        case 10: { gamma_n = ( real_type )+8.29320925369314161721509313184037699564554;               break; }
        case 11: { gamma_n = ( real_type )+9.8696044010893586188344909998761511353137;                break; }
        case 12: { gamma_n = ( real_type )+11.5830773873895944901599234651324273740834;               break; }
        case 13: { gamma_n = ( real_type )+13.4336282125938492311913905276092057119548;               break; }
        case 14: { gamma_n = ( real_type )+15.4212568767021228419288921873064861489277;               break; }
        case 15: { gamma_n = ( real_type )+17.5459633797144153223724284442242686850021;               break; }
        case 16: { gamma_n = ( real_type )+19.8077477216307266725219992983625533201782;               break; }
        case 17: { gamma_n = ( real_type )+22.2066099024510568923776047497213400544558;               break; }
        case 18: { gamma_n = ( real_type )+24.742549922175405981939244798300628887835;                break; }
        case 19: { gamma_n = ( real_type )+27.4155677808037739412069194441004198203158;               break; }
        case 20: { gamma_n = ( real_type )+30.2256634783361607701806286871207128518982;               break; }
        case 21: { gamma_n = ( real_type )+33.1728370147725664688603725273615079825822;               break; }
        case 22: { gamma_n = ( real_type )+36.2570883901129910372461509648228052123677;               break; }
        case 23: { gamma_n = ( real_type )+39.4784176043574344753379639995046045412548;               break; }
        default: { gamma_n = ( real_type )0.0; }
    };
    return gamma_n;
}


#endif /* CERRF_DAWSON_H__ */
