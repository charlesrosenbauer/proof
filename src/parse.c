#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "frontend.h"


int nodeParser(TokenList* tkl, NodeTable* ntab){
	ntab->nodes = malloc(sizeof(Node) * tkl->tkct);
	ntab->fill  = 0;
	
	// TODO : parsing
	
	return 1;
}
