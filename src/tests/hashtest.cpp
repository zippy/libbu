#include <stdio.h>
#include <iostream>
#include "hashtable.h"
#include "hashfunctioncasestring.h"

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

	HashTable h( new HashFunctionCaseString(), 5, false );

	int j;
	printf("Inserting...\n");
	for( j = 0; j < 10; j++ )
	{
		h.insert( names[j], (void *)(j+1) );
		h.insert( names[j], (void *)(j+1) );
		printf("Capacity: %lu, Size: %lu, Load: %f\n",
			h.getCapacity(),
			h.getSize(),
			h.getLoad()
			);
	}

	for( j = 0; j < 10; j++ )
	{
		printf("\"%s\" = %d\n", names[j], (int)h[names[j]] );
	}

	printf("\nDeleting some...\n");

	for( int k = 0; k < 7; k++ )
	{
		h.del( names[k] );
		//h.insert( names[j], (void *)(j+1) );
		printf("Capacity: %lu, Size: %lu, Load: %f\n",
			h.getCapacity(),
			h.getSize(),
			h.getLoad()
			);
	}

	printf("\nInserting more...\n");
	
	for( ; names[j] != NULL; j++ )
	{
		h.insert( names[j], (void *)(j+1) );
		printf("Capacity: %lu, Size: %lu, Load: %f\n",
			h.getCapacity(),
			h.getSize(),
			h.getLoad()
			);
	}
}
