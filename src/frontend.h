#ifndef __FRONTEND_HEADER__
#define __FRONTEND_HEADER__


#include "stdint.h"

typedef enum{
	TK_NIL,
	
	TK_OPN_PAR,
	TK_OPN_BRK,
	TK_OPN_BRC,
	TK_END_PAR,
	TK_END_BRK,
	TK_END_BRC,

	TK_ID,
	TK_NUM,
	TK_STR,
	
	TK_QMK,
	TK_COLON,
	TK_SEMI,
	TK_PERIOD,
	TK_COMMA,
	
	TK_ADD,
	TK_SUB,
	TK_MUL,
	TK_DIV,
	TK_MOD,
	TK_EXP,
	TK_AND,
	TK_OR,
	TK_XOR,
	TK_NOT,
	TK_LS,
	TK_LSE,
	TK_GT,
	TK_GTE,
	TK_EQ,
	TK_NEQ,
	TK_NCP
}TokenType;






#endif
