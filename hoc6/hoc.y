%{
#include "hoc.h"
/*#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>*/
#define code2(c1,c2)	code(c1); code(c2)
#define code3(c1,c2,c3)	code(c1); code(c2); code(c3)
%}
%union {
	Symbol 	*sym;		/* symbol table pointer */
	Inst 	*inst;		/* machine instruction */
	int		narg;		/* number of arguments */
}
%token	<sym>	NUMBER STRING PRINT VAR BLTIN UNDEF WHILE IF ELSE
%token	<sym>	FUNCTION PROCEDURE RETURN FUNC PROC READ
%token	<narg>	ARG
%type	<inst>	expr stmt asgn prlist stmtlist
%type 	<inst>  cond while if begin end
%type	<sym>	procname
%type	<narg>	arglist
%right 	'='
%left	OR
%left	AND
%left	GT GE LT LE EQ NE
%left 	'+' '-'
%left 	'*' '/'
%left	UNARYMINUS NOT
%right 	'^'	/* exponentiation */
%%
list:		/* nothing */
	| list '\n'
	| list defn '\n'
	| list asgn '\n'	{ code2(pop, STOP); return 1; }
	| list stmt '\n'	{ code(STOP); return 1; }
	| list expr '\n'	{ code2(print, STOP); return 1; }
	| list error '\n'	{ yyerrok; }
	;
asgn:	  VAR '=' expr 		{ code3(varpush,(Inst)$1, assign); $$=$3; }
	| ARG '=' expr 		{ defnonly("$"); code2(argassign,(Inst)$1); $$=$3; }
	;
stmt:	expr			{ code(pop); }
	| RETURN		{ defnonly("return"); code(procret); }
	| RETURN expr		{ defnonly("return"); $$=$2; code(funcret); }
	| PROCEDURE begin '(' arglist ')'
				{ $$ = $2; code3(call, (Inst)$1, (Inst)$4); }
	| PRINT prlist		{ $$ = $2; }
	/*| PRINT expr		{ code(prexpr); $$ = $2; }*/
	| while cond stmt end {
			($1)[1] = (Inst)$3;		/* body of loop */
			($1)[2] = (Inst)$4; }		/* end, if cond fails */
	| if cond stmt end {				/* else-less if */
			($1)[1] = (Inst)$3;             /* then part */
                        ($1)[3] = (Inst)$4; }		/* end, if cond fails */	
	| if cond stmt end ELSE stmt end {		/* if with else */
			($1)[1] = (Inst)$3;		/* then part */
			($1)[2] = (Inst)$6; 		/* elsepart */
			($1)[3] = (Inst)$7; }		/* end, if cond fails */
	| '{' stmtlist '}'	{ $$ = $2; }
	;
cond:	  '(' expr ')'		{ code(STOP); $$ = $2; }
	;
while:	  WHILE { $$ = code3(whilecode, STOP, STOP); }
	;
if:	  IF	{ $$=code(ifcode); code3(STOP, STOP, STOP); }
	;
begin:		/* nothing */	{ $$ = progp; }
	;
end:	  /* nothing */
stmtlist:	/* nothing */	{ $$ = progp; }
	| stmtlist '\n'
	| stmtlist stmt
	;

expr:	 NUMBER		{ $$ = code2(constpush, (Inst)$1); }
	| VAR 		{ $$ = code3(varpush, (Inst)$1, eval); }
	| ARG		{ defnonly("$"); $$ = code2(arg, (Inst)$1); }
	| asgn
	| FUNCTION begin '(' arglist ')'
				{ $$ = $2; code3(call, (Inst)$1, (Inst)$4); }
	| READ '(' VAR ')'	{ $$ = code2(varread, (Inst)$3); }
	| BLTIN '(' expr ')'	{ $$=$3; code2(bltin, (Inst)$1->u.ptr); }
	| '(' expr ')'		{ $$ = $2; }
	| expr '+' expr 	{ code(add); }
	| expr '-' expr		{ code(sub); }
	| expr '*' expr		{ code(mul); }
	| expr '/' expr		{ code(div); }
	| expr '^' expr		{ code(power); }
	| '-' expr %prec UNARYMINUS	{ $$ = $2; code(negate); }
	| expr GT expr		{ code(gt); }
	| expr GE expr		{ code(ge); }
	| expr LT expr		{ code(lt); }
	| expr LE expr		{ code(le); }
	| expr EQ expr		{ code(eq); }
	| expr NE expr		{ code(ne); }
	| expr AND expr 	{ code(and); }
	| expr OR expr		{ code(or); }
	| NOT expr		{ $$ = $2; code(not); }
	;
prlist:	 expr			{ code(prexpr); }
	| STRING		{ $$ = code2(prstr, (Inst)$1); }
	| prlist ',' expr	{ code(prexpr); }
	| prlist ',' STRING	{ code2(prstr, (Inst)$3); }
	;
defn:	 FUNC procname		{ $2->type=FUNCTION; indef=1; }
	   '(' ')' stmt		{ code(procret); define($2); indef=0; }
	| PROC procname		{ $2->type=PROCEDURE; indef=1; }
	   '(' ')' stmt		{ code(procret); define($2); indef=0; }
	;
