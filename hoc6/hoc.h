#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include "symbol.h"



#define STOP	(Inst) 0

extern Inst prog[], *progp, *code(), *progbase;
extern void eval(),  add(),  sub(),  mul(),  div(),  negate(),  power();
extern void assign(),  bltin(),  varpush(),  constpush(),  print(), varread();
extern void prexpr(), prstr();
extern void gt(), lt(), eq(), ge(), le(), ne(), and(), or(), not();
extern void ifcode(), whilecode(), call(), arg(), argassign();
extern void funcret(), procret();
void define(Symbol *sp);
void init();
int moreinput();
void run();
void initcode();
void execute(Inst *p);
void defnonly(char *s);
void execerror(char *s, char *t);
void fpecatch(int sig);
int follow(int expect, int ifyes, int ifno);
int yylex();
int backslash(int c);
void yyerror(char *s);
int warning(char *s, char *t);
jmp_buf begin;
int indef;
char *infile;			/* input file name */
FILE *fin;				/* input file pointer */
char **gargv;			/* global argument list */
int gargc;
int c;		/* global for use by warning() */
char *progname;         /* for error messages */
//int lineno = 1;
