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
				nix = newNode(ntab);
				ntab->nodes[nix].kind = (k == TK_ID)? NK_ID : NK_TYP;
				ntab->nodes[nix].next = 0;
			}break;
			case TK_NUM		: {
				// Parse number
			}break;
			case TK_COM		: {
				// Probably just drop this
				// Or maybe have a separate data structure for comments
				// which we link to the next/previous node
				ntab->comms[ntab->cmct] = i;
				ntab->cmixs[ntab->cmct] = ntab->fill+1;
				ntab->cmct++;
			}break;
			
			case TK_QMK    : {
				nix = newNode(ntab);
				ntab->nodes[nix].kind = NK_QMK;
				ntab->nodes[nix].next = 0;
			} break;
			case TK_COLON  : {
				nix = newNode(ntab);
				ntab->nodes[nix].kind = NK_CLN;
				ntab->nodes[nix].next = 0;
			} break;
			case TK_SEMI   : {
				nix = newNode(ntab);
				ntab->nodes[nix].kind = NK_SEM;
				ntab->nodes[nix].next = 0;
			} break;
			case TK_PERIOD : {
				nix = newNode(ntab);
				ntab->nodes[nix].kind = NK_PER;
				ntab->nodes[nix].next = 0;
			} break;
			case TK_COMMA  : {
				nix = newNode(ntab);
				ntab->nodes[nix].kind = NK_COM;
				ntab->nodes[nix].next = 0;
			} break;
			
			default : {
				printf("ERROR : Unexpected symbol @ %i\n", i);
				return -1;
			}break;
		}
		
		
	}
	
	//printf("ERROR : Ran out of contents\n");
	//return -1;
	return 0;
}


int nodeParser(TokenList* tkl, NodeTable* ntab, SymbolTable* stab){
	ntab->nodes = malloc(sizeof(Node) * tkl->tkct);
	ntab->fill  = 0;
	
	ntab->comms = malloc(sizeof(int ) * tkl->tkct);
	ntab->cmixs = malloc(sizeof(int ) * tkl->tkct);
	ntab->cmct  = 0;
	
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
	if(subParser(tkl, ntab, stab, 0, 0) < 0) return 0;
	return 1;
}





void printNodeTable(NodeTable* ntab){
	printf("====NTAB [%i]====\n", ntab->fill);
	for(int i = 0; i < ntab->fill; i++){
		Node n = ntab->nodes[i];
		switch(n.kind){
			case NK_NDF		: printf("%03x | ???  %03x %016lx\n", i, n.next, n.sym); break;
	
			case NK_PNIL	: printf("%03x | ()   %03x\n", i, n.next); break;
			case NK_CNIL	: printf("%03x | []   %03x\n", i, n.next); break;
			case NK_KNIL	: printf("%03x | {}   %03x\n", i, n.next); break;
	
			case NK_PAR		: printf("%03x | (    %03x %03x\n", i, n.sub, n.next); break;
			case NK_BRK		: printf("%03x | [    %03x %03x\n", i, n.sub, n.next); break;
			case NK_BRC		: printf("%03x | {    %03x %03x\n", i, n.sub, n.next); break;
	
			case NK_ID		: printf("%03x | ID   %016lx %03x\n", i, n.sym, n.next); break;
			case NK_TYP		: printf("%03x | TY   %016lx %03x\n", i, n.sym, n.next); break;
			case NK_NUM		: printf("%03x | NUM  %016lx %03x\n", i, n.sym, n.next); break;
			case NK_QMK		: printf("%03x | ?    %03x\n", i, n.next); break;
			case NK_CLN		: printf("%03x | :    %03x\n", i, n.next); break;
			case NK_SEM		: printf("%03x | ;    %03x\n", i, n.next); break;
			case NK_PER		: printf("%03x | .    %03x\n", i, n.next); break;
			case NK_COM		: printf("%03x | ,    %03x\n", i, n.next); break;
	
			case NK_K_THR	: printf("%03x | thr  %03x\n", i, n.next); break;
			case NK_K_DEF	: printf("%03x | def  %03x\n", i, n.next); break;
			case NK_K_AXM	: printf("%03x | axm  %03x\n", i, n.next); break;
			case NK_K_ALL	: printf("%03x | all  %03x\n", i, n.next); break;
			case NK_K_EXS	: printf("%03x | exs  %03x\n", i, n.next); break;
		}
	}
	printf("====COMS [%i]====\n", ntab->cmct);
	for(int i = 0; i < ntab->cmct; i++)
		printf("@%03x %03x\n", ntab->cmixs[i], ntab->comms[i]);
}

