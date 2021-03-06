#include <iostream>  /*cout and endl*/
#include <stdio.h>
#include <stdlib.h>  /*atof*/
#include <math.h> /*pow*/
#include <cmath> /*abs*/
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

using namespace std; /*cout*/
int rows;
int cols;
double top;
double myleft;
double myright;
double bottom;
double epsilon;
int threads;

double **gplate1;
double **gplate2;

  
double start;

int *lim;

pthread_barrier_t barr;
pthread_mutex_t m;

double *gmaxDif1;
double *gmaxDif2;
int *pcount;
int totalpcount = -1;
int power = 0; //power

double t1;
double t2;


void *work(void *arg);

double time1()
{
    struct timeval tv;

    gettimeofday( &tv, ( struct timezone * ) 0 );
    return ( (double) (tv.tv_sec + (tv.tv_usec / 1000000.0)) );
}

int main(int argc, char *argv[])
{
  if (argc < 9){
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
  threads = atoi(argv[8]);

  long myId = (long)threads;
  //totalMaxDif = epsilon+1;
  lim = new int[threads + 1];
  pcount = new int[threads+1];
  gmaxDif1 = new double[threads+1];
  gmaxDif1[0] = 0.0;
  gmaxDif2 = new double[threads+1];
  gmaxDif2[0] = 0.0;
  pcount[0] = 0;
  lim[0] = 1;
  lim[myId] = (rows-2);

  for(int i = 1; i<threads;i++){

    lim[i] = lim[0] + i*lim[myId]/threads;
  }
  
  //cout << top << ": " << flush;
  //cout << myleft << ": " << flush;
  //cout << myright << ": " << flush;
  //cout << bottom << ": " << flush;
  //cout << endl;
  gplate1 = (double **)malloc(rows*sizeof(double));
  gplate1[0] = (double *)malloc(rows*cols*sizeof(double *));
  gplate2 = (double **)malloc(rows*sizeof(double));
  gplate2[0] = (double *)malloc(rows*cols*sizeof(double *));
  //double **gplate2 = (double **)malloc(rows*sizeof(double *));
 
  for(int i=0;i<rows;i++){
    gplate1[i] = (*gplate1 + i*cols);
    gplate2[i] = (*gplate2 + i*cols);
    
  }

  double total = 0;
  int sides = 0;

  for(int r=0;r<rows-1;r++){
    sides += 2;
    total += myleft;
    total += myright;
    gplate1[r][0] = myleft;
    gplate1[r][cols-1] = myright;
    gplate2[r][0] = myleft;
    gplate2[r][cols-1] = myright;
    
  }
  int tsides = 0;
  double ttotal = 0.0;
  for(int c=0;c<cols;c++){
    if (c==0 || c==cols-1){
      sides += 1;
      total += bottom;
      gplate1[rows-1][c] = bottom;
      gplate2[rows-1][c] = bottom;
    
    }else{
      sides += 2;
      total += bottom + top;
      gplate1[0][c] = top;
      gplate1[rows-1][c] = bottom;
      gplate2[0][c] = top;
      gplate2[rows-1][c] = bottom;
    }
  }
  start = total/sides;

  pthread_t tid[threads];

  pthread_mutex_init(&m, NULL);
  pthread_barrier_init(&barr,NULL,threads); 
  for (int i=1; i < threads; i++)  // start at 1; I am 0
  {
    //cout << "made it" << endl;
      pthread_create(&tid[i],NULL,work,(void*)(long int)i);
  }
  //cout << "threads" << endl;
  
  work((void *)myId);
  return 0;
  
  
}


void *work(void *arg){
  long myId = (long)arg;
  double **plate1 = gplate1;
  double **plate2 = gplate2;
  double **tplate;

  double *maxDif = gmaxDif1;
  double *maxDif2 = gmaxDif2;
  double *tmaxDif;
  double totalMaxDif = epsilon+1;
  
  //cout << myId << endl;
  //for(int r=1;r<rows-1;r++){
  //  for(int c=1;c<cols-1;c++){
  //pthread_mutex_lock(&m);
  int r;
  int c;
  //for(int count = lim[myId-1]; count < lim[myId]; count++){
  //  r = 1 + count/(cols-2);
  //  c = 1 + count%(cols-2);
  //cout << start << endl;
  for(int r=lim[myId-1];r<=lim[myId];r++){
      for(int c=1;c<cols-1;c++){
    //cout << myId << " " << r << " " << c << endl;
   	plate1[r][c] = start;
	plate2[r][c] = start;
      }
  }
 
  //cout << myId << " " << r << " " << c << " " << c*r << " " << lim[myId] << endl;
  //cout << "============================\n\n======================" << endl;
  //pthread_mutex_unlock(&m);
  pthread_barrier_wait(&barr);
  //return 0;
  
  //maxDif[myId] = epsilon+1;
  //pcount[myId] = -1; //print count
  
  double dif = 0;
  //string pause;
  while (totalMaxDif>epsilon){
    maxDif[myId] = 0;
    //for(int count = lim[myId-1]; count < lim[myId]; count++){
    //	 r = 1 + count/(cols-2);
    //	 c = 1 + count%(cols-2);
    //	 plate2[r][c] = 0.25 * (plate1[r][c-1] + plate1[r][c+1] + plate1[r-1][c] + plate1[r+1][c]);
    //	
    //	dif = abs(plate2[r][c] - plate1[r][c]);
    //	if (dif > maxDif[myId]){
    //	  maxDif[myId] = dif;
    //	}
    //
    //	
    //}
    for(int r=lim[myId-1];r<=lim[myId];r++){
      for(int c=1;c<cols-1;c++){
  	plate2[r][c] = 0.25 * (plate1[r][c-1] + plate1[r][c+1] + plate1[r-1][c] + plate1[r+1][c]);
	//cout << r << " " << c << endl;
	//cout << plate2[r][c] << " " << plate1[r][c] << endl;
  	dif = abs(plate2[r][c] - plate1[r][c]);
    	if (dif > maxDif[myId]){
    	  maxDif[myId] = dif;
    	}
  	  
      }
    }
    
    
    pthread_barrier_wait(&barr);
    tplate = plate1;
    plate1 = plate2;
    plate2 = tplate;
    totalMaxDif = 0;
    tmaxDif = maxDif;
    maxDif = maxDif2;
    maxDif2 = tmaxDif;
    for (int i=1; i<threads+1;i++){
      //totalpcount += pcount[i];
      if (maxDif2[i] > totalMaxDif){
	totalMaxDif = maxDif2[i];
      }
    }
    if (myId == threads){
      totalpcount += 1;
      if(totalpcount == (int)(pow(2.0,power))){
	cout << totalpcount << ": " << totalMaxDif << endl;
        power += 1;
      }
      
    }
    //pthread_barrier_wait(&barr);
    
  }

  if (myId == threads){
    //totalpcount += 1;
    totalMaxDif = 0;
    for (int i=1; i<threads+1;i++){
      //totalpcount += pcount[i];
      if (maxDif2[i] > totalMaxDif){
	totalMaxDif = maxDif2[i];
      }
    }
    cout << totalpcount << ": " << totalMaxDif << endl;
  }
  
}
