#include "frontend.h"
#include "type.h"

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"



int lexer(TokenList* tkl, int fileId){
	tkl->tks  = malloc(sizeof(Token) * tkl->textlen);
	tkl->fill = 0;
	tkl->cap  = tkl->textlen;
	
	char* txt = tkl->text;
	
	for(int i = 0; i < tkl->textlen; i++){
		switch(txt[i]){
			case '(' : {tkl->tks[tkl->fill] = (Token){TK_OPN_PAR, i, fileId}; } break;
			case '[' : {tkl->tks[tkl->fill] = (Token){TK_OPN_BRK, i, fileId}; } break;
			case '{' : {tkl->tks[tkl->fill] = (Token){TK_OPN_BRC, i, fileId}; } break;
			case ')' : {tkl->tks[tkl->fill] = (Token){TK_END_PAR, i, fileId}; } break;
			case ']' : {tkl->tks[tkl->fill] = (Token){TK_END_BRK, i, fileId}; } break;
			case '}' : {tkl->tks[tkl->fill] = (Token){TK_END_BRC, i, fileId}; } break;
			
			case '+' : {tkl->tks[tkl->fill] = (Token){TK_ADD    , i, fileId}; } break;
			case '-' : {tkl->tks[tkl->fill] = (Token){TK_SUB    , i, fileId}; } break;
			case '*' : {
				if((i+1 < tkl->textlen) && (txt[i+1] == '*')){
					tkl->tks[tkl->fill] = (Token){TK_EXP	, i, fileId};
					i++;
				}else{
					tkl->tks[tkl->fill] = (Token){TK_MUL    , i, fileId};
				}
			} break;
			case '/' : {tkl->tks[tkl->fill] = (Token){TK_DIV    , i, fileId}; } break;
			case '%' : {tkl->tks[tkl->fill] = (Token){TK_MOD    , i, fileId}; } break;
			
			case '&' : {tkl->tks[tkl->fill] = (Token){TK_AND    , i, fileId}; } break;
			case '|' : {tkl->tks[tkl->fill] = (Token){TK_OR     , i, fileId}; } break;
			case '^' : {tkl->tks[tkl->fill] = (Token){TK_XOR    , i, fileId}; } break;
			case '!' : {
				// !		!=
				if((i+1 < tkl->textlen) && (txt[i+1] == '=')){
					tkl->tks[tkl->fill] = (Token){TK_NEQ	, i, fileId};
					i++;
				}else{
					tkl->tks[tkl->fill] = (Token){TK_NOT    , i, fileId};
				}
			} break;
			
			case '=' : {
				// =		=<
				if((i+1 < tkl->textlen) && (txt[i+1] == '<')){
					tkl->tks[tkl->fill] = (Token){TK_LSE	, i, fileId};
					i++;
				}else{
					tkl->tks[tkl->fill] = (Token){TK_EQ     , i, fileId};
				}
			} break;
			case '>' : {
				// >		>=
				if((i+1 < tkl->textlen) && (txt[i+1] == '=')){
					tkl->tks[tkl->fill] = (Token){TK_GTE	, i, fileId};
					i++;
				}else{
					tkl->tks[tkl->fill] = (Token){TK_GT     , i, fileId};
				}
			} break;
			case '<' : {
				// <!>		<
				tkl->tks[tkl->fill] = (Token){TK_NOT    , i, fileId};
				
				if((i+2 < tkl->textlen) && (txt[i+1] == '!') && (txt[i+2] == '>')){
					tkl->tks[tkl->fill] = (Token){TK_NCP	, i, fileId};
					i += 2;
				}else{
					tkl->tks[tkl->fill] = (Token){TK_LS     , i, fileId};
				}
			} break;
		
			case ';' : {
				tkl->tks[tkl->fill] = (Token){TK_COM    , i, fileId};
				for(int j = i; j < tkl->textlen; j++){
					if(txt[j] == '\n'){
						i = j+1;
						j = tkl->textlen;
					}
				}
			} break;
			
			case ',' : {tkl->tks[tkl->fill] = (Token){TK_COMMA  , i, fileId}; } break;
			case '?' : {tkl->tks[tkl->fill] = (Token){TK_QMARK  , i, fileId}; } break;
			
			default  : {
				int mode = 0;
				if      ((txt[i] >= 'a') && (txt[i] <= 'z')){
					tkl->tks[tkl->fill] = (Token){TK_ID     , i, fileId};
				}else if((txt[i] >= 'A') && (txt[i] <= 'Z')){
					tkl->tks[tkl->fill] = (Token){TK_TYID   , i, fileId};
				}else if((txt[i] >= '0') && (txt[i] <= '9')){
					tkl->tks[tkl->fill] = (Token){TK_NUM    , i, fileId};
					mode = 1;
				}else{
					tkl->fill--;
					mode = 2;
				}
				
				if(mode == 1){
					int dec = 0;
					for(int j = i; j < tkl->textlen; j++){
						if((txt[j] >= '0') && (txt[j] <= '9')){
							// keep going
						}else if(txt[j] == '.'){
							dec++;
							if(dec > 1){
								i = j;
								j = tkl->textlen;
							}
						}else{
							i = j;
							j = tkl->textlen;
						}
					}
				}else if(!mode){
					for(int j = i; j < tkl->textlen; j++){
						if(((txt[j] >= '0') && (txt[j] <= '9'))
						|| ((txt[j] >= 'a') && (txt[j] <= 'z'))
						|| ((txt[j] >= 'A') && (txt[j] <= 'Z'))
						|| ( txt[j] == '_')){
							// keep going
						}else{
							i = j;
							j = tkl->textlen;
						}
					}
				}else{
					// Bad token
					if(txt[i] > ' ') return 0;
				}
			}break;
			
		}
		tkl->fill++;
	}
	
	return 1;
}



