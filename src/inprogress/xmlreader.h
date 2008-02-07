/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef XML_READER_H
#define XML_READER_H

#include <stdint.h>
#include "bu/stream.h"
#include "bu/fstring.h"
#include "bu/xmlnode.h"

namespace Bu
{
	/**
	 * An Xml 1.1 reader.  I've decided to write this, this time, based on the
	 * official W3C reccomendation, now included with the source code.  I've
	 * named the productions in the parser states the same as in that document,
	 * which may make them easier to find, etc, although possibly slightly less
	 * optimized than writing my own reduced grammer.
	 *
	 * Below I will list differences between my parser and the official standard
	 * as I come up with them.
	 *  - Encoding and Standalone headings are ignored for the moment. (4.3.3,
	 *    2.9)
	 *  - The standalone heading attribute can have any standard whitespace
	 *    before it (the specs say only spaces, no newlines). (2.9)
	 *  - Since standalone is ignored, it is currently allowed to have any
	 *    value (should be restricted to "yes" or "no"). (2.9)
	 *  - Currently only UTF-8 / ascii are parsed.
	 *  - [optional] The content of comments is thrown away. (2.5)
	 *  - The content of processing instruction blocks is parsed properly, but
	 *    thrown away. (2.6)
	 */
	class XmlReader
	{
	public:
		XmlReader( Bu::Stream &sIn );
		virtual ~XmlReader();

		XmlNode *read();

	private:
		Bu::Stream &sIn;
		Bu::FString sBuf;

	private: // Helpers
		const char *lookahead( int nAmnt );
		void burn( int nAmnt );
		void checkString( const char *str, int nLen );

	private: // States
		/**
		 * The headers, etc.
		 */
		void prolog();

		/**
		 * The xml decleration (version, encoding, etc).
		 */
		void XMLDecl();

		/**
		 * Misc things, Includes Comments and PIData (Processing Instructions).
		 */
		void Misc();

		/**
		 * Comments
		 */
		void Comment();

		/**
		 * Processing Instructions
		 */
		void PI();

		/**
		 * Whitespace eater.
		 */
		void S();

		/**
		 * Optional whitespace eater.
		 */
		void Sq();

		/**
		 * XML Version spec
		 */
		void VersionInfo();

		/**
		 * Your basic equals sign with surrounding whitespace.
		 */
		void Eq();

		/**
		 * Read in an attribute value.
		 */
		FString AttValue();

		/**
		 * Read in the name of something.
		 */
		FString Name();

		/**
		 * Encoding decleration in the header
		 */
		void EncodingDecl();

		/**
		 * Standalone decleration in the header
		 */
		void SDDecl();

		bool isS( unsigned char c )
		{
			return ( c == 0x20 || c == 0x9 || c == 0xD || c == 0xA );
		}
	};
}

#endif
