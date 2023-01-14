#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

#include "backend.h"
#include "frontend.h"




int parseLogForall(NodeTable* p, int range){
	// [forall BIND : EXPR]
	NodeKind ks[5] = (NodeKind){ NK_ID, NK_WILD, NK_COLON, NK_WILD, NK_XBRK};
	if(!matchPattern(p, p->ranges[range], ks, 5)) return 0;
	
	// TODO: assemble
	
	return 1;
}

int parseLogExists(NodeTable* p, int range){
	// [exists BIND : EXPR]
	NodeKind ks[5] = (NodeKind){ NK_ID, NK_WILD, NK_COLON, NK_WILD, NK_XBRK};
	if(!matchPattern(p, p->ranges[range], ks, 5)) return 0;
	
	// TODO: assemble
	
	return 1;
}

int parseLogAnd(NodeTable* p, int range){
	// (EXPR & EXPR)
	return 0;
}

int parseLogOr (NodeTable* p, int range){
	// (EXPR | EXPR)
	return 0;
}

int parseLogNot(NodeTable* p, int range){
	// (! EXPR)
	return 0;
}

int parseLogImp(NodeTable* p, int range){
	// (EXPR -> EXPR)
	return 0;
}

int parseLogIff(NodeTable* p, int range){
	// (EXPR <> EXPR)
	return 0;
}

int parseLogEql(NodeTable* p, int range){
	// (EXPR = EXPR)
	return 0;
}

int parseLogTerm(NodeTable* p, int range){
	// ID | TYID | NUM
	return 0;
}


int parseLog(NodeTable* p, int range){
	int x;
	x = parseLogTerm  (p, range);
	if(x) return x;
	
	x = parseLogNot   (p, range);
	if(x) return x;
	
	x = parseLogAnd   (p, range);
	if(x) return x;
	
	x = parseLogOr    (p, range);
	if(x) return x;
	
	x = parseLogImp   (p, range);
	if(x) return x;
	
	x = parseLogEql   (p, range);
	if(x) return x;
	
	x = parseLogIff   (p, range);
	if(x) return x;
	
	x = parseLogForall(p, range);
	if(x) return x;
	
	x = parseLogExists(p, range);
	if(x) return x;
	
	return 0;
}
