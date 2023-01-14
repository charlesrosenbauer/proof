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
	
	int ln=1, cl=1, tkix=0;
	for(int i = 0; i < tkl->filesize; i++){
		char x0 = tkl->text[i];
		char x1 = (i+1 < tkl->filesize)? tkl->text[i+1] : 0;
		
		switch(x0){
			case '(': {tkl->tks[tkix] = (Token){TK_OPN_PAR, i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;} break;
			case '[': {tkl->tks[tkix] = (Token){TK_OPN_BRK, i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case '{': {tkl->tks[tkix] = (Token){TK_OPN_BRC, i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			
			case ')': {tkl->tks[tkix] = (Token){TK_END_PAR, i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case ']': {tkl->tks[tkix] = (Token){TK_END_BRK, i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case '}': {tkl->tks[tkix] = (Token){TK_END_BRC, i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			
			case '?': {tkl->tks[tkix] = (Token){TK_QMK    , i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case ':': {tkl->tks[tkix] = (Token){TK_COLON  , i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case '=': {tkl->tks[tkix] = (Token){TK_EQ     , i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case '&': {tkl->tks[tkix] = (Token){TK_AND    , i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case '|': {tkl->tks[tkix] = (Token){TK_OR     , i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case '!': {tkl->tks[tkix] = (Token){TK_NOT    , i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case ';':{
				if(x1 == ';'){
					// ;; comment
					tkl->tks [tkix] = (Token   ){TK_COM , i};
					tkl->poss[tkix] = (Position){ln, cl};
					tkix++;
					for(int j = i; j < tkl->filesize; j++){
						if(tkl->text[j] == '\n'){
							i = j;
							j = tkl->filesize;
							ln++;
							cl=1;
						}
					}
				}else{
					tkl->tks [tkix] = (Token   ){TK_SEMI, i};
					tkl->poss[tkix] = (Position){ln, cl};
					tkix++;
				}
			}break;
			case '.': {tkl->tks[tkix] = (Token){TK_PERIOD , i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			case ',': {tkl->tks[tkix] = (Token){TK_COMMA  , i}; tkl->poss[tkix] = (Position){ln, cl}; tkix++;}break;
			default: {
				// ignore whitespace
				if(x0 > ' '){
					if((x0 == '-') && (x1 == '>')){
						tkl->tks [tkix] = (Token){TK_ARROW , i};
						tkl->poss[tkix] = (Position){ln, cl};
						tkix++;
					}else if((x0 == '<') && (x1 == '>')){
						tkl->tks [tkix] = (Token){TK_CONJC , i};
						tkl->poss[tkix] = (Position){ln, cl};
						tkix++;
					}else if((x0 >= 'A') && (x0 <= 'Z')){
						// tyid
						tkl->tks [tkix] = (Token   ){TK_TYID   , i};
						tkl->poss[tkix] = (Position){ln, cl};
						tkix++;
						for(int j = i+1; j < tkl->filesize; j++){
							char n = tkl->text[j];
							if((n != '(') && (n != ')') && (n != '[') && (n != ']')
							&& (n != '{') && (n != '}') && (n != ':') && (n != ';')
							&& (n != '.') && (n != ',') && (n != '?') && (n >  ' ')){
							}else{
								i = j-1;
								j = tkl->filesize;
							}
						}
					}else if((x0 >= '0') && (x0 <= '9')){
						// num
						tkl->tks [tkix] = (Token   ){TK_NUM    , i};
						tkl->poss[tkix] = (Position){ln, cl};
						tkix++;
						for(int j = i+1; j < tkl->filesize; j++){
							i++;
							char n = tkl->text[j];
							if(((n >= '0') && (n <= '9'))
							||  (n == '_')){
							}else{
								i = j-1;
								j = tkl->filesize;
							}
						}
					}else{
						// id
						tkl->tks [tkix] = (Token   ){TK_ID     , i};
						tkl->poss[tkix] = (Position){ln, cl};
						tkix++;
						for(int j = i+1; j < tkl->filesize; j++){
							char n = tkl->text[j];
							if((n != '(') && (n != ')') && (n != '[') && (n != ']')
							&& (n != '{') && (n != '}') && (n != ':') && (n != ';')
							&& (n != '.') && (n != ',') && (n != '?') && (n >  ' ')){
							}else{
								i = j-1;
								j = tkl->filesize;
							}
						}
					}
				}
			}break;
		}
		if(x0 == '\n'){
			ln++;
			cl=1;
		}else if(x0 == '\t'){
			cl  = (cl+4 & -4) + 1;
		}else{
			cl++;
		}
	}
	
	tkl->tkct = tkix+1;
	
	return 1;
}



int tokenLen(TokenList* tkl, int ix){
	if(ix <          0) return 0;
	if(ix >= tkl->tkct) return 0;
	int init = tkl->tks[ix].pos;
	int last = (ix+1 <= tkl->tkct)? tkl->tks[ix+1].pos : tkl->filesize;
	for(int i = last-1; i >= init; i--)
		if(tkl->text[i] > ' ') return (i-init)+1;
	return 0;
}



void printTokenList(TokenList* tkl){
	printf("====TOKS [F%02i] %04iTS====\n", tkl->fileId, tkl->tkct);
	for(int i = 0; i < tkl->tkct; i++){
		printf("%03i %02i @ (%04i, %03i)| ", i, tokenLen(tkl, i), tkl->poss[i].line, tkl->poss[i].column);
	
		switch(tkl->tks[i].kind){
			case TK_NIL     : printf("<NIL>  \n"); break;
	
			case TK_OPN_PAR : printf("OPN PAR\n"); break;
			case TK_OPN_BRK : printf("OPN BRK\n"); break;
			case TK_OPN_BRC : printf("OPN BRC\n"); break;
			case TK_END_PAR : printf("END PAR\n"); break;
			case TK_END_BRK : printf("END BRK\n"); break;
			case TK_END_BRC : printf("END BRC\n"); break;

			case TK_TYID    : printf("TYID   \n"); break;
			case TK_ID      : printf("ID     \n"); break;
			case TK_NUM     : printf("NUM    \n"); break;
			case TK_COM     : printf("COM    \n"); break;
	
			case TK_QMK     : printf("QMK    \n"); break;
			case TK_COLON   : printf("COLON  \n"); break;
			case TK_SEMI    : printf("SEMI   \n"); break;
			case TK_PERIOD  : printf("PERIOD \n"); break;
			case TK_COMMA   : printf("COMMA  \n"); break;
			case TK_ARROW	: printf("ARROW  \n"); break;
		}
	}
}






