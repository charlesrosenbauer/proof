#ifndef __BACKEND_HEADER__
#define __BACKEND_HEADER__


#include "stdint.h"

#include "util.h"


typedef struct{
	int64_t	index;	// opr if positive, sym if negative
	union{
		struct{
			int32_t size;
			int32_t op;
		} opr;
		
		struct{
			int64_t sym;
		} sym;
	};
}Point;

typedef enum{
	OF_VOID		= 0x00,
	OF_COMM		= 0x01,
	OF_ASOC		= 0x02
}OpFlags;

typedef struct{
	int			parct;
	OpFlags		flags;
}Operator;

typedef struct{
	uint64_t	a, b;
}Axiom;




typedef struct{
	uint64_t*	graph;
	Point*		points;
	
	Axiom*		axioms;
	Operator*	ops;
	
	int			gfill, gsize;
	int			pfill, psize;
	int			afill, asize;
	int			ofill, osize;
}Proof;


Proof		initProof	(int, int, int, int);
int			addAxiom	(Proof*, Axiom);
int			addOperator	(Proof*, Operator);
int			isMatch		(Proof*, uint64_t, uint64_t);



#endif
