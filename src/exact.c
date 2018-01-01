#include "exact.h"


// Algorithm 2.3 from [Graillat2008], due to Dekker & Veltkamp.
void split(double *x_out, double *y_out, double a) {
    // The constant `f` is the value:
    //
    //     2^s - 1
    //
    // where:
    //     s = ceil(p/2)
    //
    // In double precision, `p` = 53, so `s` = 27.
    double f = 134217729.0;

    double c = f * a;
    double x = c - (c - a);
    double y = a - x;

    *x_out = x;
    *y_out = y;
}

// Algorithm 2.1 from [Graillat2008], due to Knuth.
void exact_add(double *sum_appx, double *sum_err, double a, double b) {
    double appx = a + b;
    double t = appx - a;
    double err = (a - (appx - t)) + (b - t);

    *sum_appx = appx;
    *sum_err = err;
}

// Algorithm 2.4 from [Graillat2008], due to Dekker & Veltkamp.
void exact_mul(double *mul_appx, double *mul_err, double a, double b) {
    double a1, a2, b1, b2;
    double appx = a * b;

    split(&a1, &a2, a);
    split(&b1, &b2, b);

    double err = a2*b2 - (((appx - a1*b1) - a2*b1) - a1*b2);

    *mul_appx = appx;
    *mul_err = err;
}
