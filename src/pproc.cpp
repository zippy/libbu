#include <stdio.h>
#include <string.h>
#include "pproc.h"

void processParams( int argc, char *argv[], PPROC *pproc )
{
	// Loop over all the params except the first, no params, no looping!
	for( int j = 1; j < argc; j++ )
	{
		//printf("Param[%d]: \"%s\"\n", j, argv[j] );
		if( argv[j][0] == '-' )
		{
			if( argv[j][1] == '-' )
			{
				// Proccess a long-word param string
				for( int k = 0;
					pproc[k].proc != NULL || pproc[k].stateVar != NULL;
					k++ )
				{
					if( !strcmp( pproc[k].lpWord, &argv[j][2] ) )
					{
						if( pproc[k].proc != NULL )
						{
							j += pproc[k].proc( argc-j, &argv[j] );
						}
						if( pproc[k].stateVar != NULL )
						{
							(*(pproc[k].stateVar)) = pproc[k].bSetState;
						}
					}
				}
			}
			else
			{
				// Process a one char param string
				for( int k = 0;
					pproc[k].proc != NULL || pproc[k].stateVar != NULL;
					k++ )
				{
					if( pproc[k].cChar == argv[j][1] )
					{
						if( pproc[k].proc != NULL )
						{
							j += pproc[k].proc( argc-j, &argv[j] );
						}
						if( pproc[k].stateVar != NULL )
						{
							(*(pproc[k].stateVar)) = pproc[k].bSetState;
						}
					}
				}
			}
		}
		else
		{
			// Handle generic params here.
		}
	}
}

