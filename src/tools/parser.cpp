#include <bu/parser.h>
#include <bu/lexer.h>
#include <bu/file.h>
#include <bu/sio.h>
#include <bu/queuebuf.h>
#include <stdlib.h>

using namespace Bu;

enum Tok
{
	tokNumber,
	tokPlus,
	tokMinus,
	tokDivide,
	tokMultiply,
	tokOpenParen,
	tokCloseParen,
	tokCompute,
	tokEndOfInput=-1
};

Bu::Formatter &operator<<( Bu::Formatter &f, Tok e )
{
	switch( e )
	{
		case tokNumber: return f << "tokNumber";
		case tokPlus: return f << "tokPlus";
		case tokMinus: return f << "tokMinus";
		case tokDivide: return f << "tokDivide";
		case tokMultiply: return f << "tokMultiply";
		case tokOpenParen: return f << "tokOpenParen";
		case tokCloseParen: return f << "tokCloseParen";
		case tokCompute: return f << "tokCompute";
		case tokEndOfInput: return f << "tokEndOfInput";
	}

	return f << "***error***";
}

class MathLexer : public Lexer
{
public:
	MathLexer( Bu::Stream &rSrc ) :
		rSrc( rSrc )
	{
	}

	virtual ~MathLexer()
	{
	}

	enum TokenTypes
	{
		tokStuff
	};

	virtual Token *nextToken()
	{
		for(;;)
		{
			if( qbIn.getSize() == 0 )
			{
				char buf[4096];
				qbIn.write( buf, rSrc.read( buf, 4096 ) );

				if( rSrc.isEos() && qbIn.getSize() == 0 )
					return new Token( tokEndOfInput );
			}

			char b;
			qbIn.peek( &b, 1 );
			switch( b )
			{
				case '+':
					qbIn.seek( 1 );
					return new Token( tokPlus );

				case '-':
					qbIn.seek( 1 );
					return new Token( tokMinus );

				case '/':
					qbIn.seek( 1 );
					return new Token( tokDivide );

				case '*':
					qbIn.seek( 1 );
					return new Token( tokMultiply );

				case ' ':
				case '\t':
				case '\n':
					qbIn.seek( 1 );
					break;

				case '=':
					qbIn.seek( 1 );
					return new Token( tokCompute );

				case '(':
					qbIn.seek( 1 );
					return new Token( tokOpenParen );

				case ')':
					qbIn.seek( 1 );
					return new Token( tokCloseParen );

				case '.':
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					{
						Bu::FString sTmp;
						sTmp += b;
						qbIn.seek( 1 );
						for(;;)
						{
							qbIn.peek( &b, 1 );
							if( b != '.' && (b < '0' || b > '9') )
							{
								sio << "!! Convert '" << sTmp << "' to "
									<< strtod( sTmp.getStr(), NULL ) << sio.nl;
								return new Token(
									tokNumber, strtod( sTmp.getStr(), NULL )
									);
							}
							qbIn.seek( 1 );
							sTmp += b;
						}
					}
					break;

				default:
					throw Bu::ExceptionBase("Unexpected character '%c'.", b );
			}
		}
	}

private:
	Bu::Stream &rSrc;
	QueueBuf qbIn;
};

void redAdd( Bu::Parser &p )
{
}

void redPrint( Bu::Parser &p )
{
}

int main( int argc, char *argv[] )
{
	File fIn( argv[1], File::Read );

	Parser p;

	{
		Parser::NonTerminal nt;
		int iSelf = p.addNonTerminal("expr");
		nt.addProduction(
			Parser::Production(
				Parser::State(
					Parser::State::typeTerminal,
					tokPlus
					)
				).append(
				Parser::State(
					Parser::State::typeNonTerminal,
					iSelf
					)
				).append(
				Parser::State(
					Parser::State::typeReduction,
					p.addReduction("add")
					)
				)
			);
		nt.addProduction(
			Parser::Production()
			);
		p.addNonTerminal( "expr", nt );
	}
	{
		Parser::NonTerminal nt;
		nt.addProduction(
			Parser::Production(
				Parser::State(
					Parser::State::typeTerminalPush,
					tokNumber
					)
				).append(
				Parser::State(
					Parser::State::typeNonTerminal,
					p.getNonTerminalId("expr")
					)
				)
			);
		p.addNonTerminal( "expr'", nt );
	}
	{
		Parser::NonTerminal nt;
		nt.addProduction(
			Parser::Production(
				Parser::State(
					Parser::State::typeNonTerminal,
					p.getNonTerminalId("expr'")
					)
				).append(
				Parser::State(
					Parser::State::typeTerminal,
					tokCompute
					)
				).append(
				Parser::State(
					Parser::State::typeReduction,
					p.addReduction("print")
					)
				)
			);
		p.addNonTerminal("input", nt );
	}

	p.setRootNonTerminal("input");

	p.setReduction("add", Bu::slot( &redAdd ) );
	p.setReduction("print", Bu::slot( &redPrint ) );

	p.pushLexer( new MathLexer( fIn ) );

	p.parse();

	return 0;
}

