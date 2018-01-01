#ifndef FMA_COMMON_H
#include "common.h"
#endif


void split(double *x_out, double *y_out, double a);

void exact_add(double *sum_appx, double *sum_err, double a, double b);

void exact_mul(double *mul_appx, double *mul_err, double a, double b);
