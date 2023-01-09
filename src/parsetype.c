#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

#include "backend.h"
#include "frontend.h"




int parseTypeSum (NodeTable* p, int range){
	// Typ | Typ | Typ ...
	return 0;
}

int parseTypeProd(NodeTable* p, int range){
	// Typ & Typ & Typ ...
	return 0;
}

int parseTypeFunc(NodeTable* p, int range){
	// Typ -> Typ -> Typ ...
	return 0;
}

int parseTypeTerm(NodeTable* p, int range){
	// TyId
	// ( Typ )
	return 0;
}


int parseType(NodeTable* p, int range){
	return 0;
}
