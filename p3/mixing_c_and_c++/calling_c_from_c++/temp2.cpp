#include <stdio.h>


extern "C" 
{
    int f(void);
}

main()
{
    int i;

    i = f();
}