procname: VAR
	| FUNCTION
	| PROCEDURE
	;
arglist:	/* nothing */	{ $$ = 0; }
	| expr			{ $$ = 1; }
	| arglist ',' expr	{ $$ = $1 + 1; }
	;
	
%%

int lineno = 1;
/*int yylex();
void yyerror(char *s);
int warning(char *s, char *t);
void fpecatch(int sig);
int backslash(int c);*/
int main(int argc, char *argv[])        /* hoc2 */
{
        int i;

  	progname = argv[0];
	if (argc == 1) {		/* fake an argument list */
		static char *stdinonly[] = { "-" };
		gargv = stdinonly;
		gargc = 1;
	} else {
		gargv = argv+1;
		gargc = 1;
	}
	init();
	while (moreinput())
		run();
	return 0;
}
void defnonly(char *s) {		/* warns if illegal definition */
	if (!indef)
		execerror(s, "used outside definition");
}
void execerror(char *s, char *t)                /* recover run-time error */
{
        warning(s, t);
	fseek(fin, 0L, 2);		/* flush rest of file */
        longjmp(begin, 0);
}

void fpecatch(int sig)                  /* catch floating point exceptions */
{
        execerror("floating point exception", (char *) 0);
}
int follow(int expect, int ifyes, int ifno) {
	int c = getc(fin);

	if (c == expect)
		return ifyes;
	ungetc(c, stdin);
	return ifno;
}
int yylex()                     /* hoc1 */
{
        //int c;
        while ((c = getc(fin)) == ' ' || c == '\t')
                ;
        if (c == EOF)
        	return 0;
        if (c == '.' || isdigit(c)) {           /* number */
                double d;
		ungetc(c, fin);
                fscanf(fin, "%lf", &d);
		yylval.sym = install("", NUMBER, d);
                return NUMBER;
        }
        /*if (islower(c)) {
                yylval.sym = c - 'a';          ASCII only 
                return VAR;
        }
        if (c == '\n')
                lineno++;*/
	if (isalpha(c)){
		Symbol *s;
		char sbuf[100], *p = sbuf;
		do {
			if (p >= sbuf + sizeof(sbuf) - 1) {
				*p = '\0';
				execerror("name too long", sbuf);
			}
			*p++ = c;
		}while ((c=getc(fin)) != EOF && isalnum(c));
		ungetc(c, fin);
		*p = '\0';
		if ((s=lookup(sbuf)) == 0)
			s = install(sbuf, UNDEF, 0.0);
		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
	}
	if (c == '$') { 	/* argument? */
		int n = 0;
		while (isdigit(c=getc(fin)))
			n = 10 * n + c - '0';
		ungetc(c, fin);
		if (n == 0)
			execerror("strange $...", (char *)0);
		yylval.narg = n;
		return ARG;
	}
	if (c == '"') {		/* quoted string */
		char sbuf[100], *p, *emalloc();
		for (p = sbuf; (c=getc(fin)) != '"'; p++) {
			if (c == '\n' || c == EOF)
				execerror("missing quote", "");
			if (p >= sbuf + sizeof(sbuf) - 1) {
				*p = '\0';
				execerror("string too long", sbuf);
			}
			*p = backslash(c);
		}
		*p = 0;
		yylval.sym = (Symbol *)emalloc(strlen(sbuf)+1);
		strcpy((char *)yylval.sym, sbuf);
		return STRING;
	}
	 switch(c) {
                case '>':       return follow('=', GE, GT);
                case '<':       return follow('=', LE, LT);
                case '=':       return follow('=', EQ, '=');
                case '!':       return follow('=', NE, NOT);
                case '|':       return follow('|', OR, '|');
                case '&':       return follow('&', AND, '&');
                case '\n':      lineno++; return '\n';
                default:        return c;
        }

        return c;
}
int backslash(int c) {		/* get next char with \'s interpreted */
	char *strchr();		/* might need to use 'index() */
	static char transtab[] = "b\bf\fn\nr\rt\t";
	if (c != '\\')
		return c;
	c = getc(fin);
	if (islower(c) && strchr(transtab, c))
		return index(transtab, c)[1];
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
	if (infile)
		fprintf(stderr, " in %s", t);
        fprintf(stderr, " near line %d\n", lineno);
	while (c != '\n' && c != EOF)
		c = getc(fin);		/* flush rest of input line */
	if (c == 'n')
		lineno++;
}
int moreinput() {
	if (gargc-- <= 0)
		return 0;
	if (fin && fin != stdin)
		fclose(fin);
	infile = *gargv++;
	lineno = 1;
	if (strcmp(infile, "-") == 0) {
		fin = stdin;
		infile = 0;
	} else if ((fin=fopen(infile, "r")) == NULL) {
		fprintf(stderr, "%s: can't open %s\n", progname, infile);
		return moreinput();
	}
	return 1;
}
void run() {		/* execute until EOF */
	setjmp(begin);
	signal(SIGFPE, fpecatch);
	for (initcode(); yyparse(); initcode())
		execute(progbase);
}




