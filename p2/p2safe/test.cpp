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
  int threads = atoi(argv[8]);

  int lim[threads+1][2];

  lim[0][0] = 0;
  lim[0][1] = 1;
  if (rows >= threads){
    for(int i = 1; i<threads; i++){
      lim[i][0] = (int)((rows*i)/threads);
      cout << lim[i][0] << endl;
      lim[i][1] = cols;
    }
    lim[threads][0] = rows;
    cout << lim[threads][0] << endl;
    lim[threads][1] = cols;
    return 0;
  }else{
    
    return 0;
  }
  
  return 0;
  
}
