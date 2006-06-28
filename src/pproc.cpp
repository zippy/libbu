#include <stdio.h>
#include <string.h>
#include "pproc.h"

void pprocHelp( PPROC *pproc )
{
	int maxlen = 0;
	for( int j = 0; pproc[j].proc || pproc[j].stateVar; j++ )
	{
		int len = strlen( pproc[j].lpWord );
		if( len > maxlen ) maxlen = len;
	}

	char fmt[100];
	sprintf( fmt, "  -%%c, --%%-%ds  %%s\n", maxlen );

	for( int j = 0; pproc[j].proc || pproc[j].stateVar; j++ )
	{
		printf( fmt,
			pproc[j].cChar,
			pproc[j].lpWord,
			pproc[j].shortHelp );
	}
}

void grabParamAsData( PPROC *pproc, char *str, int *aind, int *cind )
{
	switch( pproc->cMode & PPROC_TYPE )
	{
		case PPROC_BOOL_TRUE:
			*((bool *)pproc->stateVar) = true;
			break;
			
		case PPROC_BOOL_FALSE:
			*((bool *)pproc->stateVar) = false;
			break;
			
		case PPROC_BOOL_TOGGLE:
			*((bool *)pproc->stateVar) = !(*((bool *)pproc->stateVar));
			break;
			
		case PPROC_CHAR:
			(*cind)++;
			*((char *)pproc->stateVar) = str[(*cind)];
			break;
			
		case PPROC_SHORT:
			break;
			
		case PPROC_LONG:
			break;
			
		case PPREC_LONG_LONG:
			break;
			
		case PPROC_UCHAR:
			break;
			
		case PPROC_USHORT:
			break;
			
		case PPROC_ULONG:
			break;
			
		case PPREC_ULONG_LONG:
			break;
			
		case PPROC_FLOAT:
			break;
			
		case PPROC_DOUBLE:
			break;
			
		case PPROC_LONG_DOUBLE:
			break;
			
		case PPROC_STRING:
			strcpy( (char *)(pproc->stateVar), str );
			(*aind)++;
			break;
	}
}

void processParams( int argc, char *argv[], PPROC *pproc )
{
	bool bUsed;
	// Loop over all the params except the first, no params, no looping!
	for( int j = 1; j < argc; j++ )
	{
		//printf("Param[%d]: \"%s\"\n", j, argv[j] );
		if( argv[j][0] == '-' )
		{
			if( argv[j][1] == '-' )
			{
				bUsed = false;
				// Proccess a long-word param string
				for( int k = 0;
					pproc[k].proc != NULL || pproc[k].stateVar != NULL;
					k++ )
				{
					if( !strcmp( pproc[k].lpWord, &argv[j][2] ) )
					{
						bUsed = true;
						if( pproc[k].proc != NULL )
						{
							j += pproc[k].proc( argc-j, &argv[j] );
						}
						if( pproc[k].stateVar != NULL )
						{
							grabParamAsData( &pproc[k], argv[j+1], &j, &k );
						}
						break;
					}
				}
				if( !bUsed )
				{
					if( !strcmp( "help", &argv[j][2] ) )
					{
						pprocHelp( pproc );
					}
				}
			}
			else
			{
				bUsed = false;
				// Process a one char param string
				for( int k = 0;
					pproc[k].proc != NULL || pproc[k].stateVar != NULL;
					k++ )
				{
					if( pproc[k].cChar == argv[j][1] )
					{
						bUsed = true;
						if( pproc[k].proc != NULL )
						{
							j += pproc[k].proc( argc-j, &argv[j] );
						}
						if( pproc[k].stateVar != NULL )
						{
							int tmp = 1;
							if( argv[j][2] == '\0' )
							{
								grabParamAsData( &pproc[k], argv[j+1], &j, &tmp );
							}
							else
							{
								j--;
								grabParamAsData( &pproc[k], (&argv[j+1][2]), &j, &tmp );
							}
						}
						break;
					}
				}
				if( !bUsed )
				{
					if( argv[j][1] == 'h' )
					{
						pprocHelp( pproc );
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

