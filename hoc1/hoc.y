%{
#define YYSTYPE double  /*data type of yacc stack*/
#include <stdio.h>
%}
%token NUMBER
%left	'+' '-'		/*left associative, same precedence*/
%left	'*' '/'		/*left assoc., higher precedence*/
%left	UNARYMINUS	/* new */
%%
list:	  /*nothing*/
	| list '\n'
	| list expr '\n'	{ printf("\t%.8g\n", $2); }
	;
expr:	  NUMBER		{ $$ = $1; }
	| '-' expr	%prec UNARYMINUS	{ $$ = -$2; }	/* new */
	| expr '+' expr		{ $$ = $1 + $3; }
	| expr '-' expr		{ $$ = $1 - $3; }
	| expr '*' expr		{ $$ = $1 * $3; }
	| expr '/' expr		{ $$ = $1 / $3; }
	| '(' expr ')'		{ $$ = $2; }
	;
%%
	/* end of grammar */

#include <stdio.h>
#include <ctype.h>
char *progname;		/* for error messages */
int lineno = 1;

int yylex();
void yyerror(s);
void warning(s, t);
main(argc, argv)	/* hoc1 */
	char *argv[];
{
	progname = argv[0];
	yyparse();
}

int yylex()			/* hoc1 */
{
	int c;
	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (c == '.' || isdigit(c)) {		/* number */
		ungetc(c, stdin);
		scanf("%lf", &yylval);
		return NUMBER;
	}
	if (c == '\n')
		lineno++;
	return c;
}

void yyerror(s)
	char *s;
{
	warning(s, (char *) 0);
}

void warning(s, t)
	char *s, *t;
{
	fprintf(stderr, "%s; %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}
