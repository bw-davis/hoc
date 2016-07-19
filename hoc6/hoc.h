#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

typedef struct Symbol {		/* symbol table entry */
	char *name;
	short type;		/* VAR, BLTIN, UNDEF */
	union {
		double val;		/* if VAR */
		double (*ptr)();	/* if BLTIN */
		int (*defn)();		/* FUNCTION, PROCEDURE */
		char *str;		/* STRING */
	} u;
	struct Symbol *next;	/* to link to another */
} Symbol;
Symbol *install(), *lookup();

typedef union Datum {		/* interpreter stack type */
	double val;
	Symbol *sym;
} Datum;

extern Datum pop();

typedef int (*Inst)();		/* machine instruction */
#define STOP	(Inst) 0

extern Inst prog[], *progp, *code(), *progbase;
extern void eval(),  add(),  sub(),  mul(),  div(),  negate(),  power();
extern void assign(),  bltin(),  varpush(),  constpush(),  print(), varread();
extern void prexpr(), prstr();
extern void gt(), lt(), eq(), ge(), le(), ne(), and(), or(), not();
extern void ifcode(), whilecode(), call(), arg(), argassign();
extern void funcret(), procret();
void defnonly(char *s);
void execerror(char *s, char *t);
void fpecatch(int sig);
int follow(expect, ifyes, ifno);
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
