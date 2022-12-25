#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "frontend.h"


int newNode(NodeTable* ntab){
	ntab->fill++;
	return ntab->fill-1;
}

typedef struct{
	int32_t	start, end, parent, depth;
}Range;

typedef struct{
	Range*	rs;
	int		size, fill;
}RangeTable;


/*
	TODO:
	* record ranges between () [] {}
	* track parent/child nesting of ranges
	* parse, starting with deepest ranges
	* propagate range sizes up to parents, shrinking parents accordingly
	* build a table of arrays of nodes
	
	* varargs pattern matching functions
	* comment/tag tokens can be extracted and stored elsewhere as decorators
*/









int	parseNode(Compiler* cmp, FileId file){
	TokenData* td = &cmp->tdata[file];
	Range*     rs = malloc(sizeof(Range) * td->tkct);
	int       rct = 0;
	
	int maxdp = 0;
	int depth = -1;
	int head  = -1;
	for(int i = 0; i < td->tkct; i++){
		TkType type = td->tks[i].type;
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
	int* heads = malloc(sizeof(int) * td->tkct);
	for(int i  = 0; i < td->tkct; i++) heads[i] = -1;
	for(int i  = 0; i < rct; i++){
		size  += rs[i].size;
		defs  += rs[i].depth == 0;
		if(heads >= 0) heads[rs[i].head] = i;
	}
	
	cmp->ntabs[file] = makeNodeProgram(size * 2, defs);
	cmp->ntabs[file].ranges = rs;
	
	for(int i = maxdp; i >= 0; i--){
		for(int j = 0; j < rct; j++){
			if(rs[j].depth == i){
				// TODO: build 
				int pos = cmp->ntabs[file].nfill;
				cmp->ntabs[file].nfill += rs[j].size;
				
				int ix = cmp->ntabs[file].nfill;
				for(int k = rs[j].head; k < rs[j].tail; k++){
					assert(ix >= 0);
					assert(k  >= 0);
					cmp->ntabs[file].nodes[ix] = (Node){.t=td->tks[k].type};
					if((heads[k] >= 0) && (heads[k] != rs[j].head)){
						Node n;
						switch(td->tks[k].type){
							case TK_OPN_PAR : { n.n = heads[k]; n.kind = NK_PAR; } break;
							case TK_OPN_BRK : { n.n = heads[k]; n.kind = NK_BRK; } break;
							case TK_OPN_BRC : { n.n = heads[k]; n.kind = NK_BRC; } break;
							default : { printf("BAD CASE %i\n", td->tks[k].type); exit(-1); } break;
						}
						assert(heads[k] >= 0);
						k = rs[heads[k]].tail;
						assert(ix >= 0);
						cmp->ntabs[file].nodes[ix] = n;
					}else{
						assert(ix >= 0);
						Node n = cmp->ntabs[file].nodes[ix];
						switch(td->tks[k].type){
							case TK_INT  : n.kind = NK_INT ; break;
							case TK_FLT  : n.kind = NK_FLT ; break;
							case TK_STR  : n.kind = NK_STR ; break;
							case TK_MID  : n.kind = NK_MID ; break;
							case TK_ID   : n.kind = NK_ID  ; break;
							case TK_TVAR : n.kind = NK_TVAR; break;
							case TK_TYID : n.kind = NK_TYID; break;
							default      : n.kind = NK_OP  ; break;
						}
						cmp->ntabs[file].nodes[ix] = n;
					}
					ix++;
				}
				
				
				if(!i){
					assert(cmp->ntabs[file].dfill >= 0);
					cmp->ntabs[file].defs[cmp->ntabs[file].dfill] = pos;
					cmp->ntabs[file].dfill++;
				}
			}
		}
	}
	
	printf("========FILE %03x========\n", file);
	for(int i = 0; i < rct; i++)
		printf("R%03i : H%04i T%04i > P%04i D%02i\n", i, rs[i].head, rs[i].tail, rs[i].parent, rs[i].depth);
	
	printNodeProgram(&cmp->ntabs[file]);
	
	free(heads);
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
			case NK_K_FNC	: printf("%03x | fnc  %03x\n", i, n.next); break;
		}
	}
	printf("====COMS [%i]====\n", ntab->cmct);
	for(int i = 0; i < ntab->cmct; i++)
		printf("@%03x %03x\n", ntab->cmixs[i], ntab->comms[i]);
}

