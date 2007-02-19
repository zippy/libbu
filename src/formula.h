#ifndef FORMULA_H
#define FORMULA_H

#include <stdint.h>

#include <math.h>
#include <stack>
#include "sbuffer.h"

#include "exceptionbase.h"
#include "hash.h"

subExceptionDecl( ParseException );

/**
 * Implements a very simple formula parser that allows use of variables and
 * custom functions.  This is based on a simple calculator-type parser that
 * executes as it processes, accounting for operator precedence and grouping.
 */
class Formula
{
public:
	Formula();
	virtual ~Formula();

	double run( char *sFormula );

	typedef Hash<std::string, double> varHash;
	varHash hVars;

	typedef struct Func
	{
		double operator()( double x )
		{
			return 0.0;
		}
	} Func;

	typedef Hash<std::string, Func> funcHash;
	funcHash hFunc;

	typedef struct FuncSin : Func
	{
		double operator()( double x )
		{
			return sin( x );
		}
	} FuncSin;

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
		symModulus
	};

	typedef uint8_t symType;

	std::stack<symType> sOper;
	std::stack<double> sValue;

private:
	symType getPrec( symType nOper );
	symType nextToken( char **sBuf );
	void reduce( bool bCloseParen = false );
};

#endif
