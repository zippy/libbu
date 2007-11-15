/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "paramproc.h"
#include <stdio.h>

#define ptrtype( iitype, iiname ) \
	Bu::ParamProc::ParamPtr::ParamPtr( iitype *iiname ) :	\
		type( vt ##iiname ) { val.iiname = iiname; }

Bu::ParamProc::ParamPtr::ParamPtr()
{
	val.str = NULL;
	type = vtunset;
}

ptrtype( Bu::FString, str     );
ptrtype( uint64_t,    uint64  );
ptrtype( uint32_t,    uint32  );
ptrtype( uint16_t,    uint16  );
ptrtype( uint8_t,     uint8   );
ptrtype( int64_t,     int64   );
ptrtype( int32_t,     int32   );
ptrtype( int16_t,     int16   );
ptrtype( int8_t,      int8    );
ptrtype( float,       float32 );
ptrtype( double,      float64 );
ptrtype( long double, float96 );
ptrtype( bool,        bln     );

Bu::ParamProc::ParamPtr &Bu::ParamProc::ParamPtr::operator=( ParamProc::ParamPtr &ptr )
{
	val = ptr.val;
	type = ptr.type;

	return *this;
}

bool Bu::ParamProc::ParamPtr::isSet()
{
	return type != vtunset;
}

Bu::ParamProc::ParamPtr &Bu::ParamProc::ParamPtr::operator=( const char *str )
{
	if( !isSet() ) return *this;
	switch( type )
	{
		case vtstr:
			(*val.str) = str;
			break;

		case vtuint64:
			(*val.uint64) = strtoull( str, NULL, 10 );
			break;

		case vtuint32:
			(*val.uint32) = strtoul( str, NULL, 10 );
			break;

		case vtuint16:
			(*val.uint16) = (uint16_t)strtoul( str, NULL, 10 );
			break;

		case vtuint8:
			(*val.uint8) = (uint8_t)strtoul( str, NULL, 10 );
			break;

		case vtint64:
			(*val.int64) = strtoll( str, NULL, 10 );
			break;

		case vtint32:
			(*val.int32) = strtol( str, NULL, 10 );
			break;

		case vtint16:
			(*val.int16) = (int16_t)strtol( str, NULL, 10 );
			break;

		case vtint8:
			(*val.int8) = (int8_t)strtol( str, NULL, 10 );
			break;

		case vtfloat32:
			(*val.float32) = strtof( str, NULL );
			break;

		case vtfloat64:
			(*val.float64) = strtod( str, NULL );
			break;

		case vtfloat96:
			(*val.float96) = strtold( str, NULL );
			break;

		case vtbln:
			if( strcasecmp("yes", str ) == 0 ||
				strcasecmp("true", str ) == 0 )
			{
				(*val.bln) = true;
			}
			else
			{
				(*val.bln) = false;
			}
			break;
	}

	return *this;
}

Bu::ParamProc::ParamProc()
{
}

Bu::ParamProc::~ParamProc()
{
	for( std::list<ArgSpec *>::iterator i = lArg.begin();
		 i != lArg.end(); i++ )
	{
		delete *i;
	}

	for( std::list<Banner *>::iterator i = lBan.begin();
		 i != lBan.end(); i++ )
	{
		delete *i;
	}

}
/*
void Bu::ParamProc::addParam( const char *lpWord, char cChar, Proc proc, ParamPtr val )
{
	printf("Calling callback...\n");
	val = "Hello there, this is set in the ParamProc";
	(this->*proc)();
}*/

void Bu::ParamProc::addParam( const char *lpWord, char cChar, Proc proc,
	ParamPtr val, const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	ArgSpec *as = new ArgSpec;
	if( lpWord )
		as->sWord = lpWord;

	as->cChar = cChar;
	as->proc = proc;
	as->val = val;
	if( lpDesc )
		as->sDesc = lpDesc;
	if( lpExtra )
		as->sExtra = lpExtra;
	if( lpValue )
		as->sValue = lpValue;

	lArg.push_back( as );

	if( !lBan.empty() )
	{
		if( lBan.back()->pBefore == NULL )
			lBan.back()->pBefore = as;
	}
}

void Bu::ParamProc::addParam( const char *lpWord, char cChar, Proc proc,
	const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	addParam( lpWord, cChar, proc, ParamPtr(), lpDesc, lpExtra, lpValue );
}

void Bu::ParamProc::addParam( const char *lpWord, char cChar, ParamPtr val,
	const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	addParam( lpWord, cChar, NULL, val, lpDesc, lpExtra, lpValue );
}

void Bu::ParamProc::addParam( const char *lpWord, Proc proc, ParamPtr val,
	const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	addParam( lpWord, '\0', proc, val, lpDesc, lpExtra, lpValue );
}

void Bu::ParamProc::addParam( const char *lpWord, Proc proc,
	const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	addParam( lpWord, '\0', proc, ParamPtr(), lpDesc, lpExtra, lpValue );
}

void Bu::ParamProc::addParam( const char *lpWord, ParamPtr val,
	const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	addParam( lpWord, '\0', NULL, val, lpDesc, lpExtra, lpValue );
}

void Bu::ParamProc::addParam( char cChar, Proc proc, ParamPtr val,
	const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	addParam( NULL, cChar, proc, val, lpDesc, lpExtra, lpValue );
}

void Bu::ParamProc::addParam( char cChar, Proc proc,
	const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	addParam( NULL, cChar, proc, ParamPtr(), lpDesc, lpExtra, lpValue );
}

void Bu::ParamProc::addParam( char cChar, ParamPtr val,
	const char *lpDesc, const char *lpExtra,
	const char *lpValue )
{
	addParam( NULL, cChar, NULL, val, lpDesc, lpExtra, lpValue );
}

void Bu::ParamProc::process( int argc, char *argv[] )
{
	for( int arg = 1; arg < argc; arg++ )
	{
		//printf(":::%d:::%s\n", arg, argv[arg] );
		if( argv[arg][0] == '-' )
		{
			if( argv[arg][1] == '-' )
			{
				ArgSpec *s = checkWord( argv[arg]+2 );
				if( s )
				{
					if( argv[arg][s->sWord.getSize()+2] == '=' )
					{
						if( s->val.isSet() )
						{
							if( s->sValue.getStr() == NULL )
							{
								s->val = argv[arg]+s->sWord.getSize()+3;
							}
							else
							{
								s->val = s->sValue.getStr();
							}
						}
						if( s->proc )
						{
							char **tmp = new char*[argc-arg];
							tmp[0] = argv[arg]+s->sWord.getSize()+3;
							for( int k = 1; k < argc-arg; k++ )
								tmp[k] = argv[arg+k];
							int ret = (this->*s->proc)( argc-arg, tmp );
							if( ret > 0 )
							{
								arg += ret-1;
							}
							delete tmp;
						}
					}
					else
					{
						int add = 0;
						if( s->val.isSet() )
						{
							if( s->sValue.getStr() == NULL )
							{
								if( arg+1 >= argc )
								{
									return;
								}
								s->val = argv[arg+1];
								add++;
							}
							else
							{
								s->val = s->sValue.getStr();
							}
						}
						if( s->proc )
						{
							int ret = (this->*s->proc)(
								argc-arg-1, argv+arg+1 );

							if( ret > add )
								add = 0;
							else
								add -= ret;
							arg += ret;
						}
						arg += add;
					}
					continue;
				}
				else
				{
					unknownParam( argc-arg, argv+arg );
				}
			}
			else
			{
				for( int chr = 1; argv[arg][chr]; chr++ )
				{
					ArgSpec *s = checkLetr( argv[arg][chr] );
					if( s )
					{
						if( argv[arg][chr+1] != '\0' )
						{
							bool bUsed = false;
							if( s->val.isSet() )
							{
								if( s->sValue.getStr() == NULL )
								{
									s->val = argv[arg]+chr+1;
									bUsed = true;
								}
								else
								{
									s->val = s->sValue.getStr();
								}
							}
							if( s->proc )
							{
								char **tmp = new char*[argc-arg];
								tmp[0] = argv[arg]+chr+1;
								for( int k = 1; k < argc-arg; k++ )
									tmp[k] = argv[arg+k];
								int ret = (this->*s->proc)( argc-arg, tmp );
								if( ret > 0 )
								{
									arg += ret - 1;
									delete tmp;
									break;
								}
								delete tmp;
							}
							if( bUsed )
							{
								break;
							}
						}
						else
						{
							bool bUsed = false;
							if( s->val.isSet() )
							{
								if( s->sValue.getStr() == NULL )
								{
									s->val = argv[arg+1];
									bUsed = true;
								}
								else
								{
									s->val = s->sValue.getStr();
								}
							}
							if( s->proc )
							{
								int ret = (this->*s->proc)(
									argc-arg-1, argv+arg+1
									);
								if( ret > 0 )
								{
									arg += ret;
									break;
								}
							}
							if( bUsed )
							{
								arg++;
								break;
							}
						}
					}
					else
					{
						unknownParam( argc-arg, argv+arg );
					}
				}
			}
		}
		else
		{
			cmdParam( argc-arg, argv+arg );
		}
	}
}

Bu::ParamProc::ArgSpec *Bu::ParamProc::checkWord( const char *arg )
{
	//printf("Checking \"%s\"...\n", arg );
	std::list<ArgSpec *>::const_iterator i;
	for( i = lArg.begin(); i != lArg.end(); i++ )
	{
		if( (*i)->sWord.getStr() == NULL )
			continue;

		if( !strcmp( (*i)->sWord.getStr(), arg ) )
			return *i;

		if( (*i)->val.isSet() )
		{
			if( !strncmp( (*i)->sWord.getStr(), arg, (*i)->sWord.getSize() ) &&
				arg[(*i)->sWord.getSize()] == '=' )
			{
				return *i;
			}
		}
	}

	return NULL;
}

Bu::ParamProc::ArgSpec *Bu::ParamProc::checkLetr( const char arg )
{
	//printf("Checking \'%c\'...\n", arg );
	std::list<ArgSpec *>::const_iterator i;
	for( i = lArg.begin(); i != lArg.end(); i++ )
	{
		if( (*i)->cChar == '\0' )
			continue;

		if( (*i)->cChar == arg )
		{
			return *i;
		}
	}

	return NULL;
}

int Bu::ParamProc::cmdParam( int argc, char *argv[] )
{
	printf("Unhandled command parameter \"%s\" found!\n", argv[0] );
	return 0;
}

int Bu::ParamProc::unknownParam( int argc, char *argv[] )
{
	printf("Unknown parameter \"%s\" found!\n", argv[0] );
	return 0;
}

int Bu::ParamProc::help( int argc, char *argv[] )
{
	std::list<Banner *>::const_iterator b = lBan.begin();
	std::list<ArgSpec *>::const_iterator i;
	int len=0;
	for( i = lArg.begin(); i != lArg.end(); i++ )
	{
		if( len < (*i)->sWord.getSize() + (*i)->sExtra.getSize() )
			len = (*i)->sWord.getSize() + (*i)->sExtra.getSize();
	}
	char fmt[10];
	sprintf( fmt, "%%-%ds   ", len );

	for( i = lArg.begin(); i != lArg.end(); i++ )
	{
		if( b != lBan.end() )
		{
			if( (*b)->pBefore == (*i) )
			{
				printf( (*b)->sBanner.getStr() );
				b++;
			}
		}
		printf("  ");
		if( (*i)->cChar )
		{
			if( (*i)->sWord.getStr() )
			{
				printf("-%c, ", (*i)->cChar );
			}
			else
			{
				printf("-%c  ", (*i)->cChar );
			}
		}
		else
		{
			printf("    ");
		}
		if( (*i)->sWord.getStr() )
		{
			printf("--");
			Bu::FString sTmp = (*i)->sWord.getStr();
			if( (*i)->sExtra.getStr() )
				sTmp += (*i)->sExtra.getStr();
			printf( fmt, sTmp.getStr() );
		}
		else
		{
			printf("  ");
			printf(fmt, "" );
		}
		printf("%s\n", (*i)->sDesc.getStr() );
	}
	if( b != lBan.end() )
	{
		if( (*b)->pBefore == NULL )
		{
			printf( (*b)->sBanner.getStr() );
		}
	}

	exit( 0 );
}

void Bu::ParamProc::addHelpBanner( const char *sHelpBanner )
{
	Banner *pBan = new Banner;
	pBan->sBanner = sHelpBanner;
	pBan->pBefore = NULL;
	lBan.push_back( pBan );
}

