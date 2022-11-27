#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "frontend.h"
#include "util.h"





SymbolTable makeSymbolTable(int size){
	SymbolTable ret;
	ret.size	= alignExp2(size * 4);
	ret.hashes	= malloc(sizeof(uint64_t) * ret.size);
	ret.syms	= malloc(sizeof(Symbol  ) * ret.size);
	for(int i = 0; i < size; i++) ret.hashes[i] = 0;
	ret.fill	= 0;
	return ret;
}


int insertSymbol(SymbolTable* stab, char* text, int pos, int len, int size, Symbol sym){
	uint64_t hash = hashStrSize(text, len);
	int ix = hash & stab->size-1;
	for(int i = 0; i < stab->size; i++){
		int ix = (hash + i) & (stab->size - 1);
		if(stab->hashes[i] == hash){
			if(pos == stab->syms[i].filePos) return ix;
			int pass = 1;
			if(pos+len < size){
				for(int j = 0; j < len; j++){
					if(text[j+pos] != text[j+stab->syms[i].filePos]){
						pass = 0;
						j = len;
					}
				}
				if(pass) return ix;
			}
		}
		if(!stab->hashes[i]){
			stab->hashes[i] = hash;
			stab->syms  [i] = sym;
			stab->fill++;
			return ix;
		}
	}
	return -1;
}


int lookupSymbol(SymbolTable* stab, char* text, int pos, int len, int size){
	uint64_t hash = hashStrSize(&text[pos], len);
	int ix = hash & stab->size-1;
	for(int i = 0; i < stab->size; i++){
		int ix = (hash + i) & (stab->size - 1);
		if(stab->hashes[i] == hash){
			if(pos == stab->syms[i].filePos) return ix;
			int pass = 1;
			if(pos+len < size){
				for(int j = 0; j < len; j++){
					if(text[j+pos] != text[j+stab->syms[i].filePos]){
						pass = 0;
						j = len;
					}
				}
				if(pass) return ix;
			}
		}else if(!stab->hashes[i]) return -1;
	}
	return -1;
}
