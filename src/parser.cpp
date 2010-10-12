#include "bu/parser.h"
#include "bu/lexer.h"

#include "bu/sio.h"
using namespace Bu;

Bu::Parser::Parser()
{
}

Bu::Parser::~Parser()
{
}

void Bu::Parser::pushLexer( Lexer *pLex )
{
	sLexer.push( pLex );
}

void Bu::Parser::popLexer()
{
	delete sLexer.peekPop();
}

void Bu::Parser::parse()
{
	for(;;)
	{
		Bu::Lexer::Token *pToken = sLexer.peek()->nextToken();
		sio << sLexer.peek()->tokenToString( *pToken ) << sio.nl;
		if( pToken->iToken < 0 )
		{
			delete sLexer.peekPop();
			if( sLexer.isEmpty() )
			{
				delete pToken;
				return;
			}
		}
		delete pToken;
	}	
}

void Bu::Parser::setRootNonTerminal( int iRoot )
{
	iRootNonTerminal = iRoot;
}

void Bu::Parser::setRootNonTerminal( const Bu::FString &sRoot )
{
	setRootNonTerminal( hNonTerminalName.get( sRoot ) );
}

int Bu::Parser::addNonTerminal( const Bu::FString &sName, NonTerminal &nt )
{
	int iId = aNonTerminal.getSize();
	aNonTerminal.append( nt );
	hNonTerminalName.insert( sName, iId );
	return iId;
}

int Bu::Parser::addNonTerminal( const Bu::FString &sName )
{
	int iId = aNonTerminal.getSize();
	aNonTerminal.append( NonTerminal() );
	hNonTerminalName.insert( sName, iId );
	return iId;
}

void Bu::Parser::setNonTerminal( const Bu::FString &sName, NonTerminal &nt )
{
	aNonTerminal[hNonTerminalName.get(sName)] = nt;
}

int Bu::Parser::getNonTerminalId( const Bu::FString &sName )
{
	return hNonTerminalName.get( sName );
}

int Bu::Parser::addReduction( const Bu::FString &sName, const Reduction &r )
{
	int iId = aReduction.getSize();
	aReduction.append( r );
	hReductionName.insert( sName, iId );
	return iId;
}

int Bu::Parser::addReduction( const Bu::FString &sName )
{
	int iId = aReduction.getSize();
	aReduction.append( Reduction() );
	hReductionName.insert( sName, iId );
	return iId;
}

void Bu::Parser::setReduction( const Bu::FString &sName, const Reduction &r )
{
	aReduction[hReductionName.get(sName)] = r;
}

int Bu::Parser::getReductionId( const Bu::FString &sName )
{
	return hReductionName.get( sName );
}

//
// Bu::Parser::State
//

Bu::Parser::State::State( Bu::Parser::State::Type eType, int iIndex ) :
	eType( eType ),
	iIndex( iIndex )
{
}

Bu::Parser::State::~State()
{
}

//
// Bu::Parser::NonTerminal
//

Bu::Parser::NonTerminal::NonTerminal()
{
}

Bu::Parser::NonTerminal::~NonTerminal()
{
}

void Bu::Parser::NonTerminal::addProduction( Production p )
{
	lProduction.append( p );
}

