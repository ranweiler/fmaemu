#include "fma.h"


void print_f64(double x) {
    f64 f;
    f.f64 = x;
    printf("%lx\n", f.u64);
}

double fma_f64(double a, double b, double c) {
    return b*a + c;
}

// Note that we permute the `a` and `b` arguments so that we
// compute `ab + c`, not `ba + c`. This is impacts the result
// when multiple arguments are NaN.
double fma_asm(double a, double b, double c) {
    double out;

    asm volatile(
        "movq %1, %%xmm0 \t\n"
        "movq %2, %%xmm1 \t\n"
        "movq %3, %%xmm2 \t\n"
        "vfmadd213sd %%xmm2, %%xmm0, %%xmm1 \t\n"
        "movq %%xmm1, %0"
        : "=out"(out)
        : "a"(a),
          "b"(b),
          "c"(c)
    );

    return out;
}
