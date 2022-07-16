#include "frontend.h"
#include "type.h"

#include "stdint.h"
#include "stdlib.h"


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
	int depth = 0;
	for(int i = 0; i < len; i++){
		if      ((text[i] == '(')
		||       (text[i] == '[')
		||       (text[i] == '{')){
			tks[depth] = text[i];
			depth++;
		}else if((text[i] == ')')
		||       (text[i] == ']')
		||       (text[i] == '}')){
			depth--;
			if(depth < 0) return i;
			if((text[i] == ')') && (tks[depth] != '(')) return i;
			if((text[i] == ']') && (tks[depth] != '[')) return i;
			if((text[i] == '}') && (tks[depth] != '{')) return i;
		}else if (text[i] == ';'){
			while((text[i] != '\n') && (i < len)) i++;
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
