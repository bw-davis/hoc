#ifndef _CODE_H_
#define _CODE_H_
#include "symbol.h"

typedef union Datum {		/* interpreter stack type */
	double val;
	Symbol *sym;
} Datum;

typedef void (*Inst)();		/* machine instruction */

#define STOP (Inst)0

void initcode();
void push(Datum d);	
Datum pop() ;
Inst *code(Inst f);
void execute(Inst *p);
void constpush();
void varpush();
void add();
void sub();
void mul();
void div();
void negate();
void power();
void eval();
void assign();
void print();
void bltin();
void le();
void gt();
void lt();
void eq();
void ge();
void ne();
void and();
void or();
void not();
void whilecode();
void ifcode();
void prexpr();
void define(Symbol *sp);
void call();
void ret();
void funcret();
void procret();
double *getarg();
void arg();
void argassign();
void prstr();
void varread();
#endif /* _CODE_H_ */
