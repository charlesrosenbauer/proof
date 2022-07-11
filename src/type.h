#ifndef __TYPE_HEADER__
#define __TYPE_HEADER__


#include "stdint.h"


typedef struct{
	uint64_t hash;
	uint64_t sym;
	void *l, *r;
}Icon;


typedef struct{
	uint64_t*	syms;
	Icon*		icos;
	int			size, fill;
}SymbolTable;




#endif
