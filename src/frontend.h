#ifndef __FRONTEND_HEADER__
#define __FRONTEND_HEADER__


#include "stdint.h"

#include "type.h"


/*
	TODO:
	* lexer
	* parser
*/
SymbolTable makeSymbolTable	(int);
void		printSymbolTable(SymbolTable*, char*);

int			parse			(SymbolTable*, uint64_t, char*, int);



#endif
