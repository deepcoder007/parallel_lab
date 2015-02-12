#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
using namespace std;

int main()
{
	srand(time(NULL));
	int n;
	cout<<"Enter the dimension of the matrix"<<endl;
	cin>>n;    // the dimension of the matrix
	int* arr;
	arr=new int[n];
	int i,j;
	int sum;

	FILE* ptr = fopen("A.txt","w");
	FILE* ptr2 = fopen("b.txt","w");

	for( i = 0 ; i< n; i++)
	{
		sum=0;
		for(j=0;j<n; j++)
		{
			arr[j]=rand()%1000;
			sum+=arr[j];
		}
		arr[i]=sum+rand()%1000;

		for(j=0;j<n;j++)
			fprintf(ptr,"%d ",arr[j]);
		fprintf(ptr,"\n");
		fprintf(ptr2,"%d\n",rand()%100000);
	}
	fclose(ptr);
	fclose(ptr2);

	return 0;
}
