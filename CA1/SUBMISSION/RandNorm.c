#include "RandNorm.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int randNorm(double mean, double sd) {

    double norm;
    double r;
    double t = 0.0;
    double u = 0.0;

    while (u == 0.0) {
        u = (double)rand() / (double)RAND_MAX;
    }

    r = sqrt(-2 * log(u));

    t = 0.0;
    while (t == 0.0) {
        t = 2 * (atan(1) / 4) * ((double)rand() / RAND_MAX);
    }

    norm = r * cos(t);

    return (norm * sd) + mean;
}

unsigned int* randNumsToSum(unsigned int target, unsigned int steps) {

    float* numbers;
    unsigned int* values;
    unsigned int sum_uint = 0;
    float sum_float = 0;
    float rand_num;
    unsigned int i;

    if (!(numbers = malloc(sizeof*numbers * steps))) {
        perror("[ERROR]:");
        exit(EXIT_FAILURE);
    }

    if (!(values = malloc(sizeof*values * steps))) {
        perror("[ERROR]:");
        exit(EXIT_FAILURE);
    }

    while (sum_uint != target) {
        sum_uint = 0;
        sum_float = 0;

        for (i = 0; i < steps;) {
            rand_num = (float)rand() / (float)RAND_MAX;
            numbers[i] = rand_num;
            sum_float += rand_num;
            ++i;
        }
	
        for (i = 0; i < steps;) {
            numbers[i] /= sum_float;
            numbers[i] *= target;
            values[i] = (unsigned int)(numbers[i]+0.5);
            sum_uint += values[i];
            ++i;
        }
    }

    free(numbers);

    return values;
}
