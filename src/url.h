/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_URL_H
#define BU_URL_H

#include "bu/fstring.h"
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
			Param( const Bu::FString &n, const Bu::FString &v ) :
				sName( n ), sValue( v ) { }
			Bu::FString sName;
			Bu::FString sValue;
		} Param;
		typedef Bu::List<Param> ParamList;

	public:
		Url();
		Url( const Bu::FString &sUrl );
		virtual ~Url();

		void parseUrl( const Bu::FString &sUrl );
		void parseParams( const Bu::FString &sQuery );
		void parseParams( Bu::FString::const_iterator &i );
		void parsePath( const Bu::FString &sPath );
		void parsePath( Bu::FString::const_iterator &i );
		void clear();

		Bu::FString getUrl() const;
		Bu::FString getFullPath() const;

		const Bu::FString &getProtocol() const { return sProtocol; }
		const Bu::FString &getUser() const { return sUser; }
		const Bu::FString &getPass() const { return sPass; }
		const Bu::FString &getHost() const { return sHost; }
		const Bu::FString &getPath() const { return sPath; }
		int getPort() const { return iPort; }
		ParamList::const_iterator getParamBegin() const
			{ return lParam.begin(); }

		void setProtocol( const Bu::FString &sNewHost, bool bAutoSetPort=true );
		void setUser( const Bu::FString &s ) { sUser = s; }
		void setPass( const Bu::FString &s ) { sPass = s; }
		void setHost( const Bu::FString &s ) { sHost = s; }
		void setPath( const Bu::FString &s ) { sPath = s; }
		void setPort( int i ) { iPort = i; }
		void addParam( const Bu::FString &n, const Bu::FString &v );

		bool hasPort() const { return iPort.has(); }

		static Bu::FString decode( const Bu::FString &sStr );
		static Bu::FString encode( const Bu::FString &sStr );

	private: // Parsing code
		void parseProtocol( Bu::FString::const_iterator &i );
		void parseUserPass( Bu::FString::const_iterator &i );
		void parseHost( Bu::FString::const_iterator &i );

	private:
		Bu::FString sProtocol;
		Bu::FString sUser;
		Bu::FString sPass;
		Bu::FString sHost;
		Bu::FString sPath;
		Bu::Atom<int> iPort;
		ParamList lParam;

		static char hexcode[16];
	};
};

#endif
