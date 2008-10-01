#ifndef BU_XML_READER_H
#define BU_XML_READER_H

#include "bu/fstring.h"
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
		Bu::FString sBuf;
		Bu::FString sStr;
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
