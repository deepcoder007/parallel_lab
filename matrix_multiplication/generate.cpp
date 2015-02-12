#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
using namespace std;

int main()
{
	srand(time(NULL));
	int m,n;
	cout<<"Enter the dimension of the matrix"<<endl;
	cin>>m>>n;    // the dimension of the matrix
	int* arr;
	arr=new int[n];
	int i,j;
	int sum;

	FILE* ptr = fopen("A.txt","w");

	for( i = 0 ; i< m; i++)
	{
		for(j=0;j<n; j++)
		{
			arr[j]=rand()%1000;
		}

		for(j=0;j<n;j++)
			fprintf(ptr,"%d ",arr[j]);
		fprintf(ptr,"\n");
	}
	fclose(ptr);

	FILE* ptr2 = fopen("b.txt","w");
	for( i = 0 ; i < n ; i++ )
	{
		fprintf(ptr2,"%d\n",rand()%1000);
	}
	fclose(ptr2);

	return 0;
}
