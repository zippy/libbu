#include <stdio.h>
#include "pproc.h"

int main( int argc, char *argv[] )
{
	bool bOn = false;
	bool bOff = true;
	bool bTog = false;
	char cChar = '?';
	PPROC table[] = {
		{ "boolon",		'n', PPROC_BOOL_TRUE,	NULL,	&bOn,
				"Set the bool on." },
		{ "booloff",	'f', PPROC_BOOL_FALSE,	NULL,	&bOff,
				"Set the bool off." },
		{ "booltog",	't', PPROC_BOOL_TOGGLE,	NULL,	&bTog,
				"Set the bool off." },
		{ "char",		'c', PPROC_CHAR,		NULL,	&cChar,
				"Set the char." },
		{ NULL,			'\0',0, 				NULL, 	NULL,
				NULL }
	};

	processParams( argc, argv, table );

	printf("Final results:\n");
	printf("\tbOn   = %s\n", (bOn ? "true" : "false") );
	printf("\tbOff  = %s\n", (bOff ? "true" : "false") );
	printf("\tbTog  = %s\n", (bTog ? "true" : "false") );
	printf("\tcChar = '%c'\n", cChar );

	return 0;
}

