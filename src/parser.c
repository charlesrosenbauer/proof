#include "frontend.h"
#include "type.h"

#include "stdint.h"
#include "stdlib.h"


/*
	For now, the language will just be a Lisp. A nicer syntax may come later.
*/
int parse(char* text, int len){
	int  tkct = 0;
	int* tks  = alloca(sizeof(int) * len);
	int depth = 0;
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
	}
	return 0;
}
