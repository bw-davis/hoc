#ifndef _CODE_H
#define _CODE_H

#include "symbol.h"

#define STOP	(Inst) 0		//used by hoc.y and code.c
	

typedef union datum {		/* interpreter stack type */
	double val;
	Symbol *sym;
}Datum;

typedef void (*Inst)();		/* machine instruction */
extern Inst prog[], *progp, *progbase;

Datum pop();
void push(Datum d);
void initcode(); 	
void eval(),  add(),  sub(),  mul(),  divide(),  negate(),  power();	
void assign(),  bltin(),  varpush(),  constpush(),  print();
void prexpr(), prstr();	
void gt(), lt(), eq(), ge(), le(), ne(), and(), or(), not(); 
void ifcode(), whilecode(), call(), arg(), argassign();
void funcret(), procret(); 
void define(Symbol *sp);
void execute(Inst *p);
Inst *code(Inst f);	


#endif /* _CODE_H_ */