#include <stdio.h>

double evalfunc(int nodeid, int *expandid1, int *expandid2);

void main(int argc, char *argv[])
{
    int currnode, e1, e2, e3, e4, e5;
    double rc;

    currnode = 0;
    rc = evalfunc(currnode, &e1, &e2);
    printf("at currnode %d  rc %lf  e1 %d  e2 %d\n",currnode,rc,e1,e2);

    currnode = e1;
    rc = evalfunc(currnode, &e1, &e3);
    printf("at currnode %d  rc %lf  e1 %d  e3 %d\n",currnode,rc,e1,e3);

    currnode = e2;
    rc = evalfunc(currnode, &e2, &e4);
    printf("at currnode %d  rc %lf  e2 %d  e4 %d\n",currnode,rc,e2,e4);

    currnode = e3;
    rc = evalfunc(currnode, &e3, &e5);
    printf("at currnode %d  rc %lf  e3 %d  e5 %d\n",currnode,rc,e3,e5);
}
