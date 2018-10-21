#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

#define nelem(x) (sizeof(x) / sizeof(x[0]))

#define deg2rad(deg) ((deg)*M_PI / 180)

void usage(void);
void die(char *, ...);
void compile(char *);
void execute(void);
void newfile(char *);

int yylex(void);
void yyerror(const char *);
int yyparse(void);

void *emalloc(size_t);
void *erealloc(void *, size_t);

long lookupcolor(const char *);
long hex(const char *);

void initgins(void);
void setinsbuf(int);
void emit(int, ...);
void gfcall(char *);
void gcall(char *);
void rcall(char *);
void gbloop(int);
void geloop(void);
Func *lookupfunc(char *);

void linkcode(void);
void resetvm(void);
void setresetpc(int);
void setcode(int *, int, int);

void initgfx(void);
void display(void);
