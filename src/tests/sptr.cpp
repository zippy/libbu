#include <stdio.h>
#include "sptr.h"

class Annoy
{
public:
	Annoy() : nCnt( 0 )
	{
		printf("Created.\n");
	}

	virtual ~Annoy()
	{
		printf("Destroyed.\n");
	}

	virtual void go()
	{
		printf("%d: I'm annoying.\n", ++nCnt);
	}

	int nCnt;
};

class Annoy2: public Annoy
{
public:
	Annoy2(){};
	virtual ~Annoy2(){};
	virtual void go()
	{
		printf("{{I'm Annoy2!!}} ");
		Annoy::go();
	}
	virtual void go2()
	{
		printf("This is me, on my own...\n");
	}
};

void beAnnoying( SPtr<Annoy> bob )
{
	printf("bob-Count: %d\n", bob.count() );
	bob->go();
}

int main()
{
	SPtr<Annoy> pt( new Annoy2 );
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

			{
				SPtr<Annoy2> pt4( SPtrCast<Annoy2>( pt3 ) );
				printf("Count: %d\n", pt4.count() );

				pt4->go2();
			}
			printf("Count: %d\n", pt.count() );
		}
		printf("Count: %d\n", pt.count() );
	}
	printf("Count: %d\n", pt.count() );
}

