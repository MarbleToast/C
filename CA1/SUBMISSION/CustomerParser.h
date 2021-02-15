#ifndef CUSTOMER_PARSER_H
#define CUSTOMER_PARSER_H

#include <stdio.h>

int readConfigValue(FILE* file, const char* lhs_to_find, int allow_non_positive);

#endif
