%{
#include "hoc.h"
#include <stdio.h>
extern double Pow();
%}
%union {
	double val;		/* actual value */
	Symbol *sym;		/* symbol table pointer */
}
%token	<val>	NUMBER
%token	<sym>	VAR BLTIN UNDEF
%type	<val>	expr asgn
%right 	'='
%left 	'+' '-'
%left 	'*' '/'
%left	UNARYMINUS
%right 	'^'	/* exponentiation */
%%
list:		/* nothing */
	| list '\n'
	| list asgn '\n'
	| list expr '\n'	{ printf("\t%.8g\n", $2); }
	| list error '\n'	{ yyerrok; }
	;
asgn:	 VAR '=' expr { $$=$1->u.val=$3; $1->type = VAR; }
	;
expr:	 NUMBER
	| VAR { if ($1->type == UNDEF)
			execerror("undefinded variable", $1->name);
		$$ = $1->u.val; }
	| asgn
	| BLTIN '(' expr ')'	{ $$ = (*($1->u.ptr))($3); }
	| expr '+' expr 	{ $$ = $1 + $3; }
	| expr '-' expr		{ $$ = $1 - $3; }
	| expr '*' expr		{ $$ = $1 * $3; }
	| expr '/' expr		{
			if ($3 == 0.0)
				execerror("division by zero", "");
			$$ = $1 / $3; }
	| expr '^' expr		{ $$ = Pow($1, $3); }
	| '(' expr ')'		{ $$ = $2; }
	| '-' expr %prec UNARYMINUS	{ $$ = -$2; }
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
        yyparse();
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
                ungetc(c, stdin);
                scanf("%lf", &yylval.val);
                return NUMBER;
        }
        /*if (islower(c)) {
                yylval->sym = c - 'a';          ASCII only 
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







