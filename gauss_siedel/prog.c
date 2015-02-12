/* C Example */
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mpi.h>

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

	float** A;   // m*n
	float* b;   // m*1
	float* x;   // n*1
	int m,n;     // stores the dimension of A
	int i,j,k,l;
	int div_size;   // the size of each division


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

	if( m != n)
	{
		printf("ERROR: Only square matrix allowed \n");
		return 0;
	}

	MPI_Init (&argc, &argv);	/* starts MPI */
	MPI_Status status;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */


	div_size = ( m / size +1 ); // Assumption m > size -> for m<=size => write another procedure

	// now read and store data according to the convenience

	if( 0 == rank )  // the root process will read only the residual data
	{
	//	printf("parent to be programmed \n");
		A = (float**)malloc( sizeof(float*)*div_size );
		for(i=0;i<m;i++)
			A[i]=(float*)malloc( sizeof(float)*n );
		b = (float*)malloc( sizeof(float)*div_size );
		x = (float*)malloc( sizeof(float)*n );
		memset( x , 1 , sizeof(x) );   // initialize x to [0,0, .. , 0 ]

		// read the A and b
		ptr=fopen("A.txt","r");
		ptr2 = fopen("b.txt","r");
		// skip the initial rows of A and b
		for(i=0 ; i < (size-1)*div_size ; i++ )
		{
			fgets(str,sizeof(str), ptr) ;  // read a_i
			fgets(t_str , sizeof(t_str), ptr2 );   // read b_i
		}
		// now read the div_size no. of rows to the arrays
		j=0;  // marker for matrix A and b
		for( ; i< m  ; i++,j++ )
		{
			fgets(str,sizeof(str), ptr) ;  // read a_i
			fgets(t_str , sizeof(t_str), ptr2 );   // read b_i
//			printf("Rank: %d  b : %d \n", rank,  atoi(t_str) );
	//		printf("Rank: %d     A_i : %s \n",rank,str);
			b[j]= atoi(t_str);
			// Now tokenize the A and initialize

			str2 = strtok( str, " \n");
			k=0;
			do
			{
				A[j][k] = atoi( str2 );
			//	printf("str2 : %d \n", atoi(str2 ));
				k++;
			}while(  ( str2 = strtok( NULL , " \n") )!= NULL );
		}
		fclose(ptr);
		fclose(ptr2);

		MPI_Barrier(MPI_COMM_WORLD);

		// data input complete -> now start the processing

	//	while( 1)
		for(l =0 ; l< ITER; l++)
		{
			// Broadcast value of x
		//	MPI_Bcast( x, n , MPI_FLOAT, 0, MPI_COMM_WORLD );
		//	printf("HERE ROOT \n");

			float* t_x = (float*)malloc( sizeof(float)*div_size );   // to hold the temporary received values


			// Now collect the x

			for( i = 1; i < size ; i++ )   // for all the child process
			{
				MPI_Send ( x , n , MPI_FLOAT, i, 0, MPI_COMM_WORLD );
				MPI_Recv ( t_x, div_size , MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status );

				for( j= (i-1)*div_size ; j < i*div_size ; j++ )
				{
					x[j] = t_x[j - (i-1)*div_size ];
				}

			}

			// then update the x
			for( i =  (size-1)* div_size ; i <  m ; i++ )
			{

				float sum=0;
				for( j = 0 ; j< n ;j++)
				{
					if( j==i) continue ;  
					sum += A[i - (size-1)*div_size ][j]*x[j];
				}
				x[ i ] = ( b[i - (size-1)*div_size]  -  sum ) / A[i - (size-1)*div_size ][i] ;
			}



			printf("x : ");
			for( i = 0 ; i< n ;i++)
				printf(" %f ", x[i] );
			printf("\n");

		}

	}
	else    // other processes will read the other data
	{
		A = (float**)malloc( sizeof(float*)*div_size );
		for(i=0 ; i< div_size ;i++)
			A[i]=(float*)malloc( sizeof(float)*n );
		b = (float*)malloc( sizeof(float)*div_size );
		x = (float*)malloc( sizeof(float)*n );
		memset( x , 0 , sizeof(x) );   // initialize x to [0,0, .. , 0 ]

		// read the A and b
		ptr=fopen("A.txt","r");
		ptr2 = fopen("b.txt","r");
		// skip the initial rows of A and b
		for(i=0 ; i < (rank-1)*div_size ; i++ )
		{
			fgets(str,sizeof(str), ptr) ;  // read a_i
			fgets(t_str , sizeof(t_str), ptr2 );   // read b_i
		}
		// now read the div_size no. of rows to the arrays
		j=0;  // marker for matrix A and b
		for( ; i< (rank)*div_size ; i++,j++ )
		{
			fgets(str,sizeof(str), ptr) ;  // read a_i
			fgets(t_str , sizeof(t_str), ptr2 );   // read b_i
		//	printf("b : %d \n", atoi(t_str) );
//			printf("Rank: %d  b : %d \n", rank,  atoi(t_str) );
	//		printf("Rank: %d     A_i : %s \n",rank,str);
			b[j]= atoi(t_str);
			// Now tokenize the A and initialize

			str2 = strtok( str, " \n");
			k=0;
			do
			{
				A[j][k] = atoi( str2 );
			//	printf("str2 : %d \n", atoi(str2 ));
				k++;
			}while(  ( str2 = strtok( NULL , " \n") )!= NULL );
		}
		fclose(ptr);
		fclose(ptr2);

		MPI_Barrier(MPI_COMM_WORLD);

		// data input complete -> now start the processing

	//	while( 1 )
		for(l =0 ; l< ITER; l++)
		{
			// first get the value of current x from the roort
			// Broadcast value of x
		//	MPI_Bcast( x, n , MPI_FLOAT, 0, MPI_COMM_WORLD );
		//	printf("HERE CHILD \n");

			MPI_Recv ( x, n , MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status );


			// then update the x
			for( i =  (rank-1)* div_size ; i <  rank*div_size ; i++ )
			{

				float sum=0;
				for( j = 0 ; j< n ;j++)
				{
					if( j==i) continue ;  
					sum += A[i - (rank-1)*div_size ][j]*x[j];
				}
				x[ i ] = ( b[i - (rank-1)*div_size]  -  sum ) / A[i - (rank-1)*div_size ][i] ;
			}
			
			// send the changed value s of x to root
			MPI_Send( &x[ (rank-1)*div_size ], div_size , MPI_FLOAT, 0, 0, MPI_COMM_WORLD );
		}

	}

	MPI_Finalize();
	return 0;
}

