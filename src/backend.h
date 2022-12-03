#ifndef __BACKEND_HEADER__
#define __BACKEND_HEADER__


#include "util.h"


typedef enum{
	// Terms, constructors
	BK_NIL		= 0x000,
	BK_TERM		= 0x001,
	BK_TYPE		= 0x002,
	BK_NUM		= 0x003,
	BK_PAR		= 0x004,
	BK_LIST		= 0x005,
	BK_TPRD		= 0x006,
	BK_TSUM		= 0x007,
	BK_BIND		= 0x008,

	// Statements
	BK_FORALL	= 0x100,
	BK_EXISTS	= 0x101,
	BK_AND		= 0x102,
	BK_OR		= 0x103,
	BK_NOT		= 0x104,
	BK_FUNC		= 0x105,
	BK_APPLY	= 0x106,
	
	// Proof combinators
	BK_MATCH	= 0x200,
	BK_SPLIT	= 0x201,
	BK_CASE		= 0x202,
	BK_CONTRA	= 0x203,
	BK_ASSUME	= 0x204,
	BK_CONJCT	= 0x205,
	BK_EQUIV	= 0x206
}BCKind;

typedef struct{
	uint32_t	a, b, c;
	BCKind		kind;
}Bytecode;


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
