#include "hash.h"
#include "staticstring.h"

int main()
{
	const char *names[]={
		"Homer the Great",
		"And Maggie Makes Three",
		"Bart's Comet",
		"Homie The Clown",
		"Bart Vs Australia",
		"Homer vs Patty and Selma",
		"A star is burns",
		"Lisa's Wedding",
		"Two Dozen and One Greyhounds",
		"The PTA Disbands",
		"Round Springfield",
		"The Springfield connection",
		"Lemon of Troy",
		"Who Shot Mr. Burns (Pt. 1)",
		"Who Shot Mr. Burns (pt. 2)",
		"Radioactive Man",
		"Home Sweet Homediddly-dum-doodly",
		"Bart Sells His Soul",
		"Lisa the Vegetarian",
		"Treehouse of horror VI",
		"King Size Homer",
		"Mother Simpson",
		"Sideshow Bob's Last Gleaming",
		"The Simpson's 138th Show Spectacular",
		"Marge Be Not Proud",
		"Team Homer",
		"Two Bad Neighbors",
		"Scenes From the Class Struggle in Springfield",
		"Bart the Fink",
		"Lisa the Iconoclast",
		"Homer the Smithers",
		"The Day the Violence Died",
		"A Fish Called Selma",
		"Bart on the road",
		"22 Short Films about Springfield",
		"The Curse of the Flying Hellfish",
		"Much Apu about Nothing",
		"Homerpalooza",
		"The Summer of 4 Ft 2",
		"Treehouse of Horror VII",
		"You Only Move Twice",
		"The Homer They Fall",
		"Burns Baby Burns",
		"Bart After Dark",
		"A Millhouse Divided",
		"Lisas Date With Destiny",
		"Hurricane Neddy",
		"The Mysterious Voyage of Our Homer",
		"The Springfield Files",
		"The Twisted World of Marge Simpson",
		"Mountain of Madness",
		NULL
	};

	Hash<const char *, int> sTest;

	printf("Inserting\n-------------------\n\n");
	for( int j = 0; j < 33; j++ )
	{
		sTest[names[j]] = j;
	}
	
	printf("Test1: %d, Test2: %d\n", sTest.has("Lemon of Troy"), sTest.has(std::string("Lemon of Troy").c_str() ) );

	sTest.has(std::string("Lemon of Troy").c_str() );

	printf("Getting\n-------------------\n\n");

	sTest.erase("Homer the Great");
	sTest["Bart's Comet"].erase();

	for( Hash<const char *, int>::iterator i = sTest.begin();
		 i != sTest.end(); i++ )
	{
		Hash<const char *, int>::iterator j = i;
		printf("%d: %s\n", (*j).second, (*j).first );
	}

	printf("Testing\n-------------------\n\n");
	for( int j = 0; j < 33; j++ )
	{
		if( sTest.has(names[j]) )
		{
			if( sTest[names[j]] != j )
			{
				printf("'%s' should be %d, is %d\n",
					names[j], j,
					sTest[names[j]].value()
					);
			}
		}
		else
		{
			printf("Missing element %d, '%s'\n", j, names[j] );
		}
	}

	printf("Clearing\n-------------------\n\n");

	sTest.clear();
	
	for( Hash<const char *, int>::iterator i = sTest.begin();
		 i != sTest.end(); i++ )
	{
		Hash<const char *, int>::iterator j = i;
		printf("%d: %s\n", (*j).second, (*j).first );
	}

}

