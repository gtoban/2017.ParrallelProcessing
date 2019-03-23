#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

__global__    // <--- writing a kernel function to be run on the gpu (called on host)
void saveIDs(int *idsOut){
     //int tid = threadIdx.x;
     //    int bidx = blockIdx.x;
     //	     int bdim = blockDim.x;
     //	         int globaltid;
     //
     //globaltid = blockIdx.x * blockDim.x + threadIdx.x;
    
     const int tid = threadIdx.x;  // <-- multi-dimensional; just getting one dim right now

    idsOut[tid] = tid;  // put my gpu thread id into the vector
}

// __host__   default for all functions
// __device__ can be invoked on GPU side (called on device and runs on device - kernel helpers)

__global__    // <--- writing a kernel function to be run on the gpu (called on host)
void steady(double *plate1, double *plate2, double *maxDiff, int &rows, int &cols, double &epsilon, int &iteration,double &totalMaxDiff, int *barrier) {
     double diff = 0;
     int globaltid;
     globaltid = blockIdx.x * blockDim.x + threadIdx.x;
     //printf("%d: barrier %d, iteration %d, epsilon %f, cols %d, rows %d\n", globaltid,barrier[globaltid], iteration, epsilon, cols, rows);
     //
     //return;
     //if (globaltid == -1){
     //  for(int i=0;i<rows;i++){
     //	 for (int j=0;j<cols;j++){
     //	   printf("%4.4f ", *(plate1 + i*cols + j));
     //	 }
     //	 printf("\n");
     //  }
     //}
     //printf("MADE IT %d\n",globaltid);
     
     //if (globaltid < rows-2){
       //printf("%d: %4.2f %d %d %1.2f %d\n", globaltid, *(plate1 + globaltid*cols), rows, cols, epsilon, iteration);
     if (globaltid < rows-1 && globaltid!=0){
       barrier[globaltid]=0;
       //printf("%d: barrier %d\n", globaltid,barrier[globaltid]);
       return;
       do{
	 maxDiff[globaltid]=0;
	 //printf("%d: %d %d\n", globaltid, iteration, iteration%2);
	 if (iteration%2==0){
	   for(int i=1;i<cols-1;i++){
	     *(plate2 + (globaltid)*cols + i) = 0.25 * (*(plate1 + (globaltid)*cols + i-1) +
							*(plate1 + (globaltid)*cols + i+1) +
							*(plate1 + (globaltid-1)*cols + i) +
							*(plate1 + (globaltid+1)*cols + i));

	     diff = abs(*(plate2 + (globaltid)*cols + i) - *(plate1 + (globaltid)*cols + i));
	     if(maxDiff[globaltid]<diff)
	       maxDiff[globaltid]=diff;
	 
	   }
	   //printf("%d: %4.4f %4.4f\n", globaltid, maxDiff[globaltid],diff);
	
	 }else{

	   for(int i=1;i<cols-1;i++){
	     //val = *(plate2 (globaltid+1)*cols + i-1) ;
	     *(plate1 + (globaltid)*cols + i) = 0.25 * (*(plate2 + (globaltid)*cols + i-1) +
							*(plate2 + (globaltid)*cols + i+1) +
							*(plate2 + (globaltid-1)*cols + i) +
							*(plate2 + (globaltid+1)*cols + i));

	     diff = abs(*(plate1 + (globaltid)*cols + i) - *(plate2 + (globaltid)*cols + i));
	     if(maxDiff[globaltid]<diff)
	       maxDiff[globaltixd]=diff;
	 
	   }
	 }
	 barrier[globaltid] = 1;
	 printf("%d: %f\n", globaltid, barrier[globaltid]);
	 while(barrier[globaltid] == 1)
	   continue;
	 
       }while(totalMaxDiff < epsilon);
     }else if(globaltid == 0){
       
       int power=0;
       for (int i=1; i<rows-1;i++){
	 printf("%d: barrier %d\n", globaltid,barrier[i]);
       
	 while(barrier[i] == -1){
	   continue;
	 }
       }
       return;
       do{
	 totalMaxDiff = 0;
	 for (int i=1; i<rows-1;i++){
	   while(barrier[i] == 0)
	     continue;
	 }
	 
	 for (int i=1; i<rows-1; i++){
	   if(maxDiff[i] > totalMaxDiff)
	     totalMaxDiff = maxDiff[i];
	   //barrier[i] = 0;
	 }
	  
	 for (int i=1; i<rows-1;i++)
	   barrier[i] = 0;

	 if (iteration == (int)pow(2.0,power))
	   printf("%d: %f\n", iteration, totalMaxDiff);
	  
	 
	 iteration++;
       }while(totalMaxDiff < epsilon);
       
     }
}
     
       //printf("%d: %4.4f %4.4f\n", globaltid, maxDiff[globaltid],diff);

     //}

     //double **myplate = plate1 + globaltid*cols);
     //printf("%d: %4.2f %d %d %1.2f %d\n", globaltid, *(plate1 + globaltid*cols), rows, cols, epsilon, iteration);
     //printf("%d: %4.2f %d %d %1.2f %d\n", globaltid, 49.6, rows, cols, epsilon, iteration);
     //printf("%d: \n", globaltid);

     //

