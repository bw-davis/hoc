#include "symbol.h"
#include "hoc.h"
#include <string.h>

static Symbol *symlist = 0;		/* symbol table: linked list */

Symbol *lookup( char *s){		/* find s in symbol table */
	Symbol *sp;

	for (sp = symlist; sp != (Symbol *) 0; sp = sp->next){
		if (strcmp(sp->name, s) == 0){
			return sp;
		}
	}
	return NULL;		/* NULL ==> not found */
}

Symbol *install(char *s, int t, double d){
	Symbol *sp;

	sp = (Symbol *) emalloc(sizeof(Symbol));
	sp->name = (char *)emalloc(strlen(s) + 1);
	strcpy(sp->name, s);
	sp->name = strdup(s);
	sp->type = t;
	sp->u.val = d;
	sp->next = symlist;		/* put at front of list */
	symlist = sp;
	return sp;
}
