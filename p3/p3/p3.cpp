#include <iostream>  /*cout and endl*/
#include <stdio.h>
#include <stdlib.h>  /*atof*/
#include <math.h> /*pow*/
#include <cmath> /*abs*/
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

using namespace std; /*cout*/

extern "C"
{

  double evalfunc(int nodeid, int *expandid1, int *expandid2);
  
}

int threads;
int toFind;
int *threadIds;
int *parents;
int *parentsCopy;
int parentCount;
int assignedParents;
int **solutions;
int solutionCount;
bool foundAllFlag;
int *nodeCount;
int totalNodeCount;

pthread_barrier_t barr;
pthread_mutex_t m;
pthread_mutex_t m1;

void recursion(int parentId, long &myId);
void *work(void *arg);

double time1()
{
    struct timeval tv;

    gettimeofday( &tv, ( struct timezone * ) 0 );
    return ( (double) (tv.tv_sec + (tv.tv_usec / 1000000.0)) );
}

int main(int argc, char *argv[])
{
  if (argc < 3){
    cout << "Too Few Params" << endl;
    return -1;
  }
  threads = atoi(argv[1]);
  toFind = atoi(argv[2]);
  int startLevel = 0;
  int i=0.0;
  int expandid1,expandid2;
  int pexpandid1,pexpandid2;
  double nodeStat;
  int count = 0;
  parentCount = 0;
  assignedParents = threads-1;
  solutionCount = 0;
  foundAllFlag = false;
  /*
   *
   * Ensure The Start Level Is One More than Necessary
   * This will provide some room for one or more threads
   * to finish before the others
   */

  while((int)pow(2.0,(double)startLevel) < threads)
    startLevel++;
  startLevel++;
  //cout << "startLevel: " << startLevel << ", " << (int)pow(2.0,(double)startLevel) << endl;

  parents = (int *)malloc((int)pow(2.0,(double)startLevel)*sizeof(int ));
  parentsCopy = (int *)malloc((int)pow(2.0,(double)startLevel)*sizeof(int ));
  solutions = (int **)malloc(toFind*sizeof(int *));
  *solutions = (int *)malloc(toFind*2*sizeof(int));
  threadIds = (int *)malloc(threads*sizeof(int ));
  nodeCount = (int *)malloc((threads+1)*sizeof(int ));

  for (int i=0; i<toFind; i++)
    *(solutions + i) = *solutions + i*2;
  totalNodeCount = 0;
  for(int i=0;i<=threads;i++)
    nodeCount[i] = 0;
  
  
  nodeStat = evalfunc(0, &expandid1, &expandid2);
  //totalNodeCount++;
  pexpandid1 = expandid1;
  pexpandid2 = expandid2;
  
  if (nodeStat > 1.0){
    //cout << "Made it" << endl;
    solutions[solutionCount][0] = 0;
    solutions[solutionCount][1] = nodeStat;
    solutionCount++;
    nodeCount[threads] += 1;
  }

  //CHECK 1 child
  nodeStat = evalfunc(pexpandid1, &expandid1, &expandid2);
  totalNodeCount++;
  parents[parentCount] = pexpandid1;
  parentsCopy[parentCount] = pexpandid1;
  parentCount++;
  if (nodeStat >= 1.0){
    //cout << "Made it" << endl;
    solutions[solutionCount][0] = pexpandid1;
    solutions[solutionCount][1] = nodeStat;
    solutionCount++;
    nodeCount[threads] += 1;
  }

  //CHECK 2nd CHILD
  nodeStat = evalfunc(pexpandid2, &expandid1, &expandid2);
  totalNodeCount++;
  parents[parentCount] = pexpandid2;
  parentsCopy[parentCount] = pexpandid2;
  parentCount++;

  if (nodeStat > 1.0){
    //cout << "Made it" << endl;
    solutions[solutionCount][0] = pexpandid2;
    solutions[solutionCount][1] = nodeStat;
    solutionCount++;
    nodeCount[threads] += 1;
  }

  if(solutionCount > toFind){
    //cout << "all solutions found " << endl;
    //return 0;
    foundAllFlag == true;
  }

  /*
   * Find the parent for each thread
   * 
   *
   */
  int prevCount = parentCount;
  int levelCount = 1;
  int totalCount = parentCount;
  //cout << "startLevel: " << startLevel << endl;
  //cout << "\n" << "parents: " << parentCount << endl;
  //cout << "totalCount: " << totalCount << endl;    
  //cout << "levelCount: " << levelCount << "\n" << endl;
  while(parentCount < threads || levelCount < startLevel){

    /*
     *
     * Check the children each parent added to the list last time
     *
     */
    parentCount = 0;
    for (int i =0; i < prevCount; i++){
      //cout << "\n" << "parents: " << parentCount << endl;
      //cout << "totalCount: " << totalCount << "\n" << endl;    
  
      totalCount += 2;
      nodeStat = evalfunc(parentsCopy[i], &expandid1, &expandid2);
      pexpandid1 = expandid1;
      pexpandid2 = expandid2;
      //cout << "parent,child,child: " << flush;
      //cout << parentsCopy[i] << "," <<  pexpandid1 << "," << pexpandid2 <<endl;
      //CHECK 1 child
      nodeStat = evalfunc(pexpandid1, &expandid1, &expandid2);
      nodeCount[threads] += 1;
      if (nodeStat > 1.0){
	//cout << "Made it" << endl;
	solutions[solutionCount][0] = pexpandid1;
	solutions[solutionCount][1] = nodeStat;
	solutionCount++;
	parents[parentCount] = pexpandid1;
	//parentsCopy[parentCount] = pexpandid1;
	parentCount++;

      }else{
	parents[parentCount] = pexpandid1;
	//parentsCopy[parentCount] = pexpandid1;
	parentCount++;

      }
      
      //CHECK 2nd CHILD
      nodeStat = evalfunc(pexpandid2, &expandid1, &expandid2);
      nodeCount[threads] += 1;
      if (nodeStat > 1.0){
	//cout << "Made it" << endl;
	solutions[solutionCount][0] = pexpandid2;
	solutions[solutionCount][1] = nodeStat;
	solutionCount++;
	parents[parentCount] = pexpandid2;
	//parentsCopy[parentCount] = pexpandid2;
	parentCount++;

      }else {
	parents[parentCount] = pexpandid2;
	//parentsCopy[parentCount] = pexpandid2;
	parentCount++;

      }
      
      if(solutionCount > toFind){
	//cout << "all solutions found " << endl;
	//return 0;
	foundAllFlag = true;
      }

      
      //parentsCopy[i] = 

    }

    for (int i =0; i < parentCount; i++){
      parentsCopy[i] = parents[i];
      //cout << parents[i] << " " << flush;
    }    

    levelCount++;
    //cout << "\n" << "parents: " << parentCount << endl;
    //cout << "totalCount: " << totalCount << endl;
    //cout << "levelCount: " << levelCount << endl;
    //for (int i =0; i < parentCount; i++){
    //  //parentsCopy[i] = parents[i];
    //  cout << parents[i] << " " << flush;
    //}
    //cout << endl;
    prevCount = parentCount;
  }

  //cout << "Parent Count: " << parentCount << endl;
  pthread_t tid[threads];
  pthread_mutex_init(&m, NULL);
  pthread_barrier_init(&barr,NULL,threads); 
  for (int i=0; i < threads; i++)  // start at 1; I am 0
  {
    threadIds[i] = i;
    //cout << "PID: " << i << endl;
    pthread_create(&tid[i],NULL,work,(void*)(long int)threadIds[i]);
  }

  for (int i=0; i<threads;i++){
    (void) pthread_join(tid[i], NULL);
    totalNodeCount += nodeCount[i];
  }
  totalNodeCount += nodeCount[threads];
  cout << "NodeId, Value\n" << "____________" << endl; 
  for (int i=0; i<solutionCount; i++){

    cout << solutions[i][0] << ", " << solutions[i][1] << endl;
  }

  cout << "\nA total of " << totalNodeCount << " nodes were searched." << endl;
  cout << "A total of " << solutionCount << " solutions were found." << endl;
  
  ////cout << "threads" << endl;
  //
  //work((void *)myId);
  return 0;
  
  
}


