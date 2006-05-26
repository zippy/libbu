#ifndef TOKENSTRING_H
#define TOKENSTRING_H

#include "linkedlist.h"

/** A single tokenized command line.  Contains all information necesarry to
  * nicely access a stand-alone command line and to perform alias expansion
  * inside of that command line.
  * When expanding a token, the original command line is left intact, so any
  * command usng a command line verbatum (getTokenString not getToken) will get
  * the original, and not the expanded version.
  * Since indexing into the original command line is also done by token, it
  * means that using getTokenString( 0 ) will not always get you the first
  * character of the command line, it will get you the first non-whitespace
  * character.
  * Furthermore, when expanding the expantion string is tokenized as well,
  * but since the original string is unchanged, all tokens that expand any
  * given index will all retain the same index into the original command line.
  *@todo Update this to allow it to break on different types of token
  * delimiters.
  *@author Mike Buland
  */
class TokenString{
public:
	/** Automatically call parseLine when created.
	  *@param lpNewTokenString The command line to tokenize
	  *@author Mike Buland
	  */
	TokenString( const char *lpNewTokenString=NULL );
	~TokenString();

	/** Performs a tokenizing parse on the given command line, setting it as
	  * the internal command line for all future tokenizing (excluding
	  * expansion)
	  *@param lpNewTokenString The new command line to set to this object.
	  *@author Mike Buland
	  */
	void parseLine( const char *lpNewTokenString );

	/** Appends a token to the list of available tokens.  This references the
	  * internal pointer to the command line, so no token string must be
	  * specified.
	  *@param nStart The first character of the token to insert.
	  *@param nEnd The last character of the token to insert.
	  *@author Mike Buland
	  */
	void appendToken( int nStart, int nEnd );

	/** Gets the number of tokens.  This is particularly useful post-aliasing
	  * since the number of tokens may not match what is percieved from the
	  * original command line.
	  *@returns The number of available tokens.
	  *@author Mike Buland
	  */
	int getNumTokens();

	/** Gets a processed token specified by index.
	  *@param nIndex The index of the token to retrieve.
	  *@returns A pointer to the requested token.  Please note that these tokens
	  * may not match the original command line.
	  *@author Mike Buland
	  */
	char *getToken( int nIndex );

	/** Gets the original command line based on tokens.  Use this if you want
	  * to perform your own processing on parts of the command line, without
	  * resorting to tokens.
	  * The first character in the returned string will always be
	  * non-whitespace.
	  *@param nIndex The index of the token to start at, zero gets you the whole
	  * command line.
	  *@returns A pointer to the internal original command line string, starting
	  * at the position of the first non-whitespace character of the token
	  * specified.
	  *@author Mike Buland
	  */
	char *getTokenString( int nIndex=0 );

	/** Expands a token, replacing it with the string lpNewToken, but
	  * processing the new string for tokens before performing the replacement
	  *@param nIndex Which token should be replaced.
	  *@param lpNewToken The string to replace the token with.
	  *@author Mike Buland
	  */
	void expandTokenTo( int nIndex, char *lpNewToken );

	/** Inserts a token at any position in the command line.  This does not
	  * effect the original command line.
	  *@param nStart The start of the token in the string lpNewToken. (inclusive)
	  *@param nEnd The end of the token in the string lpToken. (inclusive)
	  *@param lpOldOrig The pointer to the position in the orginal command
	  * line where this new token should point.
	  *@param lpNewToken The string containing the new token.  May contain more
	  * than just one token.
	  *@param nIndex The position to insert the token to.
	  *@author Mike Buland
	  */
	void insertToken( int nStart, int nEnd, char *lpOldOrig, const char *lpNewToken, int nIndex );

private:
	char *lpTokenString;	/**< The original text that this string came from */
	LinkedList lToken;		/**< The list of tokens. */

	/**
	 * A single token within the token string.
	 */
	typedef struct Token
	{
		char *lpOrig;  /**< This is just a pointer back to lpTokenString */
		char *lpToken; /**< This is really a whole token */
	} Token;
};

#endif
