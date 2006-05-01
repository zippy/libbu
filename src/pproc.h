#ifndef PPROC_H_
#define PPROC_H_

/**
 * Contains all required info to handle a single program parameter.
 *@author Mike Buland
 */
typedef struct PPROC
{
	const char *lpWord;	/**< The full text-word to use as a param. */
	const char cChar;	/**< The short char version of the param. */
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
	bool *stateVar;	/**< A pointer to a bool to be setwhen this is triggered */
	bool bSetState; /**< The state to set the above bool to. */
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
