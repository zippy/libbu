/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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
#include "bu/string.h"

namespace Bu
{
	subExceptionDecl( FormulaException );
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
		class Func
		{
		public:
			virtual prec operator()( prec )=0;
		};

		typedef Hash<Bu::String, prec> varHash;
		typedef Hash<Bu::String, Func *> funcHash;

		Formula()
		{
		}

		virtual ~Formula()
		{
			for( typename funcHash::iterator i = hFunc.begin();
				 i != hFunc.end(); i++ )
			{
				delete (*i);
			}
		}

		prec run( const Bu::String &sFormulaSrc )
		{
			if( sFormulaSrc.isEmpty() )
				throw FormulaException("Empty formula, nothing to do.");
			try
			{
				const char *sFormula = sFormulaSrc.getStr();
				for(;;)
				{
					uint8_t tNum = nextToken( &sFormula );
					if( tNum == symSubtract )
					{
						sOper.push( symNegate );
						continue;
					}
					else if( tNum == symNot )
					{
						sOper.push( symNot );
						continue;
					}
					else if( tNum == symOpenParen )
					{
						sOper.push( tNum );
						continue;
					}
					else if( tNum == symFunction )
					{
						sOper.push( symFunction );
						continue;
					}
					else if( tNum == symEOS )
					{
						throw Bu::FormulaException(
							"Cannot end with an operator.");
					}

			oppart:	uint8_t tOpr = nextToken( &sFormula );
					if( tOpr == symEOS )
					{
						reduce();
						prec ret = sValue.top();
						sValue.clear();
						sFunc.clear();
						sOper.clear();
						return ret;
					}
					if( !sOper.isEmpty() && getPrec( sOper.top() ) >
						getPrec( tOpr ) )
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
			}
			catch( ... )
			{
				sValue.clear();
				sFunc.clear();
				sOper.clear();
				throw;
			}
		}

		varHash hVars;
		funcHash hFunc;

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
			symFunction,
			symExponent,
			symNegate,
			symModulus,

			symAnd,
			symOr,
			symXor,
			symNot
		};

		typedef uint8_t symType;

		Bu::Stack<symType> sOper;
		Bu::Stack<prec> sValue;
		Bu::Stack<Bu::String> sFunc;

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
				case symNegate:
				case symFunction:
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
										throw FormulaException(
											"Numbers cannot have more than one "
											". in them."
											);
								}
								if( cbuf == '.' ||
									(cbuf >= '0' && cbuf <= '9') )
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
									if( hVars.has( tok ) )
									{
										sValue.push( hVars[tok] );
										return symNumber;
									}
									else if( hFunc.has( tok ) )
									{
										sFunc.push( tok );
										return symFunction;
									}
									else
									{
										throw FormulaException(
											"No variable or function named "
											"\"%s\" exists.",
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
						sValue.top() += dTop;
						break;

					case symSubtract:
						sValue.top() -= dTop;
						break;

					case symMultiply:
						sValue.top() *= dTop;
						break;

					case symDivide:
						sValue.top() /= dTop;
						break;

					case symExponent:
						sValue.top() = static_cast<prec>(
								pow( sValue.top(), dTop )
							);
						break;

					case symModulus:
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

					case symFunction:
						sValue.push( (*hFunc.get( sFunc.pop() ))( dTop ) );
						break;

					case symNegate:
						sValue.push( -dTop );
						break;

					case symNot:
						sValue.push( static_cast<prec>(
								~static_cast<bin>(dTop)
							) );
						break;
				}
			}

			if( bCloseParen == true )
			{
				throw FormulaException(
					"Close-paren found without matching open-paren."
					);
			}
		}
	};
}

#endif
