#include "support.h"
#include <stdio.h>


extern void execerror(char *s, char *t);

char *emalloc (unsigned n){
	char *p;
	char *malloc();
	p = malloc(n);
	if (p == 0){
		execerror("out of memory", (char *) 0);
	}
	return p;
}
