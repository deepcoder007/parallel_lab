/* C Example */
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>


int main ( int argc, char *argv[])
{
  int rank, size;
  char msg[20];
  int myrank, tag=99;

  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Status status;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */

	if( rank == 0)
	{
  		int dims[2];
		MPI_Dims_create( size, 2, dims );
		int i;
		for( i=0 ;i<2 ; i++)
		{
			printf("For dim %d : %d \n", i+1, dims[i] );
		}
	}




//  printf( "Hello world from process %d of %d\n", rank, size );
  MPI_Finalize();
  return 0;
}

