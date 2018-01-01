#include "common.h"


double from_raw(uint64_t raw) {
    f64 f = { .u64 = raw };
    return f.f64;
}

uint64_t to_raw(double x) {
    f64 f = { .f64 = x };
    return f.u64;
}

double quiet_nan(double nan) {
    f64 f = { .f64 = nan };

    f.u64 |= (uint64_t) 1 << 51;

    return f.f64;
}
