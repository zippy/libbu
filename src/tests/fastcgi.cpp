/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/fastcgi.h"

#include <unistd.h>

class Cgi : public Bu::FastCgi
{
public:
	Cgi() :
		Bu::FastCgi::FastCgi()
	{
	}

	Cgi( int iPort ) :
		Bu::FastCgi::FastCgi( iPort )
	{
	}

	virtual ~Cgi()
	{
	}

	virtual int onRequest( const StrHash &hParams,
		const Bu::FString &sStdIn, Bu::Stream &sStdOut,
		Bu::Stream &/*sStdErr*/ )
	{
		Bu::FString sOut("Content-Type: text/html\r\n\r\n");
		sOut += "<html><body><h1>Environment:</h1><ul>";
		for( StrHash::const_iterator i = hParams.begin(); i; i++ )
		{
			sOut += "<li>" + i.getKey() + " = " +
				i.getValue() + "</li>";
		}
		sOut += "</ul>";
		char buf[2048];
		sOut += "<h1>Cwd:</h1><ul><li>";
		sOut += getcwd( buf, 2048 );
		sOut += "</li></ul>";
		sOut += "<h1>Stdin:</h1>";
		sOut.formatAppend("%d bytes<pre>", sStdIn.getSize() );
		Bu::FString sL, sR;
		for( Bu::FString::const_iterator i = sStdIn.begin();
			i; i++ )
		{
			sL.formatAppend("%02X ",
				(unsigned int)((unsigned char)*i) );
			if( *i < 27 )
				sR += ". ";
			else
				sR.formatAppend("&#%d; ",
					(unsigned int)((unsigned char)*i) );
			if( sL.getSize()/3 == 8 )
			{
				sOut += sL + " | " + sR + "\n";
				sL = sR = "";
			}
		}
		if( sL != "" )
		{
			while( sL.getSize()/3 < 8 )
				sL += "   ";
			sOut += sL + " | " + sR + "\n";
		}
		sOut += "</pre><hr/><pre>";
		sOut += sStdIn;
		sOut += "</pre>";
		sOut += "<form method=\"post\" enctype=\"multipart/form-data\"><textarea name=\"bob\"></textarea><br /><input type=\"file\" name=\"somefile\" /><br /><input type=\"submit\" name=\"yeah\" value=\"try it\" /></form>";
		sOut += "</body></html>";

		sStdOut.write( sOut );

		return 0;
	}
};

int main()
{
	Cgi c( 8789 );

	c.run();

	return 0;
}

