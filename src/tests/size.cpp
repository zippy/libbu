#include "bu/hash.h"
#include "bu/fstring.h"

#define pSize( t ) printf("%15s: %db\n", #t, sizeof( t ) );

int main()
{
	typedef Bu::Hash<char, char> charcharHash;
	typedef Bu::Hash<Bu::FString, Bu::FString> strstrHash;
	pSize( Bu::FString );
	pSize( charcharHash );
	pSize( strstrHash );
}
