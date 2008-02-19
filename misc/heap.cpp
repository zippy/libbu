#include <stdio.h>
#include <stdlib.h>

void printHeap( int *aiHeap, int iHeapFill )
{
	printf("graph G {\n");
	for( int j = 0; j < iHeapFill; j++ )
	{
		if( j*2+1 < iHeapFill )
			printf("    %d -- %d;\n",
					j, j*2+1
				  );
		if( j*2+2 < iHeapFill )
			printf("    %d -- %d;\n",
					j, j*2+2
				  );
	}
	for( int j = 0; j < iHeapFill; j++ )
	{
		printf("    %d [label=\"%d\"];\n",
				j, aiHeap[j]
			  );
	}
	printf("}\n");
}

void heapPush( int iNum, int *aiHeap, int &iHeapFill )
{
	for( int j = 0; j < iHeapFill; )
	{
		if( iNum < aiHeap[j] )
		{
			int iTmp = aiHeap[j];
			aiHeap[j] = iNum;
			iNum = iTmp;
		}

		if( iNum <= aiHeap[j*2+1] )
		{
			j = j*2+1;
			continue;
		}
		else
		{
			j = j*2+2;
			continue;
		}
	}
	aiHeap[iHeapFill] = iNum;
	for( int j = iHeapFill; j >= 0; )
	{
		int k = (j-1)/2;
		if( aiHeap[k] <= aiHeap[j] )
			break;
		
		int iTmp = aiHeap[k];
		aiHeap[k] = aiHeap[j];
		aiHeap[j] = iTmp;
		j = k;
	}
	iHeapFill++;
}

int heapPop( int *aiHeap, int &iHeapFill )
{
	int iRet = aiHeap[0];

	int j;
	for( j = 0; j < iHeapFill; )
	{
		if( aiHeap[j*2+1] >= aiHeap[j*2+2] && j*2+2 < iHeapFill )
		{
			aiHeap[j] = aiHeap[j*2+2];
			j = j*2+2;
		}
		else if( j*2+1 < iHeapFill )
		{
			aiHeap[j] = aiHeap[j*2+1];
			j = j*2+1;
		}
		else
			break;
	}
	aiHeap[j] = aiHeap[iHeapFill-1];
	iHeapFill--;
	return iRet;
}

int main( int argc, char *argv[] )
{
	int *aiHeap = new int[40];
	int iHeapFill = 0;

	int iNums = atoi( argv[1] );
	int iOffs = atoi( argv[2] );
	char cDo = argv[3][0];

	for( int j = 0; j < iNums; j++ )
	{
		int iNum = rand()%20;
		if( cDo == 'y' ) printf("%d ", iNum );
		heapPush( iNum, aiHeap, iHeapFill );
	}
	if( cDo == 'y' ) printf("\n");

	for( int j = 0; j < iOffs; j++ )
	{
		if( cDo == 'y' )
			printf("%d ", heapPop( aiHeap, iHeapFill ) );
		else
			heapPop( aiHeap, iHeapFill );
	}
	if( cDo == 'y' ) printf("\n");
	else printHeap( aiHeap, iHeapFill );

	delete[] aiHeap;
	return 0;
}

