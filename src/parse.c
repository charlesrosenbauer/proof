#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "stdio.h"

#include "frontend.h"


NodeTable	makeNodeTable(int nodes, int defs){
	NodeTable ret;
	ret.nodes		= malloc(sizeof(Node    ) * nodes);
	ret.defs		= malloc(sizeof(uint32_t) * defs );
	for(int i = 0; i < nodes; i++) ret.nodes[i] = (Node){.kind=0, .n=0, .tkix=0};
	ret.nsize		= nodes;
	ret.nfill		= 1;	// 0 is NULL
	ret.dsize		= defs;
	ret.dfill		= 0;
	
	return ret;
}


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
	
	*ntab 			= makeNodeTable(size * 2, defs);
	ntab->ranges 	= rs;
	ntab->rct    	= rct;
	
	for(int i = maxdp; i >= 0; i--){
		for(int j = 0; j < rct; j++){
			if(rs[j].depth == i){
				int pos = ntab->nfill;
				ntab->nfill += rs[j].size;
				
				rs[j].root = pos;
				
				int ix  = pos;
				for(int k = rs[j].head+1; k <= rs[j].tail; k++){
					ntab->nodes[ix] = (Node){.t=tkl->tks[k].kind};
					if((heads[k] >= 0) && (heads[k] != rs[j].head)){
						Node n;
						n.tkix = k;
						switch(tkl->tks[k].kind){
							case TK_OPN_PAR : { n.n = heads[k]; n.kind = NK_PAR; } break;
							case TK_OPN_BRK : { n.n = heads[k]; n.kind = NK_BRK; } break;
							case TK_OPN_BRC : { n.n = heads[k]; n.kind = NK_BRC; } break;
							default : { printf("BAD CASE %i\n", tkl->tks[k].kind); exit(-1); } break;
						}
						k = rs[heads[k]].tail;
						ntab->nodes[ix] = n;
					}else if(heads[k] < 0){
						Node n = ntab->nodes[ix];
						n.tkix = k;
						switch(tkl->tks[k].kind){
							case TK_NUM     : n.kind = NK_NUM ; break;
							case TK_COM     : n.kind = NK_COM ; break;
							case TK_ID      : {
								//n.n    = tokenSymbol(cmp, file, k);
								n.kind = NK_ID;
							}break;
							case TK_TYID    : {
								//n.n    = tokenSymbol(cmp, file, k);
								n.kind = NK_TYP;
							}break;
							case TK_END_PAR : n.kind = NK_XPAR; break;
							case TK_END_BRK : n.kind = NK_XBRK; break;
							case TK_END_BRC : n.kind = NK_XBRC; break;
							
							case TK_QMK		: n.kind = NK_QMK ; break;
							case TK_COLON	: n.kind = NK_CLN ; break;
							case TK_SEMI	: n.kind = NK_SEM ; break;
							case TK_PERIOD	: n.kind = NK_PER ; break;
							
							default			: n.kind = NK_NDF ; break;
						}
						ntab->nodes[ix] = n;
					}
					ix++;
				}
				
				
				if(!i){
					ntab->defs[ntab->dfill] = j;
					ntab->dfill++;
				}
			}
		}
	}
	
	free(heads);
	return 1;
}

/*
int	matchPattern(Compiler* cmp, FileId file, Range r, AtomKind* aks, TkType* tks, int tkct){
	NodeProgram* p = &cmp->ntabs[file];
	if(p == NULL) return 0;
	
	//	ID ID ()
	//	a  b  ()	#T
	for(int i = 0; (i < r.size) && (i < tkct); i++){
		if( (p->nodes[i+r.root].kind != aks[i])
		|| ((p->nodes[i+r.root].t    != tks[i]) && (aks[i] == AK_OP))){
			return 0;
		}
	}
	return 1;
}


int	splitOnToken(Compiler* cmp, FileId file, Range r, AtomKind ak, TkType tk, int* ixs){
	NodeProgram* p = &cmp->ntabs[file];
	if(p == NULL) return 0;
	
	//	[f: a, b, c]
	//	:  [f] [a, b, c]
	//	,  [f] [[a] [b] [c]]
	int splits = 0;
	for(int i  = 0; i < r.size; i++){
		if( (p->nodes[i+r.root].kind == ak)
		&& ((p->nodes[i+r.root].t    == tk) || (ak != AK_OP))){
			ixs[splits] = i;
			splits++;
		}
	}
	return splits;
}


int containsToken(Compiler* cmp, FileId file, Range r, AtomKind ak, TkType tk){
	NodeProgram* p = &cmp->ntabs[file];
	if(p == NULL) return 0;
	
	//	[f: a, b, c]
	//	:  #T
	for(int i = 0; i < r.size; i++)
		if( (p->nodes[i+r.root].kind == ak)
		&& ((p->nodes[i+r.root].t    == tk) || (ak != AK_OP)))
			return 1;
	
	return 0;
}*/
