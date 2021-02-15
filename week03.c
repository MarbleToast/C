#include <stdio.h>

int main(int argc, char const *argv[])
{
    char str[20] = "Hello world!\n";
    char *p = str;

    printf("str is %s\n", str);
    printf("p is %s\n", p);

    p += 4;
    printf("p is %s\n", p);

    *p = 'x';
    printf("p is %s\n", p);

    *p = '\0';
    printf("p is %s\n", p);

    system("pause");
    return 0;
}
