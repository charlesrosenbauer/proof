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
	for(int i = 0; i < tab->size; i++){
		if(tab->hashes[i]){
			int len   = wordLen(text, tab->syms[i].filePos);
			for(int j = 0; j < len; j++) buf[j] = text[tab->syms[i].filePos+j];
			buf[len]  = 0;
			printf("%04i %08lx %s\n", i, tab->hashes[i], buf);
		}
	}
}



/*
	For now, the language will just be a Lisp. A nicer syntax may come later.
	
	Eventually we want to add:
	* [] and {}
	* infix operators
	* operators (+ - * / % \ < > = =< >= != ? : | & ! :A :E !A !E , . ~ _)
*/
int parse(SymbolTable* tab, uint64_t fileId, char* text, int len){
	int  tkct = 0;
	int* tks  = alloca(sizeof(int ) * len);
	int  word = -1;
	int depth = 0;
	for(int i = 0; i < len; i++){
		int wordmode = 0;
		if      ( (text[i] == '(')
		||        (text[i] == '[')
		||        (text[i] == '{')){
			tks[depth] = text[i];
			depth++;
		}else if( (text[i] == ')')
		||        (text[i] == ']')
		||        (text[i] == '}')){
			depth--;
			if(depth < 0) return i;
			if((text[i] == ')') && (tks[depth] != '(')) return i;
			if((text[i] == ']') && (tks[depth] != '[')) return i;
			if((text[i] == '}') && (tks[depth] != '{')) return i;
		}else if  (text[i] == ';'){
			while((text[i] != '\n') && (i < len)) i++;
		}else if(wordChar(text[i])){
			if(word == -1) word = i;
			wordmode = 1;
		}
		
		if((word != -1) && !wordmode){
			insertSymbol(tab, fileId, text, word, i-word);
			printf("WORD [%i %i: %08lx]\n", word, i, hashText(&text[word], i-word));
			word = -1;
		}
	}
	if(depth != 0) return len;
	/*
	depth     = 0;
	for(int i = 0; i < len; i++){
		if      ( text[i] == '('){
			// open
		}else if( text[i] == ')'){
			// end
		}else if((text[i] >= '0') && (text[i] <= '9')){
			// integer or middle of symbol
		}else if((text[i] >= 'a') && (text[i] <= 'z')){
			// symbol
		}else if((text[i] >= 'A') && (text[i] <= 'Z')){
			// symbol
		}else if( text[i] <= ' '){
			// whitespace
		}
	}*/
	return -1;
}