void *work(void *arg){
  long myId = (long)arg;
  int parentId = myId;
  int myParent = parents[parentId];
  pthread_barrier_wait(&barr);
  //cout << myId<< " " << nodeCount[myId] << endl;
  //pthread_mutex_lock(&m);
  //cout << myId << ", " << parentId << endl;
  //printf("%d, %d\n", myId, parentId);
  //
  while(parentId < parentCount){
  //
    //printf("%d, %d, %d\n", myParent, myId, parentId);
  
    recursion(myParent, myId);
    
    //pthread_mutex_lock(&m);
    pthread_mutex_lock(&m);
    //cout << assignedParents << endl;
    assignedParents++;
    parentId = assignedParents;
    pthread_mutex_unlock(&m);
    //cout << parentId << endl;
    //pthread_mutex_unlock(&m);
    myParent = parents[parentId];
    //break;
    //cout << myId<< " " << nodeCount[myId] << endl;
    //cout << myId << ", " << parentId << endl;
  }
  
}

void recursion(int parentId, long &myId){
  if (foundAllFlag || parentId < 0)
      return;
  //cout << myId << ", " << parentId << endl;
  //printf("%d, %d\n", parentId, myId);
  int expandid1,expandid2;
  int pexpandid1,pexpandid2;
  double nodeStat;
  
  nodeStat = evalfunc(parentId, &expandid1, &expandid2);
  pexpandid1 = expandid1;
  pexpandid2 = expandid2;
  //cout << "parent,child,child: " << flush;
  //cout << parentsCopy[i] << "," <<  pexpandid1 << "," << pexpandid2 <<endl;
  //CHECK 1 child
  if (pexpandid1 > 0){
    nodeStat = evalfunc(pexpandid1, &expandid1, &expandid2);
    if (nodeStat > 0){
      nodeCount[myId] += 1;
      if (nodeStat > 1.0){
	//cout << "Made it" << endl;
	pthread_mutex_lock(&m1);
    
	solutions[solutionCount][0] = pexpandid1;
	solutions[solutionCount][1] = nodeStat;
	solutionCount++;
	if (solutionCount == toFind)
	  foundAllFlag = true;
	pthread_mutex_unlock(&m1);

	if (foundAllFlag)
	  return;
	recursion(pexpandid1, myId);
      }else {
	recursion(pexpandid1, myId);
      }
    }
  }  
  //CHECK 2nd CHILD
  if (pexpandid2 > 0){
    nodeStat = evalfunc(pexpandid2, &expandid1, &expandid2);
    if (nodeStat > 0){
      nodeCount[myId] += 1;
      if (nodeStat > 1.0){

	pthread_mutex_lock(&m1);

	solutions[solutionCount][0] = pexpandid2;
	solutions[solutionCount][1] = nodeStat;
	solutionCount++;
	if (solutionCount == toFind)
	  foundAllFlag = true;
	pthread_mutex_unlock(&m1);

	if (foundAllFlag)
	  return;
	recursion(pexpandid2, myId);
  
      }else {
	recursion(pexpandid2, myId);
  
      }
    }
  }
  

}
