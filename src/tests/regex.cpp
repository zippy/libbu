#include <bu/regex.h>
#include <stdio.h>

int main( int argc, char *argv[] )
{
	if( argc < 3 )
	{
		printf("No...  %s <regex> <string>\n\n", argv[0] );
		return 0;
	}

	Bu::RegEx re( argv[1] );

	printf("Regex:     %s\n", argv[1] );
	printf("Match:     %s\n", argv[2] );
	
	if( re.execute( argv[2] ) )
	{
		for( int j = 0; j < re.getNumSubStrings(); j++ )
		{
			printf("SubStr %d: %s\n", j, re.getSubString( j ).getStr() );
		}
	}
	else
	{
		printf("Regex did not match.\n");
	}

	return 0;
}

