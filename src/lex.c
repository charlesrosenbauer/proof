#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "frontend.h"


/*

	TK_NIL,
	
(	TK_OPN_PAR,
[	TK_OPN_BRK,
{	TK_OPN_BRC,
)	TK_END_PAR,
]	TK_END_BRK,
}	TK_END_BRC,

	TK_TYID,
	TK_ID,
	TK_NUM,
	TK_STR,
	TK_COM,
	
?	TK_QMK,
:	TK_COLON,
;	TK_SEMI,
.	TK_PERIOD,
,	TK_COMMA,
	
+	TK_ADD,
-	TK_SUB,
*	TK_MUL,
/	TK_DIV,
%	TK_MOD,
**	TK_EXP,
&	TK_AND,
|	TK_OR,
^	TK_XOR,
!	TK_NOT,
<	TK_LS,
=<	TK_LSE,
>	TK_GT,
>=	TK_GTE,
=	TK_EQ,
!=	TK_NEQ,
<>	TK_NCP

*/

int lexer(TokenList* tkl){
	tkl->tks	= malloc(sizeof(Token   ) * tkl->filesize);
	tkl->poss	= malloc(sizeof(Position) * tkl->filesize);
	tkl->tkct	= 0;
	
	int l=1, c=1, tkix=0;
	for(int i = 0; i < tkl->filesize; i++){
		char x0 = tkl->text[i];
		char x1 = (i+1 < tkl->filesize)? tkl->text[i+1] : 0;
		
		switch(x0){
			case '(': {tkl->tks[tkix] = (Token){TK_OPN_PAR, i}; tkix++;} break;
			case '[': {tkl->tks[tkix] = (Token){TK_OPN_BRK, i}; tkix++;}break;
			case '{': {tkl->tks[tkix] = (Token){TK_OPN_BRC, i}; tkix++;}break;
			
			case ')': {tkl->tks[tkix] = (Token){TK_OPN_PAR, i}; tkix++;}break;
			case ']': {tkl->tks[tkix] = (Token){TK_END_BRK, i}; tkix++;}break;
			case '}': {tkl->tks[tkix] = (Token){TK_END_BRC, i}; tkix++;}break;
			
			case '?': {tkl->tks[tkix] = (Token){TK_QMK    , i}; tkix++;}break;
			case ':': {tkl->tks[tkix] = (Token){TK_COLON  , i}; tkix++;}break;
			case ';':{
				if(x1 == ';'){
					// ;; comment
					for(int j = i; j < tkl->filesize; j++){
						i++;
						if(tkl->text[j] == '\n') j = tkl->filesize++;
					}
					tkl->tks[tkix] = (Token){TK_SEMI, i};
					tkix++;
				}else{
					tkl->tks[tkix] = (Token){TK_SEMI, i};
					tkix++;
				}
			}break;
			case '.': {tkl->tks[tkix] = (Token){TK_PERIOD , i}; tkix++;}break;
			case ',': {tkl->tks[tkix] = (Token){TK_COMMA  , i}; tkix++;}break;
			
			case '+': {tkl->tks[tkix] = (Token){TK_ADD    , i}; tkix++;}break;
			case '-': {tkl->tks[tkix] = (Token){TK_SUB    , i}; tkix++;}break;
			case '*':{
				if(x1 == '*'){
					tkl->tks[tkix] = (Token){TK_EXP    , i};
					tkix++;
					i++;
				}else{
					tkl->tks[tkix] = (Token){TK_MUL    , i};
					tkix++;
				}
			}break;
			case '/': {tkl->tks[tkix] = (Token){TK_DIV    , i}; tkix++;}break;
			case '%': {tkl->tks[tkix] = (Token){TK_MOD    , i}; tkix++;}break;
			case '&': {tkl->tks[tkix] = (Token){TK_AND    , i}; tkix++;}break;
			case '|': {tkl->tks[tkix] = (Token){TK_OR     , i}; tkix++;}break;
			case '^': {tkl->tks[tkix] = (Token){TK_XOR    , i}; tkix++;}break;
			case '!':{
				if(x1 == '='){
					tkl->tks[tkix] = (Token){TK_NEQ    , i};
					tkix++;
					i++;
				}else{
					tkl->tks[tkix] = (Token){TK_NOT    , i};
					tkix++;
				}
			}break;
			case '>':{
				if(x1 == '='){
					tkl->tks[tkix] = (Token){TK_GTE    , i};
					tkix++;
					i++;
				}else{
					tkl->tks[tkix] = (Token){TK_GT     , i};
					tkix++;
				}
			}break;
			case '<':{
				if(x1 == '>'){
					tkl->tks[tkix] = (Token){TK_NCP    , i};
					tkix++;
					i++;
				}else{
					tkl->tks[tkix] = (Token){TK_LS     , i};
					tkix++;
				}
			}break;
			case '=':{
				if(x1 == '<'){
					tkl->tks[tkix] = (Token){TK_LSE    , i};
					tkix++;
					i++;
				}else{
					tkl->tks[tkix] = (Token){TK_EQ     , i};
					tkix++;
				}
			}break;
			
			default: {
				// id
				// tyid
				// num
			}break;
		}
	}
	return 1;
}
















