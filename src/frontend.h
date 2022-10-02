#ifndef __FRONTEND_HEADER__
#define __FRONTEND_HEADER__


#include "stdint.h"

#include "type.h"


/*
	TODO:
	* lexer
	* parser
*/
typedef enum{
	TK_OPN_PAR	=	0x00,
	TK_OPN_BRK	=	0x01,
	TK_OPN_BRC	=	0x02,
	TK_END_PAR	=	0x03,
	TK_END_BRK	=	0x04,
	TK_END_BRC	=	0x05,
	
	TK_ID		=	0x06,
	TK_TYID		=	0x07,
	TK_NUM		=	0x08,
	TK_COM		=	0x09,
	
	TK_ADD		=	0x0a,	//	+
	TK_SUB		=	0x0b,	//	-
	TK_MUL		=	0x0c,	//	*
	TK_DIV		=	0x0d,	//	/
	TK_MOD		=	0x0e,	//	%
	TK_EXP		=	0x0f,	//	**
	
	TK_AND		=	0x10,	//	&
	TK_OR		=	0x11,	//	|
	TK_NOT		=	0x12,	//	!
	TK_XOR		=	0x13,	//	^
	
	TK_LS		=	0x14,	//	<
	TK_LSE		=	0x15,	//	=<
	TK_GT		=	0x16,	//	>
	TK_GTE		=	0x17,	//	>=
	TK_EQ		=	0x18,	//	=
	TK_NEQ		=	0x19,	//	!=
	TK_NCP		=	0x1a,	//	<!>
	
	TK_COMMA	=	0x1b,	//	,
	TK_QMARK	=	0x1c	//	?
}TokenType;

typedef struct{
	TokenType	type;
	uint32_t	pos, file;
}Token;

typedef struct{
	char*		text;
	Token*		tks;
	int			textlen, fill, cap;
}TokenList;

typedef enum{
	LK_NIL = 0,
	LK_NUM = 1,
	LK_BRC = 2,
	LK_BRK = 3,
	LK_PAR = 4,
	LK_TYP = 5,
	LK_ID  = 6,
	LK_OP  = 7
}LispKind;

typedef struct{
	uint64_t val;
	LispKind kind;
	uint32_t next;
	uint32_t tpos;
}Lisp;

typedef struct{
	Lisp*	nodes;
	int		size, fill;
}LispTable;



SymbolTable makeSymbolTable	(int);
void		printSymbolTable(SymbolTable*, char*);

int			parse			(SymbolTable*, uint64_t, char*, int);



#endif
