#ifndef STATIC_STRING_H
#define STATIC_STRING_H

#include <string>
#include "serializable.h"

/**
 * Simple string managing class.  Allows for dynamically allocated string data
 * along with some minor caching to speed things up when accessing your
 * string data.  Really designed for making copies of strings easy and safe,
 * and making accessing meta-info like length fast and reliable as well.
 *@author Mike Buland
 */
class StaticString : public Serializable
{
public:
	StaticString();
	StaticString( const char *lpNewStr, int nNewLen=-1 );
	StaticString( StaticString &xSrcStr, int nNewLen=-1 );
	StaticString( int nLength );
	virtual ~StaticString();

	char *getString();
	int getLength();
	int setLength( int nNewLength );

	void setString( const char *lpNewStr, int nNewLen=-1 );
	void setString( StaticString &sNewStr, int nNewLen=-1 );

	char getAt( int nIndex );
	void setAt( int nIndex, char cVal );

	class StaticString &operator=( class StaticString &lpOtherStr );
	class StaticString &operator=( std::string &lpOtherStr );
	class StaticString &operator=( const char *lpNewStr );
	operator const char *();
	char &operator[]( int nIndex );
	operator int();
	char *operator+( int nAmnt );
	char *operator-( int nAmnt );

	void clear();

	virtual void serialize( class Serializer &ar );

private:
	char *lpStr;
	int nLen;
};

#endif
