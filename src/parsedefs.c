#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

#include "backend.h"
#include "frontend.h"


int parseAbsValue(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	Range r = ntab->ranges[ntab->defs[defid]];
	
	// {val name (BIND) EXPR }
	
	return 0;
}

int parseValue(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	Range r = ntab->ranges[ntab->defs[defid]];

	// {val name EXPR }
	
	return 0;
}

int parseAbsFunction(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	Range r = ntab->ranges[ntab->defs[defid]];
	
	// {fnc name (BIND) (FNTY) ((FBIND) => EXPR) ... }
	
	return 0;
}

int parseFunction(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	Range r = ntab->ranges[ntab->defs[defid]];

	// {fnc name (FNTY) ((FBIND) => EXPR) ... }
	
	return 0;
}



int parseAbsTypedef(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	Range r = ntab->ranges[ntab->defs[defid]];
	
	// {typ Name ((BIND) => TYPE) }
	
	return 0;
}

int parseTypedef(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	Range r = ntab->ranges[ntab->defs[defid]];

	// {typ Name TYPE }
	
	return 0;
}

int parseAbsFnTypedef(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	Range r = ntab->ranges[ntab->defs[defid]];
	
	// {fty Name ((BIND) => FNTY) }
	
	return 0;
}

int parseFnTypedef(FrontendFile file, int defid){
	TokenList* 	  tkl = file.tkl;
	NodeTable*	 ntab = file.ntab;
	SymbolTable* syms = file.syms;
	Range r = ntab->ranges[ntab->defs[defid]];

	// {fty Name (FNTY) }
	
	return 0;
}




