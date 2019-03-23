#include <iostream>  /*cout and endl*/
#include <stdio.h>
#include <stdlib.h>  /*atof*/
#include <math.h> /*pow*/
#include <cmath> /*abs*/
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "mpi.h"


using namespace std; /*cout*/
int rows,rankrows;
int cols;
double top;
double myleft;
double myright;
double bottom;
double epsilon;

double **gplate1;
double **gplate2;
double **tplate;
double *passrow, *rowscatter;

  
double start;
int power;

double globalDiff,localDiff;

void *work(void *arg);

double time1()
{
    struct timeval tv;

    gettimeofday( &tv, ( struct timezone * ) 0 );
    return ( (double) (tv.tv_sec + (tv.tv_usec / 1000000.0)) );
}

int main(int argc, char *argv[])
{
  if (argc < 8){
    cout << "Too Few Params" << endl;
    return -1;
  }
  rows = atoi(argv[1]);
  cols = atoi(argv[2]);
  top = atof(argv[3]);
  myleft = atof(argv[4]);
  myright = atof(argv[5]);
  bottom = atof(argv[6]);
  epsilon = atof(argv[7]);
  
  
  double numerator = ((cols-2)*top + (rows-1)*(myleft+myright) + bottom*cols);
  double demon = (cols-2) + 2*(rows-1) + cols;
  start = numerator/demon;
  
  
  int i, rc, myrank, numranks,tag1,myround,iteration;//,status;
  double wtick, wtime, stime, diff;
  MPI_Status status1, status2;
  MPI_Request request1, request2, request3, request4;

  rc = MPI_Init(&argc,&argv);
  rc = MPI_Comm_size(MPI_COMM_WORLD,&numranks);
  rc = MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

  //printf("rank %d\n",myrank);
  //printf("numranks %d\n",numranks);

  if(numranks == 1){
    printf("Need at least 2 ranks\n");
    MPI_Abort(MPI_COMM_WORLD,-1);
  }

  if(myrank > rows-2) //do not allow more ranks than rows
    MPI_Abort(MPI_COMM_WORLD,-1);
  //return 0;
  
  rankrows = (rows-2)/(numranks-1)+2; //there sections plus two for the rows they need to calculate next step
  
  if(myrank==0){
    //printf("numranks %d\n",numranks);
    //printf("gen rankrows: %d\n",rankrows);
    rankrows=4;


  }else if(myrank==numranks-1){
    if (numranks>2)
      rankrows=(rows-2)-(numranks-2)*(rankrows-2) + 2;
    else
      rankrows=rows;
    //printf("%d: rankrows: %d\n",myrank,rankrows);

  }
  gplate1 = (double **)malloc(rankrows*sizeof(double));
  gplate1[0] = (double *)malloc(rankrows*cols*sizeof(double *));
  gplate2 = (double **)malloc(rankrows*sizeof(double));
  gplate2[0] = (double *)malloc(rows*cols*sizeof(double *));
  passrow = (double *)malloc(numranks*cols*sizeof(double));
  rowscatter = (double *)malloc(numranks*cols*sizeof(double));
  
  for(int i=0;i<rankrows;i++){
    gplate1[i] = (*gplate1 + i*cols);
    gplate2[i] = (*gplate2 + i*cols);

  }
  
  
  
  for(int r=0;r<rankrows;r++){
    gplate1[r][0] = myleft;
    gplate1[r][cols-1] = myright;
    gplate2[r][0] = myleft;
    gplate2[r][cols-1] = myright;
    
    for(int c=1;c<cols-1;c++){
      	gplate1[r][c] = start;
  	gplate2[r][c] = start;
    }
  
  }
  if(myrank==1){

    for(int c=1;c<cols-1;c++){
      	gplate1[0][c] = top;
  	gplate2[0][c] = top;
    }
  
  }
  
  if(myrank == numranks-1){
    for(int c=0;c<cols;c++){
      	gplate1[rankrows-1][c] = bottom;
  	gplate2[rankrows-1][c] = bottom;
    }
  
  }

  //if(myrank==0){
  //  for(int c=0;c<cols;c++){
  //    	gplate1[2][c] = top;
  //	gplate2[2][c] = top;
  //	gplate1[1][c] = bottom;
  //	gplate2[1][c] = bottom;
  //  
  //  }
  //
  //}
    
  if(myrank!=0){
    localDiff = epsilon+1;
  
    while(localDiff>epsilon){
      localDiff=0;
      //if(myrank!=0){
      for (int row =1; row<rankrows-1; row++){
	for (int col=1;col<cols-1;col++){
	  gplate2[row][col]=0.25 * (gplate1[row][col-1] + gplate1[row][col+1] + gplate1[row-1][col] + gplate1[row+1][col]);
	  diff=abs(gplate2[row][col]-gplate1[row][col]);
	  //printf("diff: %4.4f\n ",diff);
	
	  if(localDiff<diff)
	    localDiff=diff;
	}
      }
      //}
      //share rows

      if(numranks > 2){
	//if(myrank==1){
	//  //myrank*10;
	//  //myround = (myrank-1<0)? numranks-1:myrank-1;
	//  MPI_Send(gplate2[rankrows-2],cols,MPI_DOUBLE,2,10,MPI_COMM_WORLD);
	//  //MPI_Send(gplate2[rankrows-2],cols,MPI_DOUBLE,(myrank+1)%(numranks),myrank*10+1,MPI_COMM_WORLD);
	//  //tag1=(myrank-1)*10;
    	//
	//}else if(myrank==2){
	//  MPI_Recv(gplate2[0],cols,MPI_DOUBLE,1,10,MPI_COMM_WORLD,&status1);
    	//
	//  //MPI_Recv(gplate2[0],cols,MPI_DOUBLE,myrank-1,(myrank-1)*10+1,MPI_COMM_WORLD,&status2);
    	//
	//}
	//
	//if(myrank==numranks-1){
	//  MPI_Send(gplate2[1],cols,MPI_DOUBLE,numranks-2,numranks-1,MPI_COMM_WORLD);
    	//
	//}else if(myrank==numranks-2){
	//  MPI_Recv(gplate2[rankrows-1],cols,MPI_DOUBLE,numranks-1,numranks-1,MPI_COMM_WORLD,&status1);
	//  
	//}
	if(myrank>1 && myrank<numranks-1){
	  if(myrank%2==1){
	    MPI_Send(gplate2[rankrows-2],cols,MPI_DOUBLE,myrank+1,myrank*10,MPI_COMM_WORLD);
	  
	    MPI_Send(gplate2[1],cols,MPI_DOUBLE,myrank-1,myrank*10,MPI_COMM_WORLD);

	    MPI_Recv(gplate2[0],cols,MPI_DOUBLE,myrank-1,(myrank-1)*10,MPI_COMM_WORLD,&status1);
	    
	    MPI_Recv(gplate2[rankrows-1],cols,MPI_DOUBLE,myrank+1,(myrank+1)*10,MPI_COMM_WORLD,&status1);

	    
	  }else{
	    MPI_Recv(gplate2[0],cols,MPI_DOUBLE,myrank-1,(myrank-1)*10,MPI_COMM_WORLD,&status1);
	    
	    MPI_Recv(gplate2[rankrows-1],cols,MPI_DOUBLE,myrank+1,(myrank+1)*10,MPI_COMM_WORLD,&status1);

	    MPI_Send(gplate2[rankrows-2],cols,MPI_DOUBLE,myrank+1,myrank*10,MPI_COMM_WORLD);
	  
	    MPI_Send(gplate2[1],cols,MPI_DOUBLE,myrank-1,myrank*10,MPI_COMM_WORLD);
	    

	  }

	}else{
	  if(myrank==1){
	    
	    MPI_Send(gplate2[rankrows-2],cols,MPI_DOUBLE,2,10,MPI_COMM_WORLD);
	  
	    MPI_Recv(gplate2[rankrows-1],cols,MPI_DOUBLE,myrank+1,(myrank+1)*10,MPI_COMM_WORLD,&status1);
	  
	    
	  }else if(myrank==numranks-1){
	    if(myrank%2==1){
	      MPI_Send(gplate2[1],cols,MPI_DOUBLE,numranks-2,(numranks-1)*10,MPI_COMM_WORLD);
    	
	      MPI_Recv(gplate2[0],cols,MPI_DOUBLE,myrank-1,(myrank-1)*10,MPI_COMM_WORLD,&status1);
	  
	    }else{
	      
	      MPI_Recv(gplate2[0],cols,MPI_DOUBLE,myrank-1,(myrank-1)*10,MPI_COMM_WORLD,&status1);
	  
	      MPI_Send(gplate2[1],cols,MPI_DOUBLE,numranks-2,(numranks-1)*10,MPI_COMM_WORLD);
    	
	      
	    }
	  }
	  
	}
      
      }
      //if(myrank%2==0 && numranks!=2){
      //	//myrank*10;
      //	myround = (myrank-1<0)? numranks-1:myrank-1;
      //	MPI_Send(gplate2[1],cols,MPI_DOUBLE,myround,myrank*10,MPI_COMM_WORLD);
      //	MPI_Send(gplate2[rankrows-2],cols,MPI_DOUBLE,(myrank+1)%(numranks),myrank*10+1,MPI_COMM_WORLD);
      //	//tag1=(myrank-1)*10;
      //	
      //}else if((myrank+1)%2==0 && numranks!=2){
      //	MPI_Recv(gplate2[rankrows],cols,MPI_DOUBLE,(myrank+1)%(numranks),((myrank+1)%(numranks))*10,MPI_COMM_WORLD,&status1);
      //	myround = (myrank-1<0)? numranks-1:myrank-1;
      //	MPI_Recv(gplate2[0],cols,MPI_DOUBLE,myround,(myround)*10+1,MPI_COMM_WORLD,&status2);
      //	
      //}
      //share maxdiff
      MPI_Reduce(&localDiff, &globalDiff, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
      
      //get maxdiff
      MPI_Bcast(&localDiff,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
      //printf("%d: localDiff: %4.4f\n",myrank,localDiff);
      
      
      //break;
      tplate=gplate1;
      gplate1=gplate2;
      gplate2=tplate;
    
    }
  }else{
    power=0.0;
    iteration=0;
    localDiff = epsilon+1;
    while(localDiff>epsilon){
      localDiff=0;
      //MPI_Recv(gplate2[2],cols,MPI_DOUBLE,1,10,MPI_COMM_WORLD,&status1);
      //MPI_Recv(gplate2[2],cols,MPI_DOUBLE,1,10,MPI_COMM_WORLD,&status1);
      //
      //
      //MPI_Isend(gplate2[3],cols,MPI_DOUBLE,numranks-1,1,MPI_COMM_WORLD,&request2);
      //MPI_Isend(gplate2[0],cols,MPI_DOUBLE,1,0,MPI_COMM_WORLD,&request1);
      //tag1=(myrank-1)*10;
      //MPI_Recv(gplate2[1],cols,MPI_DOUBLE,1,10,MPI_COMM_WORLD,&status2);
  
  
      
      MPI_Reduce(&localDiff, &globalDiff, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
      localDiff=globalDiff;
      if(iteration==(int)pow(2.0,power)){
	
	printf("%d: iteration: %d localDiff: %4.4f\n",myrank,iteration,globalDiff);
	power++;
      }
      
      iteration++;
      
      MPI_Bcast(&globalDiff,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
      
      //break;
    }
  }

 if(myrank==0){
      //printf("%d: iteration: %d localDiff: %4.4f\n",myrank,iteration,localDiff);
      //printf("iteration: %d, pow: %4.4f", iteration,pow(2.0,0.0));
      	
    printf("%d: iteration: %d localDiff: %4.4f\n",myrank,iteration-1,localDiff);
       
    
  }

  //}
  MPI_Finalize();
  
  return 0;

  
  
}


void *work(void *arg){
  long myId = (long)arg;
  //double **plate1 = gplate1;
  //double **plate2 = gplate2;
  //double **tplate;
  //
  //double *maxDif = gmaxDif1;
  //double *maxDif2 = gmaxDif2;
  //double *tmaxDif;
  //double totalMaxDif = epsilon+1;
  //
  ////cout << myId << endl;
  ////for(int r=1;r<rows-1;r++){
  ////  for(int c=1;c<cols-1;c++){
  ////pthread_mutex_lock(&m);
  //int r;
  //int c;
  ////for(int count = lim[myId-1]; count < lim[myId]; count++){
  ////  r = 1 + count/(cols-2);
  ////  c = 1 + count%(cols-2);
  ////cout << start << endl;
  //for(int r=lim[myId-1];r<=lim[myId];r++){
  //    for(int c=1;c<cols-1;c++){
  //  //cout << myId << " " << r << " " << c << endl;
  // 	plate1[r][c] = start;
  //	plate2[r][c] = start;
  //    }
  //}
  //
  ////cout << myId << " " << r << " " << c << " " << c*r << " " << lim[myId] << endl;
  ////cout << "============================\n\n======================" << endl;
  ////pthread_mutex_unlock(&m);
  //pthread_barrier_wait(&barr);
  ////return 0;
  //
  ////maxDif[myId] = epsilon+1;
  ////pcount[myId] = -1; //print count
  //
  //double dif = 0;
  ////string pause;
  //while (totalMaxDif>epsilon){
  //  maxDif[myId] = 0;
  //  //for(int count = lim[myId-1]; count < lim[myId]; count++){
  //  //	 r = 1 + count/(cols-2);
  //  //	 c = 1 + count%(cols-2);
  //  //	 plate2[r][c] = 0.25 * (plate1[r][c-1] + plate1[r][c+1] + plate1[r-1][c] + plate1[r+1][c]);
  //  //	
  //  //	dif = abs(plate2[r][c] - plate1[r][c]);
  //  //	if (dif > maxDif[myId]){
  //  //	  maxDif[myId] = dif;
  //  //	}
  //  //
  //  //	
  //  //}
  //  for(int r=lim[myId-1];r<=lim[myId];r++){
  //    for(int c=1;c<cols-1;c++){
  //	plate2[r][c] = 0.25 * (plate1[r][c-1] + plate1[r][c+1] + plate1[r-1][c] + plate1[r+1][c]);
  //	//cout << r << " " << c << endl;
  //	//cout << plate2[r][c] << " " << plate1[r][c] << endl;
  //	dif = abs(plate2[r][c] - plate1[r][c]);
  //  	if (dif > maxDif[myId]){
  //  	  maxDif[myId] = dif;
  //  	}
  //	  
  //    }
  //  }
  //  
  //  
  //  pthread_barrier_wait(&barr);
  //  tplate = plate1;
  //  plate1 = plate2;
  //  plate2 = tplate;
  //  totalMaxDif = 0;
  //  tmaxDif = maxDif;
  //  maxDif = maxDif2;
  //  maxDif2 = tmaxDif;
  //  for (int i=1; i<threads+1;i++){
  //    //totalpcount += pcount[i];
  //    if (maxDif2[i] > totalMaxDif){
  //	totalMaxDif = maxDif2[i];
  //    }
  //  }
  //  if (myId == threads){
  //    totalpcount += 1;
  //    if(totalpcount == (int)(pow(2.0,power))){
  //	cout << totalpcount << ": " << totalMaxDif << endl;
  //      power += 1;
  //    }
  //    
  //  }
  //  //pthread_barrier_wait(&barr);
  //  
  //}
  //
  //if (myId == threads){
  //  //totalpcount += 1;
  //  totalMaxDif = 0;
  //  for (int i=1; i<threads+1;i++){
  //    //totalpcount += pcount[i];
  //    if (maxDif2[i] > totalMaxDif){
  //	totalMaxDif = maxDif2[i];
  //    }
  //  }
  //  cout << totalpcount << ": " << totalMaxDif << endl;
  //}
  
}
