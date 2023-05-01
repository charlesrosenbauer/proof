#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "backend.h"





Proof initProof(int gct, int pct, int act, int oct){
	Proof ret;
	ret.graph	= malloc(sizeof(uint64_t) * gct);
	ret.points	= malloc(sizeof(Point   ) * pct);
	ret.axioms	= malloc(sizeof(Axiom   ) * act);
	ret.ops		= malloc(sizeof(Operator) * oct);
	ret.gfill	= 0;
	ret.gsize	= gct;
	ret.pfill	= 0;
	ret.psize	= pct;
	ret.afill	= 0;
	ret.asize	= act;
	ret.ofill	= 0;
	ret.osize	= oct;

	return ret;
}



int	addAxiom(Proof* p, Axiom a){
	if(p->afill+5 >= p->asize){
		Axiom* tmp = p->axioms;
		p->asize  *= 2;
		p->axioms  = malloc(sizeof(Axiom) * p->asize);
		for(int i  = 0; i < p->afill; i++) p->axioms[i] = tmp[i];
		free(tmp);
	}
	p->axioms[p->afill] = a;
	p->afill++;
	return p->afill-1;
}



int	addOperator(Proof* p, Operator o){
	if(p->ofill+5    >= p->osize){
		Operator* tmp = p->ops;
		p->osize     *= 2;
		p->ops        = malloc(sizeof(Operator) * p->osize);
		for(int i     = 0; i < p->ofill; i++) p->ops[i] = tmp[i];
		free(tmp);
	}
	p->ops[p->ofill] = o;
	p->ofill++;
	return p->ofill-1;
}



int	isMatch(Proof* p, uint64_t a, uint64_t b){
	// match a over b
	return 0;
}
