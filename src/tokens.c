#include "frontend.h"
#include "type.h"

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"



int lexer(TokenList* tkl){
	tkl->tks  = malloc(sizeof(Token) * tkl->textlen);
	tkl->fill = 0;
	tkl->cap  = tkl->textlen;
	
	char* txt = tkl->text;
	
	for(int i = 0; i < tkl->textlen; i++){
		switch(txt[i]){
			case '(' : {tkl->tks[tkl->fill] = (Token){TK_OPN_PAR, i, 0}; } break;
			case '[' : {tkl->tks[tkl->fill] = (Token){TK_OPN_BRK, i, 0}; } break;
			case '{' : {tkl->tks[tkl->fill] = (Token){TK_OPN_BRC, i, 0}; } break;
			case ')' : {tkl->tks[tkl->fill] = (Token){TK_END_PAR, i, 0}; } break;
			case ']' : {tkl->tks[tkl->fill] = (Token){TK_END_BRK, i, 0}; } break;
			case '}' : {tkl->tks[tkl->fill] = (Token){TK_END_BRC, i, 0}; } break;
			
			case '+' : {tkl->tks[tkl->fill] = (Token){TK_ADD    , i, 0}; } break;
			case '-' : {tkl->tks[tkl->fill] = (Token){TK_SUB    , i, 0}; } break;
			case '*' : {tkl->tks[tkl->fill] = (Token){TK_MUL    , i, 0}; } break;
			case '/' : {tkl->tks[tkl->fill] = (Token){TK_DIV    , i, 0}; } break;
			case '%' : {tkl->tks[tkl->fill] = (Token){TK_MOD    , i, 0}; } break;
			
			case '&' : {tkl->tks[tkl->fill] = (Token){TK_AND    , i, 0}; } break;
			case '|' : {tkl->tks[tkl->fill] = (Token){TK_OR     , i, 0}; } break;
			case '!' : {tkl->tks[tkl->fill] = (Token){TK_NOT    , i, 0}; } break;
			case '^' : {tkl->tks[tkl->fill] = (Token){TK_XOR    , i, 0}; } break;
			
		
		}
	}
	
	return 1;
}
