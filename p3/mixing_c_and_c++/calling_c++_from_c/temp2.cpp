#include <stdio.h>

extern "C" 
{
    int f(void);
}

int f(void)
{
    printf("INSIDE CPP\n");
    return 0;
}

