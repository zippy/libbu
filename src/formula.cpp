#include "formula.h"

subExceptionDef( ParseException );

Formula::Formula()
{
	hVars["pi"] = M_PI;
	hVars["e"] = M_E;

	hFunc["sin"] = FuncSin();
}

Formula::~Formula()
{
}

double Formula::run( char *sFormula )
{
	for(;;)
	{
		uint8_t tNum = nextToken( &sFormula );
		if( tNum == symEOS )
			break;
		else if( tNum == symSubtract )
		{
			tNum = nextToken( &sFormula );
			if( tNum != symNumber )
				throw ParseException("Unary minus must be followed by a number, "
					"variable, function, or parenthesis.");
			sValue.top() = -sValue.top();
		}
		else if( tNum == symOpenParen )
		{
			sOper.push( tNum );
			continue;
		}

oppart:	uint8_t tOpr = nextToken( &sFormula );
		if( tOpr == symEOS )
		{
			//printf("EOS ");
			reduce();
			return sValue.top();
			break;
		}
		if( !sOper.empty() && getPrec( sOper.top() ) > getPrec( tOpr ) )
		{
			reduce();
		}
		if( tOpr != symCloseParen )
		{
			sOper.push( tOpr );
		}
		else
		{
			reduce( true );
			goto oppart;
		}
	}
	return sValue.top();
}

void Formula::reduce( bool bCloseParen )
{
	while( !sOper.empty() )
	{
		uint8_t nOpr = sOper.top();
		if( nOpr == symOpenParen )
		{
			//printf("Found ( stopping reduction.\n");
			if( bCloseParen == true )
				sOper.pop();
			return;
		}
		sOper.pop();

		double dTop = sValue.top();
		sValue.pop();

		switch( nOpr )
		{
			case symAdd:
				//printf("%f + %f = %f\n", sValue.top(), dTop, sValue.top()+dTop );
				sValue.top() += dTop;
				break;

			case symSubtract:
				//printf("%f - %f = %f\n", sValue.top(), dTop, sValue.top()-dTop );
				sValue.top() -= dTop;
				break;

			case symMultiply:
				//printf("%f * %f = %f\n", sValue.top(), dTop, sValue.top()*dTop );
				sValue.top() *= dTop;
				break;

			case symDivide:
				//printf("%f / %f = %f\n", sValue.top(), dTop, sValue.top()/dTop );
				sValue.top() /= dTop;
				break;

			case symExponent:
				//printf("%f ^ %f = %f\n", sValue.top(), dTop, pow(sValue.top(),dTop) );
				sValue.top() = pow( sValue.top(), dTop );
				break;

			case symModulus:
				//printf("%f %% %f = %f\n", sValue.top(), dTop, fmod(sValue.top(),dTop) );
				sValue.top() = fmod( sValue.top(), dTop );
				break;
		}
	}

	if( bCloseParen == true )
	{
		throw ParseException("Close-paren found without matching open-paren.");
	}
}

uint8_t Formula::getPrec( uint8_t nOper )
{
	switch( nOper )
	{
		case symNumber:
		case symVariable:
		case symOpenParen:
		case symCloseParen:
			return 0;

		case symAdd:
		case symSubtract:
			return 1;

		case symMultiply:
		case symDivide:
		case symModulus:
			return 2;

		case symExponent:
			return 3;

		default:
			return 0;
	}
}

uint8_t Formula::nextToken( char **sBuf )
{
	for(;;)
	{
		char cbuf = **sBuf;
		++(*sBuf);
		switch( cbuf )
		{
			case '+':
				return symAdd;

			case '-':
				return symSubtract;

			case '*':
				return symMultiply;

			case '/':
				return symDivide;

			case '^':
				return symExponent;

			case '%':
				return symModulus;

			case '(':
				return symOpenParen;

			case ')':
				return symCloseParen;

			case ' ':
			case '\t':
			case '\n':
			case '\r':
				break;

			case '\0':
				return symEOS;

			default:
				if( cbuf == '.' || (cbuf >= '0' && cbuf <= '9') )
				{
					char num[50]={cbuf};
					int nPos = 1;
					bool bDot = false;

					for(;;)
					{
						cbuf = **sBuf;
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
						if( cbuf == '.' || (cbuf >= '0' && cbuf <= '9') )
						{
							num[nPos++] = cbuf;
						}
						else
						{
							num[nPos] = '\0';
							sValue.push( strtod( num, NULL ) );
							return symNumber;
						}
						++(*sBuf);
					}
				}
				else if( (cbuf >= 'a' && cbuf <= 'z') || 
						 (cbuf >= 'A' && cbuf <= 'Z') ||
						 (cbuf == '_') )
				{
					char tok[50]={cbuf};
					int nPos = 1;

					for(;;)
					{
						cbuf = **sBuf;
						if( (cbuf >= 'a' && cbuf <= 'z') || 
							(cbuf >= 'A' && cbuf <= 'Z') ||
							(cbuf >= '0' && cbuf <= '9') ||
							cbuf == '_' || cbuf == '.' || cbuf == ':' )
						{
							tok[nPos++] = cbuf;
						}
						else
						{
							tok[nPos] = '\0';
							//printf("Checking variable \"%s\"\n", tok );
							try
							{
								sValue.push( hVars[tok] );
								return symNumber;
							}
							catch( HashException &e )
							{
								throw ParseException(
									"No variable named \"%s\" exists.",
									tok
									);
							}
						}
						++(*sBuf);
					}
				}
				break;
		}
	}
}

