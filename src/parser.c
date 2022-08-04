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

int wordEq(char* text, int a, int b){
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



/*
	For now, the language will just be a Lisp. A nicer syntax may come later.
	
	Eventually we want to add:
	* [] and {}
	* infix operators
	* operators (+ - * / % \ < > = =< >= != ? : | & ! :A :E !A !E , . ~ _)
*/
int parse(char* text, int len){
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
