#include <string.h>

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

static Symbol *symlist = 0;		/* symbol table: linked list */
char *emalloc(unsigned n);
Symbol *lookup( char *s);
Symbol *install(char *s, int t, double d);
