// hello world enhanced with Abort demo and Wtime demo

#include <stdio.h>
 
#include "mpi.h"

int main(int argc, char *argv[])
{
    int i, rc, myrank, numranks;
    double wtick, wtime, stime;
    printf("%d", argc);
    for (i=0;i<argc;i++)
      printf("argv[%d]=%s\n",i,argv[i]);
    rc = MPI_Init(&argc,&argv);
    rc = MPI_Comm_size(MPI_COMM_WORLD,&numranks);
    rc = MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
 
    printf("rank %d\n",myrank);
    printf("numranks %d\n",numranks);

    // Abort demo
    if (argc > 1  &&  strcmp(argv[1],"-a") == 0)
    {
        if (myrank == 0)
        {
            printf("%d: testing abort\n",myrank);
            MPI_Abort(MPI_COMM_WORLD, -1);
        }
        printf("%d: working\n",myrank);
        while (1)
            ;
    }

    //  Wtime / Wtick demo
    if (argc > 1  &&  strcmp(argv[1],"-w") == 0)  // Wtime / Wtick
    {
        if (myrank == 0)
        {
            wtick = MPI_Wtick();
            wtime = MPI_Wtime();
            printf("%d:  wtick %e %g  wtime %e %g\n", myrank, wtick,wtick, wtime,wtime );
            printf("%d:  timediff %g\n", myrank, MPI_Wtime()-MPI_Wtime() );
            stime = MPI_Wtime();
            while ( (MPI_Wtime() - stime) < 3.1)
                ;
            printf("%d: done\n",myrank);
        }
    }

    MPI_Finalize();
}
