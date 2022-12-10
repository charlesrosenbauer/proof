#include "stdio.h"
#include "stdlib.h"

#include "frontend.h"
#include "util.h"


int main(int ac, char** av){
	char** 		files = alloca(sizeof(char*)   * ac);
	int64_t*	sizes = alloca(sizeof(int64_t) * ac);
	
	int loadpass = 1;
	for(int i = 1; i < ac; i++){
		if(!loadFile(av[i], (int64_t**)&files[i], &sizes[i])){
			printf("Unable to load file %s\n", av[i]);
			loadpass = 0;
		}
	}
	if(!loadpass) return -1;
	
	
	TokenList*  tkls = alloca(sizeof(TokenList) * ac);
	NodeTable*  ntbs = alloca(sizeof(NodeTable) * ac);
	SymbolTable syms = makeSymbolTable(65536);
	
	for(int i = 1; i < ac; i++){
		tkls[i].text	= files[i];
		tkls[i].fileId	= i;
		tkls[i].filesize= sizes[i];
		
		lexer(&tkls[i]);
		printf("%i tokens\n", tkls[i].tkct);
		
		printTokenList(&tkls[i]);
		tkls[i].tkct--;
		
		if(!nodeParser(&tkls[i], &ntbs[i], &syms)){
			printf("Node parser failed!\n");
			return 0;
		}
		printNodeTable(&ntbs[i]);
	}
	
	
	
}
