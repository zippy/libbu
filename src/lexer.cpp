/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/lexer.h"

Bu::Lexer::Lexer()
{
}

Bu::Lexer::~Lexer()
{
}

Bu::Lexer::Token::Token() :
	iToken( -1 )
{
}

Bu::Lexer::Token::Token( Bu::Lexer::TokenType iToken ) :
	iToken( iToken )
{
}

Bu::String Bu::Lexer::tokenToString( const Bu::Lexer::Token &t )
{
	Bu::MemBuf mb;
	Bu::Formatter f( mb );
	f << "<" << t.iToken << ">";
	if( t.vExtra.isSet() )
		f << " (" << t.vExtra << ")";

	return mb.getString();
}

