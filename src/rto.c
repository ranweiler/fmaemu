#include "rto.h"


// Add a and b, rounding to odd.
//
// See [BoldoMelquiond2008], Section V.D.
double add_rnd_to_odd(double a, double b) {
    f64 t;
    double appx, err;

    exact_add(&appx, &err, a, b);

    // if (isinf(appx)) {
    //     return appx;
    // }

    // If the error term `err` is zero, then the approximate term `th`
    // is exactly the sum. Rounding-to-odd is a no-op, and we're done.
    if (err == 0.0) { return appx; }

    // If the significand of the `exact_add` result is odd, then
    // rounding-to-odd will be a no-op.
    t.f64 = appx;
    if (t.u64 & 1) { return appx; }

    // If here, then the computation is inexact and the fraction part
    // of the sum is even. This means that the final bit of `err` is
    // off by one. Whether we increment or decrement is determined by
    // the signs of the approximation and error terms.
    if ((err > 0.0) ^ (appx < 0.0)) {
        t.u64++;
    } else {
        // This will not underflow because `appx` is nonzero when `err` is.
        t.u64--;
    }

    return t.f64;
}

bool is_neg(double x) {
    return signbit(x);
}

bool is_pos(double x) {
    return !is_neg(x);
}

bool is_zero(double x) {
    return x == 0.0;
}

bool is_inf(double x) {
    return isinf(x);
}

bool is_fin(double x) {
    return !is_zero(x) && !is_inf(x);
}

bool is_pos_inf(double x) {
    return is_pos(x) && is_inf(x);
}

bool is_neg_inf(double x) {
    return is_neg(x) && is_inf(x);
}

bool is_pos_zero(double x) {
    return is_pos(x) && is_zero(x);
}

bool is_neg_zero(double x) {
    return is_neg(x) && is_zero(x);
}

bool is_fin_or_inf(double x) {
    return is_fin(x) || is_inf(x);
}

bool is_zero_or_fin(double x) {
    return is_zero(x) || is_fin(x);
}

bool is_mul_pos(double a, double b) {
    return signbit(a) == signbit(b);
}

bool same_sign(double a, double b, double c) {
    bool ab_sign = is_mul_pos(a, b) ? 0 : 1;
    bool c_sign = signbit(c);

    return ab_sign == c_sign;
}

double fma_emu_non_finite(double a, double b, double c) {
    if (isnan(a)) { return quiet_nan(a); }
    if (isnan(b)) { return quiet_nan(b); }
    if (isnan(c)) { return quiet_nan(c); }

    if (is_inf(a) && is_fin_or_inf(b) && is_pos_inf(c)) {
        if (same_sign(a, b, c)) {
            return +INFINITY;
        } else {
            return NAN;
        }
    }
    if (is_inf(a) && is_fin_or_inf(b) && is_neg_inf(c)) {
        if (same_sign(a, b, c)) {
            return -INFINITY;
        } else {
            return NAN;
        }
    }
    if (is_inf(a) && is_fin_or_inf(b) && is_zero_or_fin(c)) {
        if (same_sign(a, b, c)) {
            return +INFINITY;
        } else {
            return -INFINITY;
        }
    }
    if (is_inf(a) && is_zero(b)) {
        return NAN;
    }
    if (is_zero(a) && is_inf(b)) {
        return NAN;
    }
    if (is_fin(a) && is_inf(b) && is_pos_inf(c)) {
        if (same_sign(a, b, c)) {
            return +INFINITY;
        } else {
            return NAN;
        }
    }
    if (is_fin(a) && is_inf(b) && is_neg_inf(c)) {
        if (same_sign(a, b, c)) {
            return -INFINITY;
        } else {
            return NAN;
        }
    }
    if (is_fin(a) && is_inf(b) && is_zero_or_fin(c)) {
        if (is_mul_pos(a, b)) {
            return +INFINITY;
        } else {
            return -INFINITY;
        }
    }
    if (is_zero_or_fin(a) && is_zero_or_fin(b) && is_inf(c)) {
        if (is_pos(c)) {
            return +INFINITY;
        } else {
            return -INFINITY;
        }
    }
}

// Algorithm 1 from [BoldoMelquiond2008].
double fma_emu_rto(double a, double b, double c) {
    if (!isfinite(a) || !isfinite(b) || !isfinite(c)) {
        return fma_emu_non_finite(a, b, c);
    }

    // Approximate result and error terms of the `exact_` algorithms.
    double mul_appx, mul_err;
    double add_appx, add_err;

    exact_mul(&mul_appx, &mul_err, a, b);
    exact_add(&add_appx, &add_err, c, mul_appx);

    if (isinf(add_appx)) { return add_appx; }

    double v = add_rnd_to_odd(add_err, mul_err);
    double z = add_appx + v;

    return z;
}
