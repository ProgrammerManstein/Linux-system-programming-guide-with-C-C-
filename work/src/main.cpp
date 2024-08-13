#include <stdio.h>
#include "add.hpp"
#include "minus.hpp"

int main()
{
    int a=10; int b=5;
    int res = add(a, b);
    printf("a + b = %d\n", res);
    res = minus(a, b);
    printf("a - b = %d\n", res);

    return 0;
}