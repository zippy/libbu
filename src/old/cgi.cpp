#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "cgi.h"

Cgi::Cgi( const char *strSource ) :
	aContent( new HashFunctionString(), 151, true )
{
    int length, j, k, mode = 0, slen = 0;
    char hexbuf[3] = { 0, 0, 0 };
    char *buf, chr;
    Item *cur = NULL;
    int nCur = 0;

    if( strSource != NULL )
    {
		loadContent( strSource );
	}

    if( ( getenv( "CONTENT_LENGTH" ) ) )
    {
        if( !strcmp
            ( getenv( "CONTENT_TYPE" ),
              "application/x-www-form-urlencoded" ) )
        {
            length = atoi( getenv( "CONTENT_LENGTH" ) );
            buf = new char[length + 1];
            fread( buf, 1, length, stdin );
            cur = new Item(  );
            aVars.append( cur );
            cur->type = VAR_STDINPUT;
            for( j = 0; j < length; j++ )
            {
                switch ( buf[j] )
                {
                case '=':
                    cur->name = new char[slen + 1];
                    slen = 0;
                    break;

                case '&':
                    cur->value = new char[slen + 1];
                    cur->len = slen;
                    slen = 0;
                    cur = new Item(  );
                    aVars.append( cur );
                    cur->type = VAR_STDINPUT;
                    break;

                default:
                    switch ( buf[j] )
                    {
                    case '%':  /* per-cents mean a hex-code for an ASCII char */
                        j += 2;
                        slen++;
                        break;

                    default:   /* Nothing special, move along, folks... */
                        slen++;
                        break;
                    }
                    break;
                }
            }
            cur->value = new char[slen + 1];
            cur->len = slen;
            slen = 0;
            mode = 0;
            cur = ( Item * ) aVars.getAt( 0 );
            k = 0;
            nCur = 0;
            for( j = 0; j < length; j++ )
            {
                switch ( buf[j] )
                {
                case '=':
                    mode = 1;
                    k = 0;
                    break;

                case '&':
                    mode = 0;
                    k = 0;
                    nCur++;
                    cur = ( Item * ) aVars.getAt( nCur );
                    break;

                default:
                    switch ( buf[j] )
                    {
                    case '%':  /* per-cents mean a hex-code for an ASCII char */
                        hexbuf[0] = buf[++j];
                        hexbuf[1] = buf[++j];
                        chr = ( char ) ( strtol( hexbuf, NULL, 16 ) );
                        break;

                    case '+':  /* Pluses mean spaces, odd, I know... */
                        chr = ' ';
                        break;

                    default:   /* Nothing special, move along, folks... */
                        chr = buf[j];
                        break;
                    }
                    if( mode == 0 )
                    {
                        cur->name[k] = chr;
                        cur->name[++k] = '\0';
                    }
                    else
                    {
                        cur->value[k] = chr;
                        cur->value[++k] = '\0';
                    }
                    break;
                }
            }
            delete buf;
        }
        else if( !strncmp
                 ( getenv( "CONTENT_TYPE" ), "multipart/form-data;", 20 ) )
        {
            char *boundary, *oname;
            int blen, j, k, olen;

            length = atoi( getenv( "CONTENT_LENGTH" ) );
            buf = new char[length + 1];
            fread( buf, 1, length, stdin );
            for( blen = 0; buf[blen + 1] != '\n'; blen++ );
            boundary = new char[blen + 1];
            memcpy( boundary, buf, blen );
            boundary[blen] = '\0';
            j = blen + 2;
            for( ;; )
            {
                cur = new Item(  );
                aVars.append( cur );
                cur->type = VAR_STDINPUT;
                if( !strncmp
                    ( buf + j, "Content-Disposition: form-data; name=\"",
                      38 ) )
                {
                    j += 38;
                    for( k = 0; buf[j + k] != '\"'; k++ );
                    oname = cur->name = new char[k + 1];
                    memcpy( cur->name, buf + j, k );
                    olen = k;
                    cur->name[k] = '\0';
                    j += k + 1;
                    if( !strncmp( buf + j, "; filename=\"", 12 ) )      /* Must be a file */
                    {
                        /* Acquire file name */
                        j += 12;
                        for( k = 0; buf[j + k] != '\"'; k++ );
                        cur->value = new char[k + 1];
                        memcpy( cur->value, buf + j, k );
                        cur->value[k] = '\0';
                        cur->len = k;
                        j += k + 3;

                        /* Acquire content type */
                        if( !strncmp( "Content-Type: ", buf + j, 14 ) )
                        {
                            j += 14;
                            cur = new Item(  );
                            aVars.append( cur );
                            cur->type = VAR_STDINPUT;
                            cur->name = new char[olen + 1];
                            memcpy( cur->name, oname, olen + 1 );
                            for( k = 0; buf[j + k + 1] != '\n'; k++ );
                            cur->value = new char[k + 1];
                            memcpy( cur->value, buf + j, k );
                            cur->value[k] = '\0';
                            cur->len = k;
                            j += k;
                        }
                        else
                        {
                            cur = new Item(  );
                            aVars.append( cur );
                            cur->type = VAR_STDINPUT;
                            cur->name = new char[olen + 1];
                            memcpy( cur->name, oname, olen + 1 );
                            cur->value = new char[1];
                            cur->value[0] = '\0';
                            cur->len = 0;
                        }
                        j += 4;

                        /* Acquire content */
                        cur = new Item(  );
                        aVars.append( cur );
                        cur->type = VAR_STDINPUT;
                        cur->name = new char[olen + 1];
                        memcpy( cur->name, oname, olen + 1 );
                        if( !strncmp( buf + j + k, boundary, blen ) )
                        {
                            cur->value = new char[1];
                            cur->value[0] = '\0';
                            j += blen + 4;
                        }
                        else if( !strncmp( buf + j + k + 1, boundary, blen ) )
                        {
                            cur->value = new char[1];
                            cur->value[0] = '\0';
                            j += blen + 5;
                        }
                        else
                        {
                            for( k = 0;
                                 strncmp( buf + j + k + 2, boundary, blen );
                                 k++ );
                            cur->value = new char[k + 1];
                            memcpy( cur->value, buf + j, k );
                            cur->value[k] = '\0';
                            cur->len = k;
                            j += k + blen + 4;
                        }
                    }
                    else
                    {
                        j += 4;
                        for( k = 0;
                             strncmp( buf + j + k + 2, boundary, blen );
                             k++ );
                        cur->value = new char[k + 1];
                        memcpy( cur->value, buf + j, k );
                        cur->value[k] = '\0';
                        cur->len = k;
                        j += k + blen + 4;
                    }
                    if( buf[j + 1] == '\n' )
                        j += 2;
                    if( j >= length )
                        break;
                }
                else
                {
                    cur->name = ( char * ) "ERROR";
                    cur->value = ( char * ) "Error here";
                }
            }
        }
        delete buf;
    }

    if( ( buf = getenv( "HTTP_COOKIE" ) ) )
    {
        int lbase = aVars.getSize(  );
        length = strlen( buf );
        cur = new Item(  );
        aVars.append( cur );
        cur->type = VAR_COOKIE;
        for( j = 0; j < length; j++ )
        {
            switch ( buf[j] )
            {
            case '=':
                cur->name = new char[slen + 1];
                slen = 0;
                break;

            case ';':
                cur->value = new char[slen + 1];
                cur->len = slen;
                slen = 0;
                cur = new Item(  );
                aVars.append( cur );
                cur->type = VAR_COOKIE;
                break;

            default:
                switch ( buf[j] )
                {
                case '%':      /* per-cents mean a hex-code for an ASCII char */
                    j += 2;
                    slen++;
                    break;

                default:       /* Nothing special, move along, folks... */
                    slen++;
                    break;
                }
                break;
            }
        }
        cur->value = new char[slen + 1];
        cur->len = slen;
        slen = 0;
        cur = ( Item * ) aVars.getAt( lbase );
        mode = 0;
        k = 0;
        nCur = lbase;
        for( j = 0; j < length; j++ )
        {
            switch ( buf[j] )
            {
            case '=':
                mode = 1;
                k = 0;
                break;

            case ';':
                mode = 0;
                k = 0;
                nCur++;
                cur = ( Item * ) aVars.getAt( nCur );
                break;

            default:
                switch ( buf[j] )
                {
                case '%':      /* per-cents mean a hex-code for an ASCII char */
                    hexbuf[0] = buf[++j];
                    hexbuf[1] = buf[++j];
                    chr = ( char ) ( strtol( hexbuf, NULL, 16 ) );
                    break;

                case '+':      /* Pluses mean spaces, odd, I know... */
                    chr = ' ';
                    break;

                case ' ':
                    continue;
                    break;

                default:       /* Nothing special, move along, folks... */
                    chr = buf[j];
                    break;
                }
                if( mode == 0 )
                {
                    cur->name[k] = chr;
                    cur->name[++k] = '\0';
                }
                else
                {
                    cur->value[k] = chr;
                    cur->value[++k] = '\0';
                }
                break;
            }
        }
    }

    if( ( buf = getenv( "QUERY_STRING" ) ) )
    {
        if( strlen( buf ) > 0 )
        {
            int lbase = aVars.getSize(  );
            length = strlen( buf );
            cur = new Item(  );
            aVars.append( cur );
            cur->type = VAR_CMDLINE;
            for( j = 0; j < length; j++ )
            {
                switch ( buf[j] )
                {
                case '=':
                    cur->name = new char[slen + 1];
                    slen = 0;
                    break;

                case '&':
                    cur->value = new char[slen + 1];
                    cur->len = slen;
                    slen = 0;
                    cur = new Item(  );
                    aVars.append( cur );
                    cur->type = VAR_CMDLINE;
                    break;

                default:
                    switch ( buf[j] )
                    {
                    case '%':  /* per-cents mean a hex-code for an ASCII char */
                        j += 2;
                        slen++;
                        break;

                    default:   /* Nothing special, move along, folks... */
                        slen++;
                        break;
                    }
                    break;
                }
            }
            cur->value = new char[slen + 1];
            cur->len = slen;
            slen = 0;
            cur = ( Item * ) aVars.getAt( lbase );
            nCur = lbase;
            mode = 0;
            k = 0;
            for( j = 0; j < length; j++ )
            {
                switch ( buf[j] )
                {
                case '=':
                    mode = 1;
                    k = 0;
                    break;

                case '&':
                    mode = 0;
                    k = 0;
                    nCur++;
                    cur = ( Item * ) aVars.getAt( nCur );
                    break;

                default:
                    switch ( buf[j] )
                    {
                    case '%':  /* per-cents mean a hex-code for an ASCII char */
                        hexbuf[0] = buf[++j];
                        hexbuf[1] = buf[++j];
                        chr = ( char ) ( strtol( hexbuf, NULL, 16 ) );
                        break;

                    case '+':  /* Pluses mean spaces, odd, I know... */
                        chr = ' ';
                        break;

                    default:   /* Nothing special, move along, folks... */
                        chr = buf[j];
                        break;
                    }
                    if( mode == 0 )
                    {
                        cur->name[k] = chr;
                        cur->name[++k] = '\0';
                    }
                    else
                    {
                        cur->value[k] = chr;
                        cur->value[++k] = '\0';
                    }
                    break;
                }
            }
        }
    }
}

Cgi::~Cgi(  )
{
}

char *Cgi::getVarValue( const char *name, int skip, unsigned char type )
{
    for( int j = 0; j < aVars.getSize(  ); j++ )
    {
        Item *cur = ( Item * ) aVars.getAt( j );
        if( !strcmp( cur->name, name ) )
        {
            if( ( cur->type & type ) )
            {
                if( skip <= 0 )
                {
                    return cur->value;
                }
                else
                {
                    skip--;
                }
            }
        }
    }
    return NULL;
}

int Cgi::getVarLength( const char *name, int skip, unsigned char type )
{
    for( int j = 0; j < aVars.getSize(  ); j++ )
    {
        Item *cur = ( Item * ) aVars.getAt( j );
        if( !strcmp( cur->name, name ) )
        {
            if( ( cur->type & type ) )
            {
                if( skip <= 0 )
                {
                    return cur->len;
                }
                else
                {
                    skip--;
                }
            }
        }
    }
    return -1;
}

void Cgi::writeDebugInfo()
{
    printf( "<pre>\n" );
    printf( "0x%02X - stdInput | 0x%02X - cookie | 0x%02X - cmdLine\n\n",
             VAR_STDINPUT, VAR_COOKIE, VAR_CMDLINE );
    for( int j = 0; j < aVars.getSize(  ); j++ )
    {
        Item *item = ( Item * ) aVars.getAt( j );
        printf("[%s] = \"%s\" [0x%02X]\n", item->name,
                 item->value, item->type );
    }
    printf( "</pre>\n" );
}

void Cgi::writeContentHeader( int type )
{
	switch( type )
	{
	case headerHTML:
		printf("Content-type: text/html\n\n");
		break;
	}
}

void Cgi::writeContent( const char *name, ...)
{
	char *templ = (char *)aContent.get(name);

	if( templ )
	{
		va_list ap;

		va_start (ap, name);
		vprintf (templ, ap);
		va_end (ap);
	}
	else
	{
		printf("Error finding content labeled \"%s\"\n", name );
	}
}

void Cgi::loadContent( const char *strSource )
{
	FILE *fh = NULL;
	if( strSource == NULL )
	{
		extern char *program_invocation_short_name;
		char *tmpName = new char[strlen(program_invocation_short_name)+10];
		memset( tmpName, 0, strlen(program_invocation_short_name)+10 );
		strcpy( tmpName, program_invocation_short_name );
		strcat( tmpName, ".content" );
		fh = fopen( tmpName, "rt" );
		delete tmpName;
	}
	else
	{
		fh = fopen( strSource, "rt" );
	}

	if( fh == NULL ) return;

	struct stat xStats;

	fstat( fileno( fh ), &xStats );

	char *bigBuf = new char[xStats.st_size+1];
	memset( bigBuf, 0, xStats.st_size+1 );
	fread( bigBuf, 1, xStats.st_size, fh );
	fclose( fh );

	// Now we can actually load stuff from the file, first we need to make us up a format...
	int lSize=0;
	struct Content
	{
		char *name;
		char *value;
	} xCont;
	int j = 0;
	while( j < xStats.st_size )
	{
		// We're looking for a content-block init statement
		for( ; j < xStats.st_size; j++ )
		{
			if( bigBuf[j] == '#' )
			{
				if( bigBuf[j+1] == '{' )
				{
					break;
				}
			}
		}
		j=j+2;
		if( j >= xStats.st_size ) break;
		for( ; bigBuf[j] == ' ' || bigBuf[j] == '\t'; j++ );
		for( lSize = 0; lSize+j < xStats.st_size && bigBuf[lSize+j] != '\n' && bigBuf[lSize+j] != '\r'; lSize++ );
		xCont.name = new char[lSize+1];
		memset( xCont.name, 0, lSize+1 );
		memcpy( xCont.name, &bigBuf[j], lSize );
		j += lSize+1;

		for( lSize = 0; lSize+j < xStats.st_size; lSize++ )
		{
			if( bigBuf[lSize+j] == '#' )
			{
				if( bigBuf[lSize+j+1] == '}' )
				{
					break;
				}
			}
		}
		xCont.value = new char[lSize+1];
		memset( xCont.value, 0, lSize+1 );
		memcpy( xCont.value, &bigBuf[j], lSize );

		aContent.insert( xCont.name, xCont.value );

		j += lSize + 2;
	}
}

void Cgi::writeCookie( char const *name, char const *value, char const *expires, char const *path, char const *domain, bool secure )
{
	printf("Set-Cookie: %s=%s", name, value );

	if( expires != NULL )
	{
		printf("; expires=%s", expires );
	}

	if( path != NULL )
	{
		printf("; path=%s", path );
	}

	if( domain != NULL )
	{
		printf("; domain=%s", domain );
	}

	if( secure )
	{
		printf("; secure");
	}

	printf("\n");
}
