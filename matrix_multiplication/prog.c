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


	div_size = (int)ceil((double)( m / size )  ); // Assumption m > size -> for m<=size => write another procedure
	A = (int**)malloc( sizeof(int* )*div_size );
	b = (int*) malloc ( sizeof(int)*n );
	x = (int*) malloc ( sizeof(int)*m*2 );
	buf = (int*) malloc(3*n*n*sizeof(int));
	for( i  = 0 ;i<m;i++ )
		A[i] = buf+(n*i);

	if( rank == 0 )
	{
	
		ptr2 = fopen("b.txt","r");
		for( i = 0; i< n ; i++ )
		{
			fgets( t_str, sizeof ( t_str) , ptr2 );
			b[i] = atoi( t_str );
		}
	
		fclose(ptr2);
		ptr = fopen("A.txt","r");
		for( i = 0 ; i < (size-1)*div_size ; i++)
		{
			fgets(str,sizeof(str),ptr);
		}
		j=0;
		for( ; i<m ; i++, j++)
		{
			fgets( str, sizeof(str), ptr);
			str2 = strtok( str," \n");
			k=0;
			do
			{
				A[j][k] =  atoi( str2 );
				k++;
			} while ( ( str2 = strtok( NULL, " \n") )!= NULL );
		}
		fclose( ptr );
	}
	MPI_Bcast(b, n , MPI_INT, 0, MPI_COMM_WORLD );  // send the vector b to everyone

	int* proc_row = (int*)malloc(n*div_size*sizeof(int));
	int* proc_x = (int*)malloc(div_size*sizeof(int));

	MPI_Scatter(buf, n*div_size, MPI_INT, proc_row, n*div_size, MPI_INT, 0 , MPI_COMM_WORLD );

	for(i=0; i<div_size ;i++ )
	{
		val = 0 ;
		for( j = 0; j<n;j++)
			val += proc_row[(i*n)+j]*b[j] ;
		proc_x[i]=val;
	}

	MPI_Gather( proc_x , div_size , MPI_INT, x, div_size, MPI_INT, 0 , MPI_COMM_WORLD );


	if( rank == 0)
	{
		printf("OUTPUT:\n ");
		for(i = 0 ;i<m; i++ )
			printf("%d ",x[i]);
		printf("\n");
	}


	MPI_Finalize();

	return 0;
}

