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
	TK_NIL      =   0x00,

	TK_OPN_PAR	=	0x01,
	TK_OPN_BRK	=	0x02,
	TK_OPN_BRC	=	0x03,
	TK_END_PAR	=	0x04,
	TK_END_BRK	=	0x05,
	TK_END_BRC	=	0x06,
	
	TK_ID		=	0x07,
	TK_TYID		=	0x08,
	TK_NUM		=	0x09,
	TK_COM		=	0x0a,
	
	TK_ADD		=	0x0b,	//	+
	TK_SUB		=	0x0c,	//	-
	TK_MUL		=	0x0d,	//	*
	TK_DIV		=	0x0e,	//	/
	TK_MOD		=	0x0f,	//	%
	TK_EXP		=	0x10,	//	**
	
	TK_AND		=	0x11,	//	&
	TK_OR		=	0x12,	//	|
	TK_NOT		=	0x13,	//	!
	TK_XOR		=	0x14,	//	^
	
	TK_LS		=	0x15,	//	<
	TK_LSE		=	0x16,	//	=<
	TK_GT		=	0x17,	//	>
	TK_GTE		=	0x18,	//	>=
	TK_EQ		=	0x19,	//	=
	TK_NEQ		=	0x1a,	//	!=
	TK_NCP		=	0x1b,	//	<!>
	
	TK_COMMA	=	0x1c,	//	,
	TK_QMARK	=	0x1d	//	?
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
	NK_NIL = 0,
	NK_NUM = 1,
	NK_BRC = 2,
	NK_BRK = 3,
	NK_PAR = 4,
	NK_TYP = 5,
	NK_ID  = 6,
	NK_OP  = 7
}NodeKind;

typedef struct{
	uint64_t val;
	NodeKind kind;
	uint32_t next;
	uint32_t tpos;
}Node;

typedef struct{
	Node*	nodes;
	int		size, fill;
}NodeTable;



SymbolTable makeSymbolTable	(int);
void		printSymbolTable(SymbolTable*, char*);

NodeTable	makeNodeTable	(int);
int			makeNode		(NodeTable*);
void		printNodeTable	(NodeTable*);

int			lexer			(TokenList*, int);
void		printTokens		(Token*, int);

int			parse			(SymbolTable*, TokenList*, NodeTable*, uint64_t);



#endif
