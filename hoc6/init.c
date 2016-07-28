#include "init.h"
#include "symbol.h"

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
