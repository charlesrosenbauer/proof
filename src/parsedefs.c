#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

#include "backend.h"
#include "frontend.h"


int parseDefine(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	
	// { def TYP DX DX DX AX AX AX }
	
	Range r = ntab->ranges[ntab->defs[defid]];
	NodeKind header[2] = {NK_ID , NK_TYP};
	if(matchPattern(ntab, r, header, 2)){
		printf("DEF\n");
		return !checkKeyword(file, r.root, "def", 3);
	}else{
		return 0;
	}
	
	return 0;
}

int parseTheorem(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;

	// { thm ID (TMS) -> (TMS) : XP }
	
	Range r = ntab->ranges[ntab->defs[defid]];
	NodeKind header[2] = {NK_ID , NK_ID};
	if(matchPattern(ntab, r, header, 2)){
		printf("THEOREM\n");
		return !checkKeyword(file, r.root, "theorem", 7);
	}else{
		return 0;
	}

	return 0;
}

int parseValue(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;

	// { val ID XP }
	
	Range r = ntab->ranges[ntab->defs[defid]];
	NodeKind header[2] = {NK_ID , NK_ID};
	if(matchPattern(ntab, r, header, 2)){
		printf("VAL\n");
		return !checkKeyword(file, r.root, "val", 3);
	}else{
		return 0;
	}

	return 0;
}
