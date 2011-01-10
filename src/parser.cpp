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

Lexer::Token *Bu::Parser::popToken()
{
	return sToken.peekPop();
}

void Bu::Parser::pushToken( Lexer::Token *pTok )
{
	sToken.push( pTok );
}

void Bu::Parser::parse()
{
	int iCurNt = iRootNonTerminal;
	Lexer::Token *ptCur = sLexer.peek()->nextToken();
	sio << "Token(a): " << sLexer.peek()->tokenToString( *ptCur ) << sio.nl;
	selectProduction( iCurNt, ptCur );
	
	while( !sState.isEmpty() )
	{
		switch( (*sState.peek()).eType )
		{
			case State::typeTerminal:
				sio << "terminal: " << ptCur->iToken << " == "
					<< (*sState.peek()).iIndex << sio.nl;
				if( ptCur->iToken == (*sState.peek()).iIndex )
				{
					advanceState();
					delete ptCur;
					ptCur = sLexer.peek()->nextToken();
					sio << "Token(b): " << sLexer.peek()->tokenToString( *ptCur ) << sio.nl;
				}
				else
				{
					throw Bu::ExceptionBase("Error parsing code.");
				}
				break;

			case State::typeTerminalPush:
				sio << "terminalpush: " << ptCur->iToken << " == "
					<< (*sState.peek()).iIndex << sio.nl;
				if( ptCur->iToken == (*sState.peek()).iIndex )
				{
					advanceState();
					sToken.push( ptCur );

					ptCur = sLexer.peek()->nextToken();
					sio << "Token(c): " << sLexer.peek()->tokenToString( *ptCur ) << sio.nl;
				}
				else
				{
					throw Bu::ExceptionBase("Error parsing code.");
				}
				break;

			case State::typeNonTerminal:
				sio << "nonterminal: " << ptCur->iToken << " --> "
					<< (*sState.peek()).iIndex << sio.nl;
				{
					int iNt = (*sState.peek()).iIndex;
					sio << "Current state: " << *sState.peek() << sio.nl;
					if( !selectProduction( iNt, ptCur ) )
					{
						throw Bu::ExceptionBase("Error parsing code.");
					}
				}
				break;

			case State::typeReduction:
				sio << "reduction" << sio.nl;
				aReduction[(*sState.peek()).iIndex]( *this );
				advanceState();
				break;
		}
	}	
}

bool Bu::Parser::selectProduction( int iNt, Lexer::Token *ptCur )
{
	NonTerminal &nt = aNonTerminal[iNt];
	int j = 0;
	for( NonTerminal::ProductionList::iterator i = nt.lProduction.begin();
		 i; i++,j++ )
	{
		if( (*i).isEmpty() )
			continue;
		sio << "-->(Attempting production " << iNt << ":" << j << ": "
			<< (*i).first() << ")" << sio.nl;
		if( (*i).first().eType == State::typeTerminal ||
			(*i).first().eType == State::typeTerminalPush )
		{
			if( (*i).first().iIndex == ptCur->iToken )
			{
				sState.push( (*i).begin() );
				sio.incIndent();
				sio << "Pushing production " << j << " from nt " << iNt
					<< sio.nl;
				return true;
			}
		}
		else if( (*i).first().eType == State::typeNonTerminal )
		{
			sState.push( (*i).begin() );
			sio.incIndent();
			sio << "Pushing production " << j << " from nt " << iNt
				<< " as test." << sio.nl;
			if( !selectProduction( (*i).first().iIndex, ptCur ) )
			{
				sio.decIndent();
				sState.pop();
				sio << "Production " << j << " from nt " << iNt
					<< " didn't work out." << sio.nl;
			}
			else
			{
				return true;
			}
		}
	}
	if( nt.bCanSkip )
	{
		sio << "Nothing matches, skipping non-terminal." << sio.nl;
		advanceState();
		return true;
	}
	sio << "-->(Found nothing)" << sio.nl;
	return false;
}

void Bu::Parser::advanceState()
{
	if( sState.isEmpty() )
		return;

	sState.peek()++;
	if( !sState.peek() )
	{
		sio.decIndent();
		sState.pop();
		sio << "State advanced, End of production." << sio.nl;
		advanceState();
		return;
	}
	sio << "State advanced, now: " << *(sState.peek()) << sio.nl;
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
	sio << "nt '" << sName << "' = " << iId << sio.nl;
	return iId;
}

int Bu::Parser::addNonTerminal( const Bu::FString &sName )
{
	int iId = aNonTerminal.getSize();
	aNonTerminal.append( NonTerminal() );
	hNonTerminalName.insert( sName, iId );
	sio << "nt '" << sName << "' = " << iId << sio.nl;
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

bool Bu::Parser::hasNonTerminal( const Bu::FString &sName )
{
	return hNonTerminalName.has( sName );
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

bool Bu::Parser::hasReduction( const Bu::FString &sName )
{
	return hReductionName.has( sName );
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

Bu::Parser::NonTerminal::NonTerminal() :
	bCanSkip( false )
{
}

Bu::Parser::NonTerminal::~NonTerminal()
{
}

void Bu::Parser::NonTerminal::addProduction( Production p )
{
	lProduction.append( p );
}

void Bu::Parser::NonTerminal::setCanSkip()
{
	bCanSkip = true;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, Bu::Parser::State::Type t )
{
	switch( t )
	{
		case Bu::Parser::State::typeTerminal:
			return f << "typeTerminal";

		case Bu::Parser::State::typeTerminalPush:
			return f << "typeTerminalPush";

		case Bu::Parser::State::typeNonTerminal:
			return f << "typeNonTerminal";

		case Bu::Parser::State::typeReduction:
			return f << "typeReduction";
	}
	return f << "***error***";
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, const Bu::Parser::State &s )
{
	return f << "{" << s.eType << ": " << s.iIndex << "}";
}

