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

typedef uint32_t	BCId;




/*
	We're going to use the same categorization system as Coq for now, albeit
	with explicit abstract variants. Eventually we might want to add more
	categories, but these are the foundation for now.
*/

typedef enum{
	DK_NIL		= 0,
	DK_FNVAL	= 1,
	DK_VAL		= 2,
	DK_FNTYP	= 3,
	DK_TYP		= 4,
	DK_ABFNVAL	= 5,
	DK_ABVAL	= 6,
	DK_ABFNTYP	= 7,
	DK_ABTYP	= 8
}DefKind;

typedef struct{
	DefKind		kind;
	uint32_t	name;
	
	Bytecode*	bcs;
	int			fill, size;
	
	BCId		type;
	BCId		bind, pars, rets;
	int			bdct, prct, rtct;
}Def;

typedef struct{
	Def*		defs;
	int			dfct, dfcap;
	
	uint32_t*	modnames;
	int*		modstarts;
	int			modct;
}MathTable;



//void		printDefinition		(Definition);
void		printMathTable		(MathTable);


#endif
