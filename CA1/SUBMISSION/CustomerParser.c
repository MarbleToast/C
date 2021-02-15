#include "CustomerParser.h"
#include <stdlib.h>
#include <string.h>

/* 
    Read a config value from a file.
    @param file: pointer to an opened file instance.
    @param lhs_to_find: string literal to search for in file.
    @param allow_non_positive: flag for if a value is allowed to be negative
*/
int readConfigValue(FILE* file, const char* lhs_to_find, int allow_non_positive) {
    char lhs[64] = {0};
    char rhs[64] = {0};

    char* value = NULL;
    char* stop;
    int ret;

    /* 
        Scan through file for a line in format of LHS = RHS, where both sides are captured in a char array
        If we find the LHS we are looking for, retrieve the RHS in a new char array.
    */
    while (fscanf(file, "%63s%*[ ]=%*[ ]%63s\n", lhs, rhs)) {
        if (!strcmp(lhs, lhs_to_find)) {
            if (!(value = malloc(sizeof*value*64))) {
                perror("[ERROR]:");
                exit(EXIT_FAILURE);
            };
	        strcpy(value, rhs);
            break;
        }
    }

    if (!value) {
        fprintf(stderr, "The config file did not have a definition for: %s.", lhs_to_find);
        exit(EXIT_FAILURE);
    }

    /* Cast to integer, reset the file pointer to start of file, return value. */

    ret = strtol(value, &stop, 10);

    free(value);

    if (ret < 1 && !allow_non_positive) {
        fprintf(stderr, "The config file had a non-positive value for: %s.", lhs_to_find);
        exit(EXIT_FAILURE);
    }

    rewind(file);
    return ret;
}
