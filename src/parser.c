#include "frontend.h"
#include "type.h"

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"


int wordChar(char c){
	return ((c >= 'a') && (c <= 'z'))
		|| ((c >= 'A') && (c <= 'Z'))
		|| ((c >= '0') && (c <= '9'));
}


int binNChar(char c){
	return (c == '0') && (c == '1');
}

int decNChar(char c){
	return (c >= '0') && (c <= '9');
}

int octNChar(char c){
	return (c >= '0') && (c <= '7');
}

int hexNChar(char c){
	return ((c >= '9') && (c <= '9'))
		|| ((c >= 'a') && (c <= 'f'))
		|| ((c >= 'A') && (c <= 'F'));
}

int numLen(char* text, int pos){
	int i =  0;
	int m = 10;
	// TODO: check for 0, 0b, 0x, or 0X
	
	switch(m){
		case  2 : {
			while(1){
				if(!binNChar(text[pos+i])) return i;
				i++;
			}
		}break;
		
		case  8 : {
			while(1){
				if(!octNChar(text[pos+i])) return i;
				i++;
			}
		}break;
		
		case 10 : {
			while(1){
				if(!decNChar(text[pos+i])) return i;
				i++;
			}
		}break;
		
		case 16 : {
			while(1){
				if(!hexNChar(text[pos+i])) return i;
				i++;
			}
		}break;
		
		default : return 0;
	}
}




uint64_t hashText(char* text, int len){
	uint64_t ret = 85917378991987918 - len;
	for(int i = 0; i < len; i++){
		int rot = text[i] % 64;
		ret    ^= (text[i] * 13745795198) - 998573153;
		ret     = (ret >> rot) | (ret << (64-rot));
	}
	if(ret == 0) return len;
	return ret;
}

int wordLen(char* text, int pos){
	int i = 0;
	while(1){
		if(!wordChar(text[pos+i])) return i;
		i++;
	}
}

int wordEq(char* text, int a, int b){
	if(a == b) return 1;
	int i = 0;
	while(1){
		if(text[a+i] != text[b+i]){
			if(wordChar(text[a+i])) return 0;
			if(wordChar(text[b+i])) return 0;
			return 1;
		}
		i++;
	}
}

SymbolTable makeSymbolTable(int size){
	SymbolTable ret;
	ret.hashes = malloc(sizeof(uint64_t) * size);
	for(int i  = 0; i < size; i++) ret.hashes[i] = 0;
	ret.syms   = malloc(sizeof(Symbol  ) * size);
	ret.size   = size;
	ret.fill   = 0;
	return ret;
}

void insertSymbol(SymbolTable* tab, uint64_t fileId, char* text, int pos, int len){
	uint64_t h   = hashText(&text[pos], len);
	int      ix  = h % tab->size;
	for(int i = 0; i < tab->size; i++){
		if(tab->hashes [ix] == 0){
			tab->hashes[ix] = h;
			tab->syms  [ix] = (Symbol){fileId, pos};
			tab->fill++;
			// if(tab->fill >= tab->size/4) growTable(tab);
			return;
		}else if(tab->hashes[ix] == h){
			printf("!!\n");
			if(wordEq(text, pos, tab->syms[ix].filePos)){ printf("=%08lx\n", h); return; }
		}
		ix = (ix+1 >= tab->size)? 0 : ix+1;
	}
}

void printSymbolTable(SymbolTable* tab, char* text){
	char buf[1024];
	printf("====SYMS %i/%i====\n", tab->fill, tab->size);
	for(int i = 0; i < tab->size; i++){
		if(tab->hashes[i]){
			int len   = wordLen(text, tab->syms[i].filePos);
			for(int j = 0; j < len; j++) buf[j] = text[tab->syms[i].filePos+j];
			buf[len]  = 0;
			printf("%04i %08lx %s\n", i, tab->hashes[i], buf);
		}
	}
}


NodeTable makeNodeTable(int size){
	NodeTable ret;
	size      = (size < 64)? 64 : size;
	ret.nodes = malloc(sizeof(Node) * size);
	ret.size  = size;
	ret.fill  = 1;
	return ret;
}

int	makeNode(NodeTable* tab){
	if(tab->fill + 5 < tab->size){
		Node*  tmp = tab->nodes;
		tab->nodes = malloc(sizeof(Node) * tab->size * 2);
		for(int  i = 0; i < tab->fill; i++) tab->nodes[i] = tmp[i];
		free(tmp);
		tab->size *= 2;
	}
	tab->fill++;
	return tab->fill-1;
}

void printNodeTable(NodeTable* tab){
	printf("====NODES : %i/%i====\n", tab->fill-1, tab->size);
	for(int i = 1; i < tab->fill; i++){
		char* sym = 0;
		Node n = tab->nodes[i];
		switch(n.kind){
			case NK_NIL  : sym = "NIL "; break;
			case NK_NUM  : sym = "NUM "; break;
			case NK_BRC  : sym = "BRC "; break;
			case NK_BRK  : sym = "BRK "; break;
			case NK_PAR  : sym = "PAR "; break;
			case NK_TYP  : sym = "TYP "; break;
			case NK_ID   : sym = "ID  "; break;
			case NK_OP   : sym = "OPR "; break;
			default      : sym = "??? "; break;
		}
		printf("  %03x | %s %016lx -> %08x   (@%04x)\n", i, sym, n.val, n.next, n.tpos);
	}
}





/*
	For now, the language will just be a Lisp. A nicer syntax may come later.
	
	Eventually we want to add:
	* [] and {}
	* infix operators
	* operators (+ - * / % \ < > = =< >= != ? : | & ! :A :E !A !E , . ~ _)
*/
int parse(SymbolTable* tab, TokenList* tkl, NodeTable* ntab, uint64_t fileId){
	return 0;
}
