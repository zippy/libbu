/* 
 *  EVALTEST.C - part of the EGG system.
 *
 *  Test program for the expression evaluator.
 *
 *  By Shawn Hargreaves.
 */


#define USE_CONSOLE


#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "egg.h"



int main()
{
   char buf[256];
   double result;
   int error;

   srand(time(NULL));

   printf("\nTest program for the EGG expression evaluator module.\n\nEnter a formula, or a blank line to quit.\n\n\n");

   for (;;) {
      printf("> ");
      fflush(stdout);

      if (!fgets(buf, sizeof(buf), stdin) || (!buf[0]))
	 break;

      result = evaluate(buf, &error, NULL);

      if (error)
	 printf("\nError in expression!\n\n\n");
      else 
	 printf("\nevaluate(\"%s\") = %f\n\n\n", buf, result);
   }

   printf("\n\n");

   return 0;
}

