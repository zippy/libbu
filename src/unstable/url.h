/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_URL_H
#define BU_URL_H

#include "bu/string.h"
#include "bu/atom.h"

namespace Bu
{
	class Url
	{
	public:
		typedef struct Param
		{
			Param() { }
			Param( const Param &r ) : sName( r.sName ), sValue( r.sValue ) { }
			Param( const Bu::String &n, const Bu::String &v ) :
				sName( n ), sValue( v ) { }
			Bu::String sName;
			Bu::String sValue;
		} Param;
		typedef Bu::List<Param> ParamList;

	public:
		Url();
		Url( const Bu::String &sUrl );
		virtual ~Url();

		void parseUrl( const Bu::String &sUrl );
		void parseParams( const Bu::String &sQuery );
		void parseParams( Bu::String::const_iterator &i );
		void parsePath( const Bu::String &sPath );
		void parsePath( Bu::String::const_iterator &i );
		void clear();

		Bu::String getUrl() const;
		Bu::String getFullPath() const;

		const Bu::String &getProtocol() const { return sProtocol; }
		const Bu::String &getUser() const { return sUser; }
		const Bu::String &getPass() const { return sPass; }
		const Bu::String &getHost() const { return sHost; }
		const Bu::String &getPath() const { return sPath; }
		int getPort() const { return iPort; }
		ParamList::const_iterator getParamBegin() const
			{ return lParam.begin(); }

		void setProtocol( const Bu::String &sNewHost, bool bAutoSetPort=true );
		void setUser( const Bu::String &s ) { sUser = s; }
		void setPass( const Bu::String &s ) { sPass = s; }
		void setHost( const Bu::String &s ) { sHost = s; }
		void setPath( const Bu::String &s ) { sPath = s; }
		void setPort( int i ) { iPort = i; }
		void addParam( const Bu::String &n, const Bu::String &v );

		bool hasPort() const { return iPort.has(); }

		static Bu::String decode( const Bu::String &sStr );
		static Bu::String encode( const Bu::String &sStr );

	private: // Parsing code
		void parseProtocol( Bu::String::const_iterator &i );
		void parseUserPass( Bu::String::const_iterator &i );
		void parseHost( Bu::String::const_iterator &i );

	private:
		Bu::String sProtocol;
		Bu::String sUser;
		Bu::String sPass;
		Bu::String sHost;
		Bu::String sPath;
		Bu::Atom<int> iPort;
		ParamList lParam;

		static char hexcode[16];
	};
};

#endif
