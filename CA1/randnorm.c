#include "randnorm.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int randnorm(double mean, double sd) {
    double u = 0.0;
    while (u == 0.0) {
        u = (double)rand() / (double)RAND_MAX;
    }

    double r = sqrt(-2 * log(u));

    double t = 0.0;
    while (t == 0.0)
    {
        t = 2 * (atan(1)/4) * ((double)rand() / RAND_MAX);
    }

    double norm = r * cos(t);

    return (norm * sd) + mean;
}