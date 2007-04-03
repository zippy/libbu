#include "ordhash.h"
#include <string>

typedef struct eldef
{
	eldef( int a, int b, const std::string &c ) :
		id( a ), nSequence( b ), sName( c ) {}
	int id;
	int nSequence;
	std::string sName;
} eldef;

struct seqcmp
{
	bool operator()( eldef **a,  eldef **b )
	{
		return (*a)->nSequence < (*b)->nSequence;
	}
};

struct namcmp
{
	bool operator()( eldef **a,  eldef **b )
	{
		return (*a)->sName < (*b)->sName;
	}
};

typedef OrdHash<int, eldef, seqcmp> AHash;
//typedef OrdHash<int, eldef, namcmp> AHash;

int main()
{
	AHash hsh;
	hsh[1] = eldef( 0, 43, "Bob");
	hsh[4] = eldef( 1, 443, "Abby");
	hsh[2] = eldef( 2, 1, "Name");
	hsh[5] = eldef( 3, 0, "Catagory");
	hsh[32] = eldef( 4, 12, "Epilogue");

	for( AHash::iterator i = hsh.begin(); i != hsh.end(); i++ )
	{
		eldef e = (*i).second;
		printf("%d, %d, %s\n", e.id, e.nSequence, e.sName.c_str() );
	}

}

