/* C Example */
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>
#include<math.h>

#ifndef ITER
#define ITER 100
#endif


int main ( int argc, char *argv[])
{
	int rank, size;
	char str[10000];  // because A can be very large also
	char t_str[100];
	char* str2;
	int myrank, tag=99;

	int** A;   // m*n
	int* b;   // m*1
	int* x;   // n*1
	int m,n;     // stores the dimension of A
	int i,j,k,l;
	int div_size;   // the size of each division
	int val = 0;
	int* buf;


	// Read all the data in the array

	FILE* ptr =fopen("A.txt","r");
	FILE* ptr2;
	i=0;
	while( fgets(str, sizeof(str), ptr) != NULL )
	{
		i++;
	}
	fclose(ptr);
	str2 = strtok(str," ");
	j=0;
	do
	{
		j++;
	}while ( (str2 = strtok(NULL," \n"))!=NULL );
	m=i;
	n=j;

	MPI_Init (&argc, &argv);	/* starts MPI */
	MPI_Status status;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */

	:q
	c





	MPI_Finalize();

	return 0;
}

