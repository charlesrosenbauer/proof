#ifndef __BACKEND_HEADER__
#define __BACKEND_HEADER__


#include "util.h"


typedef struct{
	uint32_t id;
	
	/*
		list of fields
		list of operations
		list of axioms
	*/
}Model;

typedef struct{
	uint32_t id;
	
	/*
		assumed statement
		implied statement
		list of expressions
	*/
}Theorem;

typedef struct{
	Theorem*	ts;
	int			tct, tcap;
	
	Model*		ms;
	int			mct, mcap;
}MathTable;






#endif
