#include "formula.h"

subExceptionDef( ParseException );

Formula::Formula()
{
}

Formula::~Formula()
{
}

double Formula::run( const char *sFormula )
{
	sBuf.write( sFormula, strlen( sFormula ) );
	sBuf.setPos( 0 );

	nState = 0;
	for(;;)
	{
		tLook = nextToken();
		if( tLook.nSym == symEOS )
			break;
		state();
	}
	printf("\n");
	return 0.0;
}

void Formula::state()
{
	switch( nState )
	{
		case 0: // initial expr
			switch( tLook.nSym )
			{
				case symNumber:
					push();
					nState = 1;
					break;
			}
			break;

		case 1: // binary operator
			switch( tLook.nSym )
			{
				case symAdd:
				case symSubtract:
					push();
					nState = 2;
					break;

				case symMultiply:
				case symDivide:
					push();
					nState = 3;
					break;
			}
			break;

		case 2: // add/subtract
			break;
	}
}

void Formula::push()
{
	printToken( tLook );
	sToken.push( tLook );
}

Formula::Token Formula::nextToken()
{
	char cbuf;
	for(;;)
	{
		if( sBuf.isEOS() )
			return Token( symEOS );

		sBuf.read( &cbuf, 1 );
		switch( cbuf )
		{
			case '+':
				return Token( symAdd );

			case '-':
				return Token( symSubtract );

			case '*':
				return Token( symMultiply );

			case '/':
				return Token( symDivide );

			case '(':
				return Token( symOpenParen );

			case ')':
				return Token( symCloseParen );

			case ' ':
			case '\t':
			case '\n':
			case '\r':
				break;

			default:
				if( cbuf == '.' || (cbuf >= '0' && cbuf <= '9') )
				{
					char num[50];
					int nPos = 0;
					bool bDot = false;

					for(;;)
					{
						num[nPos++] = cbuf;
						if( cbuf == '.' )
						{
							if( bDot == false )
								bDot = true;
							else
								throw ParseException(
									"Numbers cannot have more than one "
									". in them."
									);
						}
						sBuf.read( &cbuf, 1 );
						if( (cbuf != '.' && (cbuf < '0' || cbuf > '9')) || 
							sBuf.isEOS() )
						{
							if( !sBuf.isEOS() )	sBuf.seek( -1 );
							num[nPos] = '\0';
							return Token( symNumber, strtod( num, NULL ) );
						}
					}
				}
				break;
		}
	}
}

void Formula::printToken( Token &tok )
{
	switch( tok.nSym )
	{
		case symEOS:		printf("[EOS] ");				break;
		case symAdd:		printf("+ ");					break;
		case symSubtract:	printf("- ");					break;
		case symMultiply:	printf("* ");					break;
		case symDivide:		printf("/ ");					break;
		case symOpenParen:	printf("( ");					break;
		case symCloseParen:	printf(") ");					break;
		case symNumber:		printf("%f ", tok.val.num );	break;
		default:			printf("??? ");					break;
	}
}

