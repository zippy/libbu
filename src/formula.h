/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FORMULA_H
#define FORMULA_H

#include <stdint.h>
#include <stdlib.h>

#include <math.h>
//#include "sbuffer.h"

#include "bu/stack.h"
#include "bu/exceptionbase.h"
#include "bu/hash.h"
#include "bu/fstring.h"

subExceptionDecl( ParseException );

namespace Bu
{
	/**
	 * Implements a very simple formula parser that allows use of variables and
	 * custom functions.  This is based on a simple calculator-type parser that
	 * executes as it processes, accounting for operator precedence and
	 * grouping.
	 *
	 * prec = precision, a type to use for all math (except binary ops)
	 * bin = binary type, a type to hard cast all data to for binary ops
	 */
	template<typename prec, typename bin=uint32_t>
	class Formula
	{
	public:
		Formula()
		{
		}

		virtual ~Formula()
		{
		}

		prec run( const char *sFormula )
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
						throw ParseException(
							"Unary minus must be followed by a number, "
							"variable, function, or parenthesis.");
					sValue.top() = -sValue.top();
				}
				else if( tNum == symNot )
				{
					tNum = nextToken( &sFormula );
					if( tNum != symNumber )
						throw ParseException(
							"Unary, binary not must be followed by a number, "
							"variable, function, or parenthesis.");
					sValue.top() = static_cast<prec>(
							~static_cast<bin>(sValue.top())
						);
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
				if( !sOper.isEmpty() && getPrec( sOper.top() ) > getPrec( tOpr ) )
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


		typedef Hash<Bu::FString, prec> varHash;
		varHash hVars;

		typedef struct Func
		{
			prec operator()( prec x )
			{
				return 0;
			}
		} Func;

		typedef Hash<Bu::FString, Func> funcHash;
		funcHash hFunc;
/*
		typedef struct FuncSin : Func
		{
			prec operator()( prec x )
			{
				return sin( x );
			}
		} FuncSin;
		*/

	private:
		enum
		{
			symEOS,
			symAdd,
			symSubtract,
			symMultiply,
			symDivide,
			symOpenParen,
			symCloseParen,
			symNumber,
			symVariable,
			symExponent,
			symModulus,

			symAnd,
			symOr,
			symXor,
			symNot
		};

		typedef uint8_t symType;

		Bu::Stack<symType> sOper;
		Bu::Stack<prec> sValue;

	private:
		symType getPrec( symType nOper )
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

				case symAnd:
				case symOr:
				case symXor:
					return 2;

				case symExponent:
				case symNot:
					return 3;

				default:
					return 0;
			}
		}

		symType nextToken( const char **sBuf )
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

					case '|':
						return symOr;

					case '&':
						return symAnd;

					case '#':
						return symXor;

					case '~':
						return symNot;

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
									sValue.push(
										static_cast<prec>(
											strtod( num, NULL )
											)
										);
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

		void reduce( bool bCloseParen = false )
		{
			while( !sOper.isEmpty() )
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

				prec dTop = sValue.top();
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
						sValue.top() = static_cast<prec>(
								pow( sValue.top(), dTop )
							);
						break;

					case symModulus:
						//printf("%f %% %f = %f\n", sValue.top(), dTop, fmod(sValue.top(),dTop) );
						sValue.top() = static_cast<prec>(
								fmod( sValue.top(), dTop )
							);
						break;

					case symOr:
						sValue.top() = static_cast<prec>(
								static_cast<bin>(sValue.top()) |
								static_cast<bin>(dTop)
							);
						break;
					
					case symAnd:
						sValue.top() = static_cast<prec>(
								static_cast<bin>(sValue.top()) &
								static_cast<bin>(dTop)
							);
						break;
					
					case symXor:
						sValue.top() = static_cast<prec>(
								static_cast<bin>(sValue.top()) ^
								static_cast<bin>(dTop)
							);
						break;
				}
			}

			if( bCloseParen == true )
			{
				throw ParseException(
					"Close-paren found without matching open-paren."
					);
			}
		}
	};
}

#endif
