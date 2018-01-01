#ifndef FMA_COMMON_H
#define FMA_COMMON_H
#endif

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


// Provide multiple views of double-precision floats.
typedef union f64 {
    double f64;
    uint64_t u64;
} f64;

double from_raw(uint64_t raw);
uint64_t to_raw(double x);
double quiet_nan(double nan);
