/* C Example */
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>
#include<iostream>
using namespace std;



int main ( int argc, char *argv[])
{
  int rank, size;
  char msg[20];
  int myrank, tag=99;

  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Status status;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */
  MPI_Request req;
  int* a;
  int* b;
	  a=(int*)malloc(size*10*sizeof(int));
	  b=(int*)malloc(size*10*sizeof(int));

  for(int i = 0;i<size*10;i++)
  {	
	  a[i]=i;
  }
  MPI_Gather(a+rank*10, 10, MPI_INT, b, 10, MPI_INT, 0,MPI_COMM_WORLD );

  if( rank == 0 )
  {
//	  MPI_Gather(a+rank*10, 10, MPI_INT, b, 10, MPI_INT, 0,MPI_COMM_WORLD );
	  for(int i= 0 ;i<size*10;i++)
	  {
		  cout<<i<<"   "<<b[i]<<endl;
	  }
  }

  MPI_Finalize();
  return 0;
}

