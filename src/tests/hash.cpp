#include "bu/hash.h"
#include "bu/sptr.h"

typedef struct Bob
{
	int nID;
} Bob;

int main()
{
	Bu::Hash<int, Bu::SPtr<const Bob> > lb;
	for( int j = 0; j < 10; j++ )
	{
		Bob *b = new Bob;
		b->nID = j;
		lb.insert( j, b );
	}

	for( int j = 0; j < 10; j++ )
	{
		printf("%d\n", lb[j].value()->nID );
	}
}

