#include "RandNorm.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
    Generates a random, normallly distributed integer.
    @param mean: Mean of normal distribution
    @param sd: Variance of normal distribution
    @returns A random normallly distributed integer 
*/
int randNorm(double mean, double sd) {

    /* Utilises Box-Muller transform method */
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

/*
    Naively generates an integer array of size [steps], where the sum of all elements equals a predefined target.
    @param target: Sum of array
    @param steps: Size of array
    @returns An integer array of size [steps]
*/
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

    /* Naive loop to eventually generate exact target- rarely fails, so non-intensive */
    while (sum_uint != target) {
        sum_uint = 0;
        sum_float = 0;

        /* Create [steps] floats between 0 and 1 */
        for (i = 0; i < steps;) {
            rand_num = (float)rand() / (float)RAND_MAX;
            numbers[i] = rand_num;
            sum_float += rand_num;
            ++i;
        }
	
        /* Average and round floats to nearest integer */
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
