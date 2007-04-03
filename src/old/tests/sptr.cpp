#include <stdio.h>
#include "sptr.h"

class Annoy
{
public:
	Annoy() : nCnt( 0 )
	{
		printf("Created.\n");
	}

	~Annoy()
	{
		printf("Destroyed.\n");
	}

	void go()
	{
		printf("%d: I'm annoying.\n", ++nCnt);
	}

	int nCnt;
};

void beAnnoying( SPtr<Annoy> bob )
{
	printf("bob-Count: %d\n", bob.count() );
	bob->go();
}

int main()
{
	SPtr<Annoy> pt( new Annoy );
	printf("Count: %d\n", pt.count() );
	pt->go();

	{
		SPtr<Annoy> pt2 = pt;
		printf("Count: %d\n", pt2.count() );

		pt2->go();

		{
			SPtr<Annoy> pt3( pt2 );
			printf("Count: %d\n", pt3.count() );

			pt3->go();

			beAnnoying( pt3 );
		}
		printf("Count: %d\n", pt.count() );
	}
	printf("Count: %d\n", pt.count() );
}

