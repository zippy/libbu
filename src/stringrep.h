#ifndef STRING_REP_H
#define STRING_REP_H

#include <stdint.h>

/**
 * Calculates the length of a string.
 */
int32_t stringlen( const char *s );

/**
 * Identicle to strdup, which isn't available everywhere, but uses c++ memory
 * facilities.  Remember to use delete[] when freeing the returned string.
 */
char *stringdup( const char *s );

#endif
