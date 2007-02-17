#ifndef FORMULA_H
#define FORMULA_H

#include <stdint.h>

#include <stack>
#include "sbuffer.h"

#include "exceptionbase.h"

subExceptionDecl( ParseException );

/**
 *
 */
class Formula
{
public:
	Formula();
	virtual ~Formula();

	double run( const char *sFormula );

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
		symVariable
	};

	typedef struct Token
	{
		Token() {}
		Token( uint8_t nSym ) : nSym( nSym ) { }
		Token( uint8_t nSym, double dNum ) : nSym( nSym ) { val.num = dNum; }
		uint8_t nSym;
		union 
		{
			double num;
		} val;
	} Token;

	std::stack<Token> sToken;
	Token tLook;
	int nState;
	SBuffer sBuf;

private:
	void push();
	void state();
	Token nextToken();
	void printToken( Token &tok );
};

#endif
