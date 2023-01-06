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


void printNode(NodeTable* p, int i){
	Node n = p->nodes[i];
	printf("%03x @ ", n.tkix);
	switch(n.kind){
		case NK_NDF  : printf("%03x| ???  %03x   (%03x)\n" , i, n.n, n.tkix); break;
		
		case NK_PNIL : printf("%03x| ( )  %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_KNIL : printf("%03x| [ ]  %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_CNIL : printf("%03x| { }  %03x   (%03x)\n" , i, n.n, n.tkix); break;
		
		case NK_PAR  : printf("%03x| PAR  %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_BRK  : printf("%03x| BRK  %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_BRC  : printf("%03x| BRC  %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_NUM  : printf("%03x| NUM  %03lx   (%03x)\n", i, n.u, n.tkix); break;
		case NK_ID   : printf("%03x| ID   %03lx   (%03x)\n", i, n.u, n.tkix); break;
		case NK_TYP  : printf("%03x| TYP  %03lx   (%03x)\n", i, n.u, n.tkix); break;
		case NK_XPAR : printf("%03x| PAR. %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_XBRK : printf("%03x| BRK. %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_XBRC : printf("%03x| BRC. %03x   (%03x)\n" , i, n.n, n.tkix); break;
		
		case NK_QMK  : printf("%03x|  ?   %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_CLN  : printf("%03x|  :   %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_SEM  : printf("%03x|  ;   %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_PER  : printf("%03x|  .   %03x   (%03x)\n" , i, n.n, n.tkix); break;
		case NK_COM  : printf("%03x|  ,   %03x   (%03x)\n" , i, n.n, n.tkix); break;
	}
}


void printNodeTable(NodeTable* p){
	for(int i = 1; i < p->nfill; i++)
		printNode(p, i);
	
	for(int i = 0; i < p->dfill; i++){
		printf("DEF %i : %i\n", i, p->defs[i]);
	}
}


void printRanges(NodeTable* p){
	for(int i = 0; i < p->rct; i++){
		Range r = p->ranges[i];
		printf("R%05x | H%05i T%05i | R%05i S%05i | P%05x D%02x\n", i, r.head, r.tail, r.root, r.size, r.parent, r.depth);
		for(int j = 0; j < r.size; j++) printNode(p, r.root+j);
	}
}


int	matchPattern(NodeTable* p, Range r, NodeKind* nks, int nkct){
	
	/*
		ID ID ()
		a  b  ()	#T
	*/
	for(int i = 0; (i < r.size) && (i < nkct); i++){
		if(nks[i] != NK_NDF){
			if(p->nodes[i+r.root].kind != nks[i]){
				return 0;
			}
		}
	}
	return 1;
}


int	splitOnSymbol(NodeTable* p, Range r, NodeKind nk, int* ixs){
	
	/*
		[f: a, b, c]
		:  [f] [a, b, c]
		,  [f] [[a] [b] [c]]
	*/
	int splits = 0;
	for(int i  = 0; i < r.size; i++){
		if(p->nodes[i+r.root].kind == nk){
			ixs[splits] = i;
			splits++;
		}
	}
	return splits;
}


int containsToken(NodeTable* p, Range r, NodeKind nk){
	
	/*
		[f: a, b, c]
		:  #T
	*/
	for(int i = 0; i < r.size; i++)
		if(p->nodes[i+r.root].kind == nk)
			return 1;
	
	return 0;
}


int setToken(NodeTable* p, Range r, NodeKind* nks, int ct){
	
	uint64_t set = 0;
	for(int i = 0; i < ct; i++) set |= (1l << (nks[i] % 64));
	
	for(int i = 0; i < r.size; i++){
		int k = p->nodes[i+r.root].kind;
		if(set & (1l << (k%64))){
			int pass = 0;
			for(int j = 0; j < ct; j++){
				if(p->nodes[i+r.root].kind == nks[j]){
					pass = 1;
					j = ct;
				}
			}
			if(!pass) return 0;
		}else{
			return 0;
		}
	}
	return 1;
}






int parseDefine(NodeTable* p, int defid){
	// {def TYP DX DX DX AX AX AX }
	
	return 0;
}

int parseTheorem(NodeTable* p, int defid){
	// {thm ID (TMS) -> (TMS) : XP }

	return 0;
}

int parseValue(NodeTable* p, int defid){
	// {val ID XP }

	return 0;
}



int parseDef(NodeTable* p, int defid){
	
	int tryDef = parseDefine (p, defid);
	if(tryDef){
		if(tryDef < 0) return -1;	// bad
		// good
		return tryDef;
	}
	
	
	
	int tryThm = parseTheorem(p, defid);
	if(tryThm){
		if(tryThm < 0) return -1;	// bad
		// good
		return tryThm;
	}
	
	
	
	int tryVal = parseValue(p, defid);
	if(tryVal){
		if(tryVal < 0) return -1;	// bad
		// good
		return tryVal;
	}
	
	

	// Nothing worked
	return -1;
}





