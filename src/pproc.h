#ifndef PPROC_H_
#define PPROC_H_

/**
 * A mask to discover what the type is even if flags are set.
 */
#define PPROC_TYPE				0x0F

#define PPROC_BOOL_TRUE			0x01
#define PPROC_BOOL_FALSE		0x02
#define PPROC_BOOL_TOGGLE		0x03
#define PPROC_CHAR				0x04
#define PPROC_SHORT				0x05
#define PPROC_LONG				0x06
#define PPREC_LONG_LONG			0x07
#define PPROC_UCHAR				0x08
#define PPROC_USHORT			0x09
#define PPROC_ULONG				0x0A
#define PPREC_ULONG_LONG		0x0B
#define PPROC_FLOAT				0x0C
#define PPROC_DOUBLE			0x0D
#define PPROC_LONG_DOUBLE		0x0E
#define PPROC_STRING			0x0F

#define PPROCF_CALLBACK			0x10
#define PPROCF_ALLOW_EQUALS		0x20
#define PPROCF_SHORT_TERMINAL	0x40
#define PPROCF_TERMINATE		0x80


/**
 * Contains all required info to handle a single program parameter.
 *@author Mike Buland
 */
typedef struct PPROC
{
	const char *lpWord;	/**< The full text-word to use as a param. */
	const char cChar;	/**< The short char version of the param. */

	const char cMode;	/**< One of the PPROC_* macros, these are not flags. */

	/**
	 * Pointer to the function to call when this param is triggered.
	 *@param argc The number of params after and including the one that
	 * triggered this call.
	 *@param argv The array of commandline tokens to use as parameters.
	 *@returns 0 for everything is ok.  A number greater than zero signals that
	 * this parameter function used n parameters and they should be skipped by
	 * the processParams function.
	 */
	int (*proc)( int argc, char *argv[] );
	void *stateVar;	/**< A pointer to a variable to set */
	const char *shortHelp;
} PPROC;

/**
 * Process command line parameters based on a null-terminated array of PPROC
 * structures.
 *@param argc Should come straight from your main function's argc.
 *@param argv Should come straight from your main function's argv.
 *@param pproc The array of params that this function can respond to.
 */
void processParams( int argc, char *argv[], PPROC *pproc );

#endif /*PPROC_H_*/
