/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/url.h"
#ifndef WIN32
# include <netdb.h>
# include <netinet/in.h>
#endif
#include <stdlib.h>

char Bu::Url::hexcode[] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

Bu::Url::Url()
{
}

Bu::Url::Url( const Bu::FString &sUrl )
{
	parseUrl( sUrl );
}

Bu::Url::~Url()
{
}

void Bu::Url::parseUrl( const Bu::FString &sUrl )
{
	clear();

	Bu::FString::const_iterator i = sUrl.begin();
	parseProtocol( i );
	parseUserPass( i );
	parseHost( i );
	parsePath( i );
}

Bu::FString Bu::Url::decode( const Bu::FString &sStr )
{
	Bu::FString sRet;
	char buf[3] = {0, 0, 0};
	for( Bu::FString::const_iterator i = sStr.begin(); i; i++ )
	{
		if( *i == '+' )
		{
			sRet += ' ';
		}
		else if( *i == '%' )
		{
			i++;
			buf[0] = *i;
			i++;
			buf[1] = *i;
			sRet += (char)((unsigned char)strtol( buf, NULL, 16 ));
		}
		else
		{
			sRet += *i;
		}
	}
	return sRet;
}

Bu::FString Bu::Url::encode( const Bu::FString &sStr )
{
	Bu::FString sRet;
	for( Bu::FString::const_iterator i = sStr.begin(); i; i++ )
	{
		if( *i == ' ' )
		{
			sRet += '+';
		}
		else if(
			(*i >= 'A' && *i <= 'Z') ||
			(*i >= 'a' && *i <= 'z') ||
			(*i >= '0' && *i <= '9') ||
			(*i == '-' || *i == '_' || *i == '.' || *i == '~')
			)
		{
			sRet += *i;
		}
		else
		{
			unsigned char b = *i;
			sRet += '%';
			sRet += hexcode[(b>>4)&0xF];
			sRet += hexcode[b&0xF];
		}
	}
	return sRet;
}

void Bu::Url::parseProtocol( Bu::FString::const_iterator &i )
{
	Bu::FString::const_iterator s = i.find("://", 3);
	if( !s )
		throw Bu::ExceptionBase("No :// in url");
	Bu::FString sTmp( i, s );
	setProtocol( sTmp );
	i = s + 3;
}

void Bu::Url::setProtocol( const Bu::FString &sNewProto, bool bAutoSetPort )
{
	sProtocol = sNewProto;
#ifndef WIN32
	if( bAutoSetPort )
	{
		struct servent *se = getservbyname( sProtocol.getStr(), "tcp" );
		if( se )
		{
			iPort = ntohs( se->s_port );
		}
	}
#endif
}

void Bu::Url::parseUserPass( Bu::FString::const_iterator &i )
{
	Bu::FString::const_iterator s = i.find('@');
	if( !s )
		return;

	Bu::FString::const_iterator p = i.find(':');
	if( p )
	{
		sUser.set( i, p );
		sPass.set( p+1, s );
	}
	else
	{
		sUser.set( i, s );
	}

	i = s + 1;
}

void Bu::Url::parseHost( Bu::FString::const_iterator &i )
{
	Bu::FString::const_iterator s = i;
	for( ; s && *s != '/'; s++ )
	{
		if( *s == ':' )
		{
			sHost.set( i, s );
			i = s + 1;
			s = i.find('/');
			Bu::FString sPort( i, s );
			iPort = strtol( sPort.getStr(), NULL, 10 );
			i = s;
			return;
		}
	}
	sHost.set( i, s );
	i = s;
}

void Bu::Url::parsePath( const Bu::FString &sPath )
{
	Bu::FString::const_iterator i = sPath.begin();
	parsePath( i );
}

void Bu::Url::parsePath( Bu::FString::const_iterator &i )
{
	if( i )
	{
		Bu::FString::const_iterator s = i.find('?');
		sPath.set( i, s );
		i = s + 1;
		if( s )
		{
			parseParams( i );
		}
	}
	else
	{
		sPath = "/";
	}
}

void Bu::Url::parseParams( const Bu::FString &sQuery )
{
	Bu::FString::const_iterator i = sQuery.begin();
	parseParams( i );
}

void Bu::Url::parseParams( Bu::FString::const_iterator &i )
{
	bool bName = true;
	Bu::FString sName, sValue;
	for( Bu::FString::const_iterator s = i; s; s++ )
	{
		if( bName )
		{
			if( *s == '&' )
			{
				sName.set( i, s );
				sValue.clear();
				i = s + 1;
				addParam( decode( sName ), decode( sValue ) );
			}
			else if( *s == '=' )
			{
				sName.set( i, s );
				i = s + 1;
				bName = false;
			}
		}
		else
		{
			if( *s == '&' )
			{
				sValue.set( i, s );
				i = s + 1;
				bName = true;
				addParam( decode( sName ), decode( sValue ) );
			}
		}
	}
	if( i )
	{
		if( bName )
		{
			sName.set( i );
			sValue.clear();
		}
		else
		{
			sValue.set( i );
		}
		addParam( decode( sName ), decode( sValue ) );
	}
}

void Bu::Url::addParam( const Bu::FString &n, const Bu::FString &v )
{
	lParam.append( Param( n, v ) );
}

void Bu::Url::clear()
{
	sProtocol.clear();
	sUser.clear();
	sPass.clear();
	sHost.clear();
	sPath.clear();
	iPort.clear();
}

Bu::FString Bu::Url::getFullPath() const
{
	Bu::FString sBuf = sPath;
	if( !lParam.isEmpty() )
	{
		for( ParamList::const_iterator i = lParam.begin(); i; i++ )
		{
			if( i == lParam.begin() )
				sBuf += "?";
			else
				sBuf += "&";

			sBuf += encode( (*i).sName );
			if( !(*i).sValue.isEmpty() )
			{
				sBuf += "=" + encode( (*i).sValue );
			}
		}
	}

	return sBuf;
}

Bu::FString Bu::Url::getUrl() const
{
	Bu::FString sBuf = sProtocol + "://" + sHost + getFullPath();
	return sBuf;
}

