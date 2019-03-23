#include <iostream>  /*cout and endl*/
#include <stdio.h>
#include <stdlib.h>  /*atof*/
#include <math.h> /*pow*/
#include <cmath> /*abs*/

using namespace std; /*cout*/

int main(int argc, char *argv[])
{

  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  double top = atof(argv[3]);
  double left = atof(argv[4]);
  double right = atof(argv[5]);
  double bottom = atof(argv[6]);
  double epsilon = atof(argv[7]);
  
  //cout << top << ": " << flush;
  //cout << left << ": " << flush;
  //cout << right << ": " << flush;
  //cout << bottom << ": " << flush;
  //cout << endl;
  double **plate1 = (double **)malloc(rows*sizeof(double));
  plate1[0] = (double *)malloc(rows*cols*sizeof(double *));
  double **plate2 = (double **)malloc(rows*sizeof(double));
  plate2[0] = (double *)malloc(rows*cols*sizeof(double *));
  //double **plate2 = (double **)malloc(rows*sizeof(double *));
  double **tplate;
  for(int i=0;i<rows;i++){
    plate1[i] = (*plate1 + i*cols);
    plate2[i] = (*plate2 + i*cols);
    
  }

  double total = 0;
  int sides = 0;

  for(int r=0;r<rows-1;r++){
    sides += 2;
    total += left;
    total += right;
    plate1[r][0] = left;
    plate1[r][cols-1] = right;
    plate2[r][0] = left;
    plate2[r][cols-1] = right;
    
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
  double start = total/sides;
  for(int r=1;r<rows-1;r++){
    for(int c=1;c<cols-1;c++){
      plate1[r][c] = start;
      plate2[r][c] = start;
    }
  }

  double maxDif = epsilon+1;
  double dif = 0;
  int pcount = -1; //print count
  int power = 0; //power
  string pause;
  while (maxDif>epsilon){
    maxDif = 0;
    pcount += 1;
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
    for(int r=1;r<rows-1;r++){
      for(int c=1;c<cols-1;c++){
  	plate2[r][c] = 0.25 * (plate1[r][c-1] + plate1[r][c+1] + plate1[r-1][c] + plate1[r+1][c]);
  
  	dif = abs(plate2[r][c] - plate1[r][c]);
  	//cout << dif << endl;
  	//cout << plate2[r][c] - plate1[r][c] << endl;
  	//cout << plate1[r][c] << endl;
  	//cout << plate2[r][c] << endl;
  	//cout << endl;
  	//break;
  	if (dif > maxDif){
  	  maxDif = dif;
  	  //cout <<pcount << ": " << maxDif << endl;
  	  //cin >> pause;
  	}
  	  
      }
    }
    
    if(pcount == (int)(pow(2.0,power))){
      cout << pcount << ": " << maxDif << endl;
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
  cout << pcount << ": " << maxDif << endl;
}
