/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_XML_READER_H
#define BU_XML_READER_H

#include "bu/string.h"
#include "bu/exceptionbase.h"

namespace Bu
{
	class Stream;

	subExceptionDecl( XmlException );

	class XmlReader
	{
	public:
		XmlReader( Stream &rInput );
		virtual ~XmlReader();

	private:
		Stream &rInput;
		int iCurToken;
		int iNextToken;
		Bu::String sBuf;
		Bu::String sStr;
		bool bIgnoreWS;
		typedef struct StreamPos
		{
			StreamPos() : iLine( 1 ), iChar( 1 ) { }
			int iLine;
			int iChar;
		} StreamPos;
		StreamPos spCurToken;
		StreamPos spNextToken;


		enum
		{
			tokXmlDeclHead	=	0x100,
			tokXmlDeclEnd,
			tokWS,
			tokIdent,
			tokString
		};

		void fillBuffer();
		void cleanupBuffer( int iUsed );
		int nextToken();

		void stDocument();
		void stProlog();
		void stXmlDecl();

		void error( const char *sMessage );
	};
};

#endif
