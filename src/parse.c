#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "frontend.h"


int nodeParser(TokenList* tkl, NodeTable* ntab){
	ntab->nodes = malloc(sizeof(Node) * tkl->tkct);
	ntab->fill  = 0;
	
	TkType* stack = alloca(sizeof(TkType) * tkl->tkct);
	int   stk = 0;
	for(int i = 0; i < tkl->tkct; i++){
		TkType k = tkl->tks[i].kind;
		switch(k){
			case TK_OPN_PAR : {stack[stk] = TK_OPN_PAR; stk++;} break;
			case TK_OPN_BRK : {stack[stk] = TK_OPN_BRK; stk++;} break;
			case TK_OPN_BRC : {stack[stk] = TK_OPN_BRC; stk++;} break;
			case TK_END_PAR : {
				stk--;
				if(stk < 0){
					printf("Unmatched parens\n");
					return 0;
				}else if(stack[stk] != TK_OPN_PAR){
					printf("Missing open parens\n");
					return 0;
				}
			}break;
			case TK_END_BRK : {
				stk--;
				if(stk < 0){
					printf("Unmatched bracket\n");
					return 0;
				}else if(stack[stk] != TK_OPN_PAR){
					printf("Missing open bracket\n");
					return 0;
				}
			}break;
			case TK_END_BRC : {
				stk--;
				if(stk < 0){
					printf("Unmatched brace\n");
					return 0;
				}else if(stack[stk] != TK_OPN_PAR){
					printf("Missing open brace\n");
					return 0;
				}
			}break;
			default : break;
		}
	}
	if(stk > 0){
		printf("%i missing close wrappers\n", stk);
		return 0;
	}
	
	
	// TODO : parsing
	
	
	return 1;
}
