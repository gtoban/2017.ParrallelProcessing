#include <iostream>  /*cout and endl*/
#include <stdio.h>
#include <stdlib.h>  /*atof*/
#include <math.h> /*pow*/
#include <cmath> /*abs*/
#include <pthread.h>
//
using namespace std; /*cout*/
int rows;
int cols;
double top;
double myleft;
double myright;
double bottom;
double epsilon;
int threads;

double **plate1;
double **plate2;
double **tplate;
  
double start;

int *lim;

pthread_barrier_t barr;
pthread_mutex_t m;

double *maxDif;
double totalMaxDif = 0.0;
int *pcount;
int totalpcount = -1;
int power = 0; //power


void *work(void *arg);

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
  totalMaxDif = epsilon+1;
  lim = new int[threads + 1];
  pcount = new int[threads+1];
  maxDif = new double[threads+1];
  maxDif[0] = 0.0;
  pcount[0] = 0;
  lim[0] = 0;
  lim[myId] = (rows-2)*(cols-2);

  for(int i = 1; i<threads;i++){

    lim[i] = i*lim[myId]/threads;
  }
  
  //cout << top << ": " << flush;
  //cout << myleft << ": " << flush;
  //cout << myright << ": " << flush;
  //cout << bottom << ": " << flush;
  //cout << endl;
  plate1 = (double **)malloc(rows*sizeof(double));
  plate1[0] = (double *)malloc(rows*cols*sizeof(double *));
  plate2 = (double **)malloc(rows*sizeof(double));
  plate2[0] = (double *)malloc(rows*cols*sizeof(double *));
  //double **plate2 = (double **)malloc(rows*sizeof(double *));
 
  for(int i=0;i<rows;i++){
    plate1[i] = (*plate1 + i*cols);
    plate2[i] = (*plate2 + i*cols);
    
  }

  double total = 0;
  int sides = 0;

  for(int r=0;r<rows-1;r++){
    sides += 2;
    total += myleft;
    total += myright;
    plate1[r][0] = myleft;
    plate1[r][cols-1] = myright;
    plate2[r][0] = myleft;
    plate2[r][cols-1] = myright;
    
  }
  int tsides = 0;
  double ttotal = 0.0;
  for(int c=0;c<cols;c++){
    if (c==0 || c==cols-1){
      sides += 1;
      total += bottom;
      plate1[rows-1][c] = bottom;
      plate2[rows-1][c] = bottom;
    
    }else{
      sides += 2;
      total += bottom + top;
      plate1[0][c] = top;
      plate1[rows-1][c] = bottom;
      plate2[0][c] = top;
      plate2[rows-1][c] = bottom;
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
  //cout << myId << endl;
  //for(int r=1;r<rows-1;r++){
  //  for(int c=1;c<cols-1;c++){
  //pthread_mutex_lock(&m);
  int r;
  int c;
  for(int count = lim[myId-1]; count < lim[myId]; count++){
    r = 1 + count/(cols-2);
    c = 1 + count%(cols-2);
    //cout << myId << " " << r << " " << c << endl;
    plate1[r][c] = start;
    plate2[r][c] = start;
   
  }
  
  //cout << myId << " " << r << " " << c << " " << c*r << " " << lim[myId] << endl;
  //cout << "============================\n\n======================" << endl;
  //pthread_mutex_unlock(&m);
  pthread_barrier_wait(&barr);
  //return 0;
  
  maxDif[myId] = epsilon+1;
  pcount[myId] = -1; //print count
  
  double dif = 0;
  //string pause;
  while (totalMaxDif>epsilon){
    maxDif[myId] = 0;
    pcount[myId] += 1;
    //for(int i=1;i<6;i+=1){
    //  cout << plate1[i][i] << ": " << flush;
    //}
    //cout << "\n" << endl;
    //cout << plate1[1][0] << ": "<< plate1[1][2] << ": "<< plate1[0][1] << ": "<< plate1[2][1] << "\n" << endl;
    //for(int i=1;i<6;i+=1){
    //  cout << plate2[i][i] << ": " << flush;
    //}
    //cout << "\n" << endl;
    //cout << plate2[1][0] << ": "<< plate2[1][2] << ": "<< plate2[0][1] << ": "<< plate2[2][1] << "\n" << endl;
    //cout << "-----------\n\n" << endl;
    //for(int r=1;r<rows-1;r++){
    //  for(int c=1;c<cols-1;c++){
    for(int count = lim[myId-1]; count < lim[myId]; count++){
    	 r = 1 + count/(cols-2);
	 c = 1 + count%(cols-2);
	 plate2[r][c] = 0.25 * (plate1[r][c-1] + plate1[r][c+1] + plate1[r-1][c] + plate1[r+1][c]);
    	
    	dif = abs(plate2[r][c] - plate1[r][c]);
    	//cout << dif << endl;
    	//cout << plate2[r][c] - plate1[r][c] << endl;
    	//cout << plate1[r][c] << endl;
    	//cout << plate2[r][c] << endl;
    	//cout << endl;
    	//break;
    	if (dif > maxDif[myId]){
    	  maxDif[myId] = dif;
    	  //cout <<pcount << ": " << maxDif << endl;
    	  //cin >> pause;
    	}
	
	
    }
    
    
    pthread_barrier_wait(&barr);
    if (myId == threads){
      totalpcount += 1;
      //      cout << "Check" << endl;
      totalMaxDif = 0;
      for (int i=1; i<threads+1;i++){
	//totalpcount += pcount[i];
	if (maxDif[i] > totalMaxDif){
	  totalMaxDif = maxDif[i];
	}
      }
      //cout << totalpcount << endl;
      //cout << (int)(pow(2.0,power)) << endl;
      if(totalpcount == (int)(pow(2.0,power))){
	cout << totalpcount << ": " << totalMaxDif << endl;
        power += 1;
      }
      
      //cout << plate1[1][1] << endl;
      //cout << plate2[1][1] << endl;
      tplate = plate1;
      //cout << tplate[1][1] << endl;
      plate1 = plate2;
      //cout << plate1[1][1] << endl;
      plate2 = tplate;
      //cout << plate2[1][1] << endl;
      //break;
      //if (power > 25){ //failSafe
      //
      //  break;
      //}
    }
    pthread_barrier_wait(&barr);
    
  }

  if (myId == threads){
    //totalpcount += 1;
    totalMaxDif = 0;
    for (int i=1; i<threads+1;i++){
      //totalpcount += pcount[i];
      if (maxDif[i] > totalMaxDif){
	totalMaxDif = maxDif[i];
      }
    }
    cout << totalpcount << ": " << totalMaxDif << endl;
  }
  
}
