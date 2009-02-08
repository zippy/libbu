#include "bu/url.h"

#include <stdio.h>

int main( int argc, char *argv[] )
{
	printf("encodede: %s\n", Bu::Url::encode( argv[1] ).getStr() );
	printf("decodede: %s\n", Bu::Url::decode( argv[1] ).getStr() );
	Bu::Url u( argv[1] );

	printf("Protocol: %s\n", u.getProtocol().getStr() );
	printf("User:     %s\n", u.getUser().getStr() );
	printf("Pass:     %s\n", u.getPass().getStr() );
	printf("Host:     %s\n", u.getHost().getStr() );
	printf("Path:     %s\n", u.getPath().getStr() );
	try
	{
		printf("Port:     %d\n", u.getPort() );
	} catch( Bu::ExceptionBase &e )
	{
		printf("Port:     not set.\n");
	}
	printf("Parameters:\n");
	for( Bu::Url::ParamList::const_iterator i = u.getParamBegin(); i; i++ )
	{
		printf("  \"%s\" = \"%s\"\n",
			(*i).sName.getStr(), (*i).sValue.getStr()
			);
	}

	return 0;
}