void printTokens(Token* tks, int ct){
	for(int i = 0; i < ct; i++){
		Token t = tks[i];
		printf("  @%05i : %02i |", t.pos, t.file);
		switch(t.type){
			case TK_NIL     : printf(" <NIL>\n"); break;
		
			case TK_OPN_PAR : printf(" (    \n"); break;
			case TK_OPN_BRK : printf(" [    \n"); break;
			case TK_OPN_BRC : printf(" {    \n"); break;
			
			case TK_END_PAR : printf(" )    \n"); break;
			case TK_END_BRK : printf(" ]    \n"); break;
			case TK_END_BRC : printf(" }    \n"); break;
		
			case TK_ID		: printf(" ID   \n"); break;
			case TK_TYID	: printf(" TYID \n"); break;
			case TK_NUM		: printf(" NUM  \n"); break;
			case TK_COM		: printf(" COM  \n"); break;
			
			case TK_ADD		: printf(" ADD  \n"); break;
			case TK_SUB		: printf(" SUB  \n"); break;
			case TK_MUL		: printf(" MUL  \n"); break;
			case TK_DIV		: printf(" DIV  \n"); break;
			case TK_MOD		: printf(" MOD  \n"); break;
			case TK_EXP		: printf(" EXP  \n"); break;
			
			case TK_AND		: printf(" AND  \n"); break;
			case TK_OR		: printf(" OR   \n"); break;
			case TK_NOT		: printf(" NOT  \n"); break;
			case TK_XOR		: printf(" XOR  \n"); break;
			
			case TK_LS		: printf(" LS   \n"); break;
			case TK_LSE		: printf(" LSE  \n"); break;
			case TK_GT		: printf(" GT   \n"); break;
			case TK_GTE		: printf(" GTE  \n"); break;
			case TK_EQ		: printf(" EQ   \n"); break;
			case TK_NEQ		: printf(" NEQ  \n"); break;
			case TK_NCP		: printf(" NCP  \n"); break;
			
			case TK_COMMA	: printf(" COMMA\n"); break;
			case TK_QMARK	: printf(" QMARK\n"); break;
		}
	
	}
}





