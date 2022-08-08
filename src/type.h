#ifndef __TYPE_HEADER__
#define __TYPE_HEADER__


#include "stdint.h"


typedef struct{
	int64_t		fileId;
	int64_t		filePos;
}Symbol;

typedef struct{
	uint64_t*	hashes;
	Symbol*		syms;
	int			size, fill;
}SymbolTable;


typedef struct{
	uint64_t	symbol;
	void 		*l, *r;
}Term;


/*
	TODO:
	* allocator table
	* proof table
	* definition table
*/




#endif
