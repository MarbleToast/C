#include <stdio.h>

typedef struct ll_node_char
{
    char data;
    struct ll_node_char *prev;
    struct ll_node_char *next;
} LL_NODE_CHAR;

typedef struct ll_node_int
{
    int data;
    struct ll_node_int *prev;
    struct ll_node_int *next;
} LL_NODE_INT;

typedef struct ll_node_float
{
    float data;
    struct ll_node_float *prev;
    struct ll_node_float *next;
} LL_NODE_FLOAT;

int main(int argc, char const *argv[])
{
    LL_NODE_CHAR *headc = NULL;
    LL_NODE_INT *headi = NULL;
    LL_NODE_FLOAT *headf = NULL;

    LL_NODE_CHAR *newc;
    if (!(newc = (LL_NODE_CHAR*) malloc(sizeof(LL_NODE_CHAR))) ) {
        return 1;
    }

    LL_NODE_INT *newi;
    newi = (LL_NODE_INT*) malloc(sizeof(LL_NODE_INT));
    if (!(newc = (LL_NODE_CHAR*) malloc(sizeof(LL_NODE_CHAR))) ) {
        return 1;
    }

    LL_NODE_FLOAT *newf;
    newf = (LL_NODE_FLOAT*) malloc(sizeof(LL_NODE_FLOAT));
    if (!(newc = (LL_NODE_CHAR*) malloc(sizeof(LL_NODE_CHAR))) ) {
        return 1;
    }

    free(newc);

    return 0;
}
