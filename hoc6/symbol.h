#ifndef _SYMBOL_H_
#define _SYMBOL_H_

typedef struct Symbol {		/* symbol table entry */	//USED IN HOC.Y CODE.C INIT.C
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

Symbol *lookup(char *s);
Symbol *install(char *s, int t, double d);

#endif /* _SYMBOL_H_ */