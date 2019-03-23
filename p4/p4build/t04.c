// simultaneous send - will it hang or not ??

#include <stdio.h>

#include "mpi.h"

#define BUFSZ 25000  // 15000 or 25000

int main(int argc, char *argv[])
{
    int i, myrank, numranks, rc;
    int buff[BUFSZ];
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numranks);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

    printf("myrank=%d commsize=%d\n",myrank,numranks);

    if (numranks != 2)
    {
        printf("aborting: should run with exactly 2 processes, not %d\n",numranks);
        MPI_Abort(MPI_COMM_WORLD,-1);
    }

    if (myrank == 0)
    {
        buff[0] = 1000;
        rc = MPI_Send(buff,BUFSZ,MPI_INT,1,999,MPI_COMM_WORLD);
    }
    else
    {
        buff[0] = 2000;
        rc = MPI_Send(buff,BUFSZ,MPI_INT,0,999,MPI_COMM_WORLD);
    }

    rc = MPI_Recv(buff,BUFSZ,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
    printf("myrank=%d  recvrc=%d  buff0=%d  from=%d  tag=%d\n",
           myrank, rc, buff[0], status.MPI_SOURCE, status.MPI_TAG);

    MPI_Finalize();
}
