#ifndef _SYMBOL_H_
#define _SYMBOL_H_
typedef struct Symbol {		/* symbol table entry */
	char *name;
	short type;		/* VAR, BLTIN, UNDEF */
	union {
		double val;		/* if VAR */
		double (*ptr)();	/* if BLTIN */
		void (*defn)();		/* FUNCTION, PROCEDURE */
		char *str;		/* STRING */
	} u;
	struct Symbol *next;	/* to link to another */
} Symbol;

Symbol *lookup(char *s);
Symbol *install(char *s, int t, double d);
#endif /* _SYMBOL_H_ */
