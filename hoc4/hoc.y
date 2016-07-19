%{
#include "hoc.h"
#include <stdio.h>
#define code2(c1,c2)	code(c1); code(c2)
#define code3(c1,c2,c3)	code(c1); code(c2); code(c3)
//extern double Pow();
%}
%union {
	Symbol 	*sym;		/* symbol table pointer */
	Inst 	*inst;		/* machine instruction */
}
%token	<sym>	NUMBER VAR BLTIN UNDEF
%right 	'='
%left 	'+' '-'
%left 	'*' '/'
%left	UNARYMINUS
%right 	'^'	/* exponentiation */
%%
list:		/* nothing */
	| list '\n'
	| list asgn '\n'	{ code2(pop, STOP); return 1; }
	| list expr '\n'	{ code2(print, STOP); return 1; }
	| list error '\n'	{ yyerrok; }
	;
asgn:	 VAR '=' expr { code3(varpush,(Inst)$1, assign); }
	;
expr:	 NUMBER		{ code2(constpush, (Inst)$1); }
	| VAR { code3(varpush, (Inst)$1, eval); }
	| asgn
	| BLTIN '(' expr ')'	{ code2(bltin, (Inst)$1->u.ptr); }
	| '(' expr ')'
	| expr '+' expr 	{ code(add); }
	| expr '-' expr		{ code(sub); }
	| expr '*' expr		{ code(mul); }
	| expr '/' expr		{ code(div); }
	| expr '^' expr		{ code(power); }
	| '-' expr %prec UNARYMINUS	{ code(negate); }
	;
%%
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
jmp_buf begin;
char *progname;         /* for error messages */
int lineno = 1;

int yylex();
void yyerror(char *s);
int warning(char *s, char *t);
void fpecatch(int sig);
int main(int argc, char *argv[])        /* hoc2 */
{
        //int fpecatch();

        progname = argv[0];
	init();
        setjmp(begin);
        signal(SIGFPE, fpecatch);
        for (initcode(); yyparse(); initcode())
		execute(prog);
	return 0;
}

void execerror(char *s, char *t)                /* recover run-time error */
{
        warning(s, t);
        longjmp(begin, 0);
}

void fpecatch(int sig)                  /* catch floating point exceptions */
{
        execerror("floating point exception", (char *) 0);
}

int yylex()                     /* hoc1 */
{
        int c;
        while ((c = getchar()) == ' ' || c == '\t')
                ;
        if (c == '.' || isdigit(c)) {           /* number */
                double d;
		ungetc(c, stdin);
                scanf("%lf", &d);
		yylval.sym = install("", NUMBER, d);
                return NUMBER;
        }
        /*if (islower(c)) {
                yylval.sym = c - 'a';          ASCII only 
                return VAR;
        }*/
        if (c == '\n')
                lineno++;
	if (isalpha(c)){
		Symbol *s;
		char sbuf[100], *p = sbuf;
		do {
			*p++ = c;
		}while ((c=getchar()) != EOF && isalnum(c));
		ungetc(c, stdin);
		*p = '\0';
		if ((s=lookup(sbuf)) == 0)
			s = install(sbuf, UNDEF, 0.0);
		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
	}
        return c;
}

void yyerror(char *s)
{
        warning(s, (char *) 0);
}

int warning(char *s, char *t)
{
        fprintf(stderr, "%s; %s", progname, s);
        if (t)
                fprintf(stderr, " %s", t);
        fprintf(stderr, " near line %d\n", lineno);
}







