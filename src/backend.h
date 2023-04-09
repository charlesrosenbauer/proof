#ifndef __BACKEND_HEADER__
#define __BACKEND_HEADER__


#include "stdint.h"

#include "util.h"


typedef struct{
	uint32_t	kind;
	int32_t	a, b;
}Binop;

typedef struct{
	uint32_t	kind;
	int32_t	size;
	int32_t*	vals;
}Function;

typedef struct{
	Binop*		bops;	// positive values
	Function*	fncs;	// negative values
	
	int			bct, bfill;
	int			fct, ffill;
}Proof;



#endif
