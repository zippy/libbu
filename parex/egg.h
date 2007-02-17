/* 
 *  EGG.H - part of the EGG system.
 *
 *  Global definitions and function prototypes.
 *
 *  By Shawn Hargreaves.
 */


#ifndef __EGG_H__
#define __EGG_H__


#ifndef TRUE 
#define TRUE         -1
#define FALSE        0
#endif

#ifndef MIN
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define MID(x,y,z)   MAX((x), MIN((y), (z)))
#endif

#ifndef ABS
#define ABS(x)       (((x) >= 0) ? (x) : (-(x)))
#endif

#ifndef ALLEGRO_H
struct BITMAP;
#endif



/* possible particle commands (ie. language keywords) */
#define EGG_COMMAND_INIT   1
#define EGG_COMMAND_SET    2
#define EGG_COMMAND_IF     3
#define EGG_COMMAND_LAY    4
#define EGG_COMMAND_DIE    5



/* each particle is controlled by a command list */
typedef struct EGG_COMMAND
{
   int type;                        /* command type */
   int line;                        /* source line number */
   char *var;                       /* variable to be set */
   char *exp;                       /* arithmetic expression */
   struct EGG_COMMAND *cmd;         /* nested, child command list (if) */
   struct EGG_COMMAND *cmd2;        /* more child commands (else) */
   struct EGG_COMMAND *next;        /* linked list pointer */
} EGG_COMMAND;



/* each type of particle is defined by one of these structures */
typedef struct EGG_TYPE
{
   char *name;                      /* type name */
   EGG_COMMAND *cmd;                /* command list */
   struct EGG_TYPE *next;           /* linked list pointer */
} EGG_TYPE;



/* particles store their state variables in these structures */
typedef struct EGG_VARIABLE
{
   char *name;                      /* variable name */
   double val;                      /* current value */
   struct EGG_VARIABLE *next;       /* linked list pointer */
} EGG_VARIABLE;



/* a specific particle instance */
typedef struct EGG_PARTICLE
{
   int num;                         /* unique ID number */
   EGG_TYPE *type;                  /* type information */
   EGG_VARIABLE *var;               /* list of variables */
   struct EGG_PARTICLE *next;       /* linked list pointer */
} EGG_PARTICLE;



/* everything you need to know about an EGG particle system */
typedef struct EGG
{
   int frame;                       /* animation frame number */
   int part_count;                  /* number of active particles */
   int part_num;                    /* particle ID counter */
   EGG_PARTICLE *part;              /* list of particle instances */
   EGG_TYPE *type;                  /* list of particle types */
   EGG_VARIABLE *var;               /* list of global variables */
} EGG;



/* script loading functions */
EGG *load_egg(char *filename, char *error);
EGG *use_egg(void *data, int length, char *error);

/* script unloading function */
void destroy_egg(EGG *egg);

/* script interpreter */
int update_egg(EGG *egg, char *error);

/* renders the current animation state into an Allegro bitmap */
void lay_egg(EGG *egg, struct BITMAP *bmp);

/* internal utility functions */
double evaluate(char *equation, int *error, double (*variable)(char *name));

int process_egg_cmd(EGG *egg, EGG_PARTICLE *part, EGG_PARTICLE *other, EGG_COMMAND *cmd, int init, int set, char *error);
void destroy_egg_cmd(EGG_COMMAND *cmd);

double get_egg_variable(EGG_PARTICLE *part, EGG_PARTICLE *other, EGG *egg, char *name);
void set_egg_variable(EGG_PARTICLE *part, EGG_PARTICLE *other, EGG *egg, char *name, double value);



#endif      /* __EGG_H__ */

