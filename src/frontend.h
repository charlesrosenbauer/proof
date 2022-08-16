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
	TK_NUM		=	0x07,
	TK_LCOM		=	0x08,
	TK_MCOM		=	0x09,
	
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
	TK_NCP		=	0x1a	//	<!>
}TokenType;

typedef struct{
	TokenType	type;
	uint32_t	pos, file;
}Token;



SymbolTable makeSymbolTable	(int);
void		printSymbolTable(SymbolTable*, char*);

int			parse			(SymbolTable*, uint64_t, char*, int);



#endif
