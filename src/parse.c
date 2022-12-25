#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "stdio.h"

#include "frontend.h"


int newNode(NodeTable* ntab){
	ntab->nfill++;
	return ntab->nfill-1;
}




int	parseNode(TokenList* tkl, NodeTable* ntab, SymbolTable* syms){
	Range*     rs = malloc(sizeof(Range) * tkl->tkct);
	int       rct = 0;
	
	int maxdp = 0;
	int depth = -1;
	int head  = -1;
	for(int i = 0; i < tkl->tkct; i++){
		TkType type = tkl->tks[i].kind;
		if(type >= TK_BTM_WRP){
			if      (type < TK_MID_WRP){
				// start new subrange
				depth++;
				maxdp = (maxdp < depth)? depth : maxdp;
				assert(rct >= 0);
				rs[rct]   = (Range){
					.head   = i,
					.tail   = 0,
					.size   = 1,
					
					.parent = head,
					.depth  = depth
				};
				if(head >= 0) rs[head].size++;
				head = rct;
				rct++;
				
				
			}else if(type <= TK_TOP_WRP){
				// end subrange, switch head to previous
				if(head >= 0){
					assert(head >= 0);
					rs[head].tail = i;
					head = rs[head].parent;
					depth--;
				}else{
					printf("FAIL\n");
					free(rs);
					return 0;
				}
			}
		}else if(head >= 0){
			assert(head >= 0);
			rs[head].size++;
		}
	}
	
	int defs   = 0;
	int size   = 0;
	int* heads = malloc(sizeof(int) * tkl->tkct);
	for(int i  = 0; i < tkl->tkct; i++) heads[i] = -1;
	for(int i  = 0; i < rct; i++){
		size  += rs[i].size;
		defs  += rs[i].depth == 0;
		if(heads >= 0) heads[rs[i].head] = i;
	}
	
	ntab->nodes  = malloc(sizeof(Node) * tkl->tkct * 2);
	ntab->nsize  = tkl->tkct;
	ntab->nfill  = 0;
	ntab->ranges = rs;
	
	for(int i = maxdp; i >= 0; i--){
		for(int j = 0; j < rct; j++){
			if(rs[j].depth == i){
				int pos = ntab->nfill;
				ntab->nfill += rs[j].size;
				
				int ix = ntab->nfill;
				for(int k = rs[j].head; k < rs[j].tail; k++){
					assert(ix >= 0);
					assert(k  >= 0);
					ntab->nodes[ix] = (Node){.t=tkl->tks[k].kind};
					if((heads[k] >= 0) && (heads[k] != rs[j].head)){
						Node n;
						switch(tkl->tks[k].kind){
							case TK_OPN_PAR : { n.n = heads[k]; n.kind = NK_PAR; } break;
							case TK_OPN_BRK : { n.n = heads[k]; n.kind = NK_BRK; } break;
							case TK_OPN_BRC : { n.n = heads[k]; n.kind = NK_BRC; } break;
							default : { printf("BAD CASE %i\n", tkl->tks[k].kind); exit(-1); } break;
						}
						assert(heads[k] >= 0);
						k = rs[heads[k]].tail;
						assert(ix >= 0);
						ntab->nodes[ix] = n;
					}else{
						assert(ix >= 0);
						Node n = ntab->nodes[ix];
						switch(tkl->tks[k].kind){
							case TK_NUM : n.kind = NK_NUM; break;
							case TK_ID  : n.kind = NK_ID ; break;
							case TK_TYID: n.kind = NK_TYP; break;
							default     : n.kind = NK_COM; break;	// Bad solution for now
						}
						ntab->nodes[ix] = n;
					}
					ix++;
				}
				
				
				if(!i){
					assert(ntab->dfill >= 0);
					ntab->defs[ntab->dfill] = pos;
					ntab->dfill++;
				}
			}
		}
	}
	
	for(int i = 0; i < rct; i++)
		printf("R%03i : H%04i T%04i > P%04i D%02i\n", i, rs[i].head, rs[i].tail, rs[i].parent, rs[i].depth);
	
	free(heads);
	return 1;
}



void printNodeTable(NodeTable* ntab){
	/*
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
			case NK_K_FNC	: printf("%03x | fnc  %03x\n", i, n.next); break;
		}
	}
	printf("====COMS [%i]====\n", ntab->cmct);
	for(int i = 0; i < ntab->cmct; i++)
		printf("@%03x %03x\n", ntab->cmixs[i], ntab->comms[i]);*/
}

