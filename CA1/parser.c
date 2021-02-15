#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_config_value(FILE *file, const char *lhs_to_find) {
    char lhs[128];
    char rhs[128];

    char *value;

    while (fscanf(file, "%s%*[ ]=%*[ ]%s\n", lhs, rhs)) {
        if (strcmp(lhs, lhs_to_find) == 0) {
            if(!(value = strdup(rhs))) {
                perror("[ERROR]:");
                exit(EXIT_FAILURE);
            };
            break;
        }
    }

    if (!value) {
        fprintf(stderr, "The config file did not have a definition for: %s.", lhs_to_find);
        exit(EXIT_FAILURE);
    }

    char *stop;
    int ret = strtol(value, &stop, 10);

    free(value);

    if (ret < 1) {
        fprintf(stderr, "The config file had a non-positive value for: %s.", lhs_to_find);
        exit(EXIT_FAILURE);
    }

    rewind(file);
    return ret;
};