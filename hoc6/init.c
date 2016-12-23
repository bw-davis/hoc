#include "init.h"
#include "symbol.h"
#include "mymath.h"
#include "code.h"
#include "x.tab.h"
#include <math.h>

static struct {
	char *name;
	double cval;
} consts[] = {
	{"PI",		3.14159265358979323846},
	{"E",		2.71828182845904523536},
	{"GAMMA",	0.57721566490153286060}, /* Euler */
	{"DEG",	       57.29577951308232084680}, /* deg/radian */
	{"PHI",		1.61803398874989484820}, /* golden ratio */
	{NULL,		0.0}
};

static struct {
	char *name;
	double (*func)();
} builtins[] = {
	{"sin",		sin},
	{"cos",		cos},
	{"atan",	atan},
	{"log", 	Log},	/* checks argument */
	{"log10",	Log10},	/* checks argument */
	{"exp", 	Exp},	/* checks argument */
	{"sqrt",	Sqrt},	/* checks argument */
	{"int",		integer},
	{"abs",		fabs},
	{NULL,		NULL}
};

static struct {			/* Keywords */
	char *name;
	int kval;
} keywords[] = {
	{"if", 		IF},
	{"else", 	ELSE},
	{"while", 	WHILE},
	{"print",	PRINT},
	{"proc",	PROC},
	{"func",	FUNC},
	{NULL,		0}
};

void init() {		/* install constants and built-ins in table */
	int i;
	Symbol *s;

	for (i = 0; consts[i].name; i++){
		install(consts[i].name, VAR, consts[i].cval);
	}
	for (i = 0; builtins[i].name; i++){
		s = install(builtins[i].name, BLTIN, 0.0);
		s->u.ptr = builtins[i].func;
	}
	for (i = 0; keywords[i].name; i++) {
		install(keywords[i].name, keywords[i].kval, 0.0);
	}
		
}
