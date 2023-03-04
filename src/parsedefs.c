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
	if(ntab->nodes[r.root         ].kind != NK_ID  ) return 0;
	if(ntab->nodes[r.root+1       ].kind != NK_TYP ) return 0;
	if(ntab->nodes[r.root+r.size-1].kind != NK_XBRC) return 0;
	printf("Function %i\n", defid);
	
	return 1;
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


int parseDef(FrontendFile file, int defid){
	
	int df = -1;
	
	df = parseFnTypedef (file, defid);
	if(df) return df;
	
	df = parseTypedef   (file, defid);
	if(df) return df;
	
	df = parseAbsTypedef(file, defid);
	if(df) return df;
	
	df = parseFunction  (file, defid);
	if(df) return df;

	// Nothing worked
	return -1;
}

