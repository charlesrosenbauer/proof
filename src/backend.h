#ifndef __BACKEND_HEADER__
#define __BACKEND_HEADER__


#include "stdint.h"

#include "util.h"


typedef enum{
	// Terms, constructors
	BK_NIL		= 0x000,
	BK_TERM		= 0x001,
	BK_TYPE		= 0x002,
	BK_NUM		= 0x003,
	BK_TPRD		= 0x004,
	BK_TSUM		= 0x005,
	BK_TFUN		= 0x006,
	BK_REC		= 0x007,

	// Logic
	BK_FORALL	= 0x100,
	BK_EXISTS	= 0x101,
	BK_AND		= 0x102,
	BK_OR		= 0x103,
	BK_NOT		= 0x104,
	BK_IMPLIES	= 0x105,
	BK_IFF		= 0x106,
	BK_EQUALS	= 0x107,
	BK_T		= 0x108,
	BK_F		= 0x109,
	
	// Proof combinators
	BK_MATCH	= 0x200,
	BK_SPLIT	= 0x201,
	BK_CASE		= 0x202,
	BK_CONTRA	= 0x203,
	BK_ASSUME	= 0x204,
	BK_CONJCT	= 0x205,
	BK_EQUIV	= 0x206,
	BK_INDUCT	= 0x207
}BCKind;

typedef struct{
	uint32_t	a, b;
	BCKind		kind;
}Bytecode;

typedef struct{
	Bytecode*	code;
	int			fill, size;
}Definition;


typedef struct{
	uint32_t	id;
	uint32_t	def;
	
	/*
		list of fields
		list of operations
		list of axioms
	*/
	uint32_t*	field;
	uint32_t*	fldef;
	
	uint32_t*	op;
	uint32_t*	opdef;
	
	uint32_t*	axiom;
	uint32_t*	axdef;
	
	int			fct, oct, xct;
}Model;

typedef struct{
	uint32_t	id;
	uint32_t	def;
	
	/*
		assumed statement
		implied statement
		list of expressions
	*/
	uint32_t	assumption;
	uint32_t	implication;
	uint32_t*	proof;
	int			pct;
}Theorem;

typedef struct{
	Definition*	ds;
	int			dct, dcap;

	Theorem*	ts;
	int			tct, tcap;
	
	Model*		ms;
	int			mct, mcap;
}MathTable;



void		printDefinition		(Definition);
void		printMathTable		(MathTable);


#endif
