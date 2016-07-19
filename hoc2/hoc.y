%{
#include <stdio.h>
double mem[26];		/* memory for variables 'a'..'z' */
%}
%union {		/* stack type */
	double val;	/* actual value */
	int index;	/* index into mem[] */
}
%token	<val>	NUMBER
%token	<index>	VAR
%type	<val>	expr
%right	'='
%left	'+' '-'		/*left associative, same precedence*/
%left	'*' '/'		/*left assoc., higher precedence*/
%left	UNARYMINUS	/* new */
%%
list:	  /*nothing*/
	| list '\n'
	| list expr '\n'	{ printf("\t%.8g\n", $2); }
	| list error '\n'	{ yyerrok; }
	;
expr:	  NUMBER		{ $$ = $1; }
	| VAR			{ $$ = mem[$1]; }
	| VAR '=' expr		{ $$ = mem[$1] = $3; }
	| expr '+' expr		{ $$ = $1 + $3; }
	| expr '-' expr		{ $$ = $1 - $3; }
	| expr '*' expr		{ $$ = $1 * $3; }
	| expr '/' expr		{
		if ($3 == 0.0)
			execerror("division by zero", "");
		$$ = $1 / $3; }
	| '(' expr ')'		{ $$ = $2; }
	| '-' expr      %prec UNARYMINUS        { $$ = -$2; }
	;
%%
	/* end of grammar */
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
jmp_buf begin;
char *progname;		/* for error messages */
int lineno = 1;

int yylex();
void yyerror(char *s);
int warning(char *s, char *t);
void fpecatch(int sig);
int main(int argc, char *argv[])	/* hoc2 */
{
	//int fpecatch();
	
	progname = argv[0];
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	yyparse();
}

void execerror(char *s, char *t)		/* recover run-time error */
{
	warning(s, t);
	longjmp(begin, 0);
}

void fpecatch(int sig)			/* catch floating point exceptions */
{
	execerror("floating point exception", (char *) 0);
}

int yylex()			/* hoc1 */
{
	int c;
	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (c == '.' || isdigit(c)) {		/* number */
		ungetc(c, stdin);
		scanf("%lf", &yylval.val);
		return NUMBER;
	}
	if (islower(c)) {
		yylval.index = c - 'a';		/* ASCII only */
		return VAR;
	}
	if (c == '\n')
		lineno++;
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
