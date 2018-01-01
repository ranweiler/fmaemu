#include "mp.h"


double fma_emu_mp(double a, double b, double c) {
    mpfr_t A, B, C, T;
    double out;

    if (isnan(a)) { return quiet_nan(a); }
    if (isnan(b)) { return quiet_nan(b); }
    if (isnan(c)) { return quiet_nan(c); }

    mpfr_set_default_prec(128);

    mpfr_init_set_d(A, a, MPFR_RNDN);
    mpfr_init_set_d(B, b, MPFR_RNDN);
    mpfr_init_set_d(C, c, MPFR_RNDN);
    mpfr_init(T);

    // t := b*a
    mpfr_mul(T, B, A, MPFR_RNDN);

    // t := t + c
    mpfr_add(T, T, C, MPFR_RNDN);

    out = mpfr_get_d(T, MPFR_RNDN);

    mpfr_clear(A);
    mpfr_clear(B);
    mpfr_clear(C);
    mpfr_clear(T);

    return out;
}
