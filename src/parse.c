#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "frontend.h"


int newNode(NodeTable* ntab){
	ntab->fill++;
	return ntab->fill-1;
}


int subParser(TokenList* tkl, NodeTable* ntab, SymbolTable* stab, int start, int depth){
	// TODO : parsing
	for(int i = start; i < tkl->tkct; i++){
		TkType k  = tkl->tks[i].kind;
		TkType kn = (i+1 < tkl->tkct)? tkl->tks[i+1].kind : TK_NIL;
		
		int   nix = 0;
		switch(k){
			case TK_OPN_PAR : {
				if(kn == TK_END_PAR){
					nix = newNode(ntab);
					ntab->nodes[nix] = (Node){.next=0, .kind=NK_PNIL};
					i++;
				}else{
					int ret = subParser(tkl, ntab, stab, i+1, depth+1);
					if( ret < 0) return -1;
					if(ntab->fill-1 != nix){
						ntab->nodes[nix].kind = NK_PAR;
						ntab->nodes[nix].next = nix+1;
					}
					i   = ret;
				}
			}break;
			case TK_OPN_BRK : {
				if(kn == TK_END_PAR){
					nix = newNode(ntab);
					ntab->nodes[nix] = (Node){.next=0, .kind=NK_KNIL};
					i++;
				}else{
					int ret = subParser(tkl, ntab, stab, i+1, depth+1);
					if( ret < 0) return -1;
					if(ntab->fill-1 != nix){
						ntab->nodes[nix].kind = NK_BRK;
						ntab->nodes[nix].next = nix+1;
					}
					i   = ret;
				}
			}break;
			case TK_OPN_BRC : {
				if(kn == TK_END_PAR){
					nix = newNode(ntab);
					ntab->nodes[nix] = (Node){.next=0, .kind=NK_CNIL};
					i++;
				}else{
					int ret = subParser(tkl, ntab, stab, i+1, depth+1);
					if( ret < 0) return -1;
					if(ntab->fill-1 != nix){
						ntab->nodes[nix].kind = NK_BRC;
						ntab->nodes[nix].next = nix+1;
					}
					i   = ret;
				}
			}break;
			
			case TK_END_PAR :
			case TK_END_BRK :
			case TK_END_BRC : {
				return i;
			}break;
			
			case TK_TYID	:
			case TK_ID		: {
				// Get symbol from symbol table
				int pos = tkl->tks[i].pos;
				int len = tokenLen(tkl, i);
				int sym = insertSymbol(stab, tkl->text, pos, len, tkl->filesize, (Symbol){.fileId=tkl->fileId, .filePos=pos});
			}break;
			case TK_NUM		: {
				// Parse number
			}break;
			case TK_COM		: {
				// Probably just drop this
				// Or maybe have a separate data structure for comments
				// which we link to the next/previous node
			}break;
			
			case TK_QMK    : break;
			case TK_COLON  : break;
			case TK_SEMI   : break;
			case TK_PERIOD : break;
			case TK_COMMA  : break;
			
			default : {
				printf("ERROR : Unexpected symbol @ %i\n", i);
				return -1;
			}break;
		}
	}
	
	printf("ERROR : Ran out of contents\n");
	return -1;
}


int nodeParser(TokenList* tkl, NodeTable* ntab, SymbolTable* stab){
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
					printf("Unmatched parens %i\n", i);
					return 0;
				}else if(stack[stk] != TK_OPN_PAR){
					printf("Missing open parens %i\n", i);
					return 0;
				}
			}break;
			case TK_END_BRK : {
				stk--;
				if(stk < 0){
					printf("Unmatched bracket %i\n", i);
					return 0;
				}else if(stack[stk] != TK_OPN_BRK){
					printf("Missing open bracket %i\n", i);
					return 0;
				}
			}break;
			case TK_END_BRC : {
				stk--;
				if(stk < 0){
					printf("Unmatched brace %i\n", i);
					return 0;
				}else if(stack[stk] != TK_OPN_BRC){
					printf("Missing open brace %i\n", i);
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
	
	
	subParser(tkl, ntab, stab, 0, 0);
	
	
	return 1;
}
