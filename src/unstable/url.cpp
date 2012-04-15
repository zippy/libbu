/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
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

Bu::Url::Url( const Bu::String &sUrl )
{
	parseUrl( sUrl );
}

Bu::Url::~Url()
{
}

void Bu::Url::parseUrl( const Bu::String &sUrl )
{
	clear();

	Bu::String::const_iterator i = sUrl.begin();
	parseProtocol( i );
	parseUserPass( i );
	parseHost( i );
	parsePath( i );
}

Bu::String Bu::Url::decode( const Bu::String &sStr )
{
	Bu::String sRet;
	char buf[3] = {0, 0, 0};
	for( Bu::String::const_iterator i = sStr.begin(); i; i++ )
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

Bu::String Bu::Url::encode( const Bu::String &sStr )
{
	Bu::String sRet;
	for( Bu::String::const_iterator i = sStr.begin(); i; i++ )
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

void Bu::Url::parseProtocol( Bu::String::const_iterator &i )
{
	Bu::String::const_iterator s = i.find("://", 3);
	if( !s )
		throw Bu::ExceptionBase("No :// in url");
	Bu::String sTmp( i, s );
	setProtocol( sTmp );
	i = s + 3;
}

void Bu::Url::setProtocol( const Bu::String &sNewProto, bool bAutoSetPort )
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

void Bu::Url::parseUserPass( Bu::String::const_iterator &i )
{
	Bu::String::const_iterator s = i.find('@');
	if( !s )
		return;

	Bu::String::const_iterator p = i.find(':');
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

void Bu::Url::parseHost( Bu::String::const_iterator &i )
{
	Bu::String::const_iterator s = i;
	for( ; s && *s != '/'; s++ )
	{
		if( *s == ':' )
		{
			sHost.set( i, s );
			i = s + 1;
			s = i.find('/');
			Bu::String sPort( i, s );
			iPort = strtol( sPort.getStr(), NULL, 10 );
			i = s;
			return;
		}
	}
	sHost.set( i, s );
	i = s;
}

void Bu::Url::parsePath( const Bu::String &sPath )
{
	Bu::String::const_iterator i = sPath.begin();
	parsePath( i );
}

void Bu::Url::parsePath( Bu::String::const_iterator &i )
{
	if( i )
	{
		Bu::String::const_iterator s = i.find('?');
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

void Bu::Url::parseParams( const Bu::String &sQuery )
{
	Bu::String::const_iterator i = sQuery.begin();
	parseParams( i );
}

void Bu::Url::parseParams( Bu::String::const_iterator &i )
{
	bool bName = true;
	Bu::String sName, sValue;
	for( Bu::String::const_iterator s = i; s; s++ )
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

void Bu::Url::addParam( const Bu::String &n, const Bu::String &v )
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

Bu::String Bu::Url::getFullPath() const
{
	Bu::String sBuf = sPath;
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

Bu::String Bu::Url::getUrl() const
{
	Bu::String sBuf = sProtocol + "://" + sHost + getFullPath();
	return sBuf;
}