int main(int argc, char *argv[])
{
  
	
    if (argc < 8){
       printf("Too Few Params\n");
       return -1;
    }
    int rows, *drows;
    int cols, *dcols;
    double top;
    double myleft;
    double myright;
    double bottom;
    double epsilon, *depsilon;

    double **hplate1;
    double **hplate2;
    double *dplate1;
    double *dplate2;
    double *dmaxDiff, *dtotalMaxDiff;
    int blocks, threadsPerBlock;
    int *barrier, *dbarrier;
    int iteration, *diteration;
    rows = atoi(argv[1]);
    cols = atoi(argv[2]);
    top = atof(argv[3]);
    myleft = atof(argv[4]);
    myright = atof(argv[5]);
    bottom = atof(argv[6]);
    epsilon = atof(argv[7]);

    //double numerator = ((cols-2)*top + (rows-1)*(myleft+myright) + bottom*cols);
    //double demon = (cols-2) + 2*(rows-1) + cols;
    double start = ((cols-2)*top + (rows-1)*(myleft+myright) + bottom*cols)  /  ((cols-2) + 2*(rows-1) + cols);
    //printf("%4.2f\n", start);


    hplate1 = (double **)malloc(rows*sizeof(double *));
    hplate1[0] = (double *)malloc(rows*cols*sizeof(double));
    hplate2 = (double **)malloc(rows*sizeof(double *));
    hplate2[0] = (double *)malloc(rows*cols*sizeof(double));
    barrier = (int *)malloc((rows-1)*sizeof(int));

    //double *plate1, double *plate2,
    //double *maxDiff, int &rows, int &cols, double &epsilon,
    //int &iteration, int &totalMaxDiff, int *barrier) 
    cudaMalloc(&dplate1,rows*cols*sizeof(double));
    cudaMalloc(&dplate2,rows*cols*sizeof(double));
    cudaMalloc(&dmaxDiff,(rows-2)*sizeof(double));
    cudaMalloc(&drows,sizeof(int));
    cudaMalloc(&dcols,sizeof(int));
    cudaMalloc(&depsilon,sizeof(double));
    cudaMalloc(&diteration,sizeof(int));
    cudaMalloc(&dtotalMaxDiff,sizeof(double));
    cudaMalloc(&dbarrier,(rows-1)*sizeof(int));//
    
    
    
    for(int i=0;i<rows;i++){
      hplate1[i] = (*hplate1 + i*cols);
      hplate2[i] = (*hplate2 + i*cols);

    }
  
    for(int r=0;r<rows-1;r++){
      hplate1[r][0] = myleft;
      hplate1[r][cols-1] = myright;
      hplate2[r][0] = myleft;
      hplate2[r][cols-1] = myright;

    }
    for(int c=0;c<cols;c++){
       if (c==0 || c==cols-1){
       	  hplate1[rows-1][c] = bottom;
	  hplate2[rows-1][c] = bottom;

       }else{
          hplate1[0][c] = top;
	  hplate1[rows-1][c] = bottom;
	  hplate2[0][c] = top;
	  hplate2[rows-1][c] = bottom;
       }
    }

    for(int r=1;r<rows-1;r++){
       for(int c=1;c<cols-1;c++){
	  hplate1[r][c] = start;
	  hplate2[r][c] = start;
       }
    }
			  
    //double *plate1, double *plate2,
    //double *maxDiff, int &rows, int &cols, double &epsilon,
    //int &totalMaxDiff, int *barrier) 
    for (int i=0;i<rows-1;i++)
      barrier[i]=-1;
    iteration = 0;
    //printf("%d: barrier %d, iteration %d, epsilon %f, cols %d, rows %d\n----------\n\n", 0,barrier[0], iteration, epsilon, cols, rows);
    
    cudaMemcpy(dplate1, hplate1[0], rows*cols*sizeof(double),  cudaMemcpyHostToDevice);
    cudaMemcpy(dplate2, hplate2[0], rows*cols*sizeof(double),  cudaMemcpyHostToDevice);
    cudaMemcpy(drows, &rows, sizeof(int),  cudaMemcpyHostToDevice);
    cudaMemcpy(dcols, &cols, sizeof(int),  cudaMemcpyHostToDevice);
    cudaMemcpy(depsilon, &epsilon, sizeof(double),  cudaMemcpyHostToDevice);
    cudaMemcpy(diteration, &iteration, sizeof(int),  cudaMemcpyHostToDevice);
    cudaMemcpy(dbarrier, barrier, (rows-1)*sizeof(int),  cudaMemcpyHostToDevice);
    
    //steady();
    blocks = (int)((rows-2)/1024) + 1;
    if (rows%blocks == 0)
      threadsPerBlock = (int)((rows-2)/blocks);
    else
      threadsPerBlock = (int)((rows-2)/blocks) + blocks;

    //double *plate1, double *plate2,
    //double *maxDiff, int &rows, int &cols, double &epsilon,
    //int &iteration, int &totalMaxDiff, int *barrier)
    
    steady<<<threadsPerBlock, 1 >>>(dplate1, dplate2, dmaxDiff,*drows, *dcols, *depsilon,*diteration, *dtotalMaxDiff,dbarrier);
    cudaDeviceSynchronize();
      
    //do{
    //  cudaMemcpy(diteration, &iteration, sizeof(int),  cudaMemcpyHostToDevice);
    //
    //  steady<<< blocks, threadsPerBlock >>>(dplate1, dplate2, dmaxDiff, *drows, *dcols, *depsilon, *diteration);
    //  cudaDeviceSynchronize();
    //  cudaMemcpy(maxDiff, dmaxDiff, (rows-2)*sizeof(double),  cudaMemcpyDeviceToHost);
    //  totalmaxDiff = 0;
    //  for (int i=0;i<rows-2;i++){
    //	//printf("maxDiff[%d] = %f\n", i, maxDiff[i]);
    //	if(maxDiff[i]>totalmaxDiff)
    //	  totalmaxDiff=maxDiff[i];
    //	
    //  }
    //
    //  if(iteration == (int)pow(2.0,power)){
    //	power+=1;
    //	printf("%d: %f\n",iteration,totalmaxDiff);
    //  }
    //  //if(iteration == 0){
    //  //	printf("%d: %f\n",iteration,totalmaxDiff);
    //  //
    //  //
    //  //}//
    //  iteration+=1;
    //  
    //}while(totalmaxDiff>epsilon);
    //
    //printf("%d: %4.4f\n",iteration-1,totalmaxDiff);
    
    
    //free(hplate1);
    //free(hplate2);
    //free(maxDiff);
    //cudaFree(dplate1);
    //cudaFree(dplate2);
    //cudaFree(dmaxDiff);
    //cudaFree(drows);
    //cudaFree(dcols);
    //cudaFree(depsilon);
    //cudaFree(diteration);
    //

    //printf("done\n");
    cudaDeviceReset();
			
    return 0;
}
    
