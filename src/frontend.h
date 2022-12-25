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

	TK_TYID,
	TK_ID,
	TK_NUM,
	TK_COM,
	
	TK_QMK,
	TK_COLON,
	TK_SEMI,
	TK_PERIOD,
	TK_COMMA
}TkType;

typedef struct{
	TkType		kind;
	uint32_t	pos;
}Token;

typedef struct{
	uint32_t	line, column;
}Position;

typedef struct{
	Token*		tks;
	Position*	poss;
	int			tkct;
	
	char*		text;
	uint32_t	fileId, filesize;
}TokenList;


int		lexer			(TokenList*);
int		tokenLen		(TokenList*, int);
void	printTokenList	(TokenList*);


typedef struct{
	int64_t		fileId;
	int64_t		filePos;
}Symbol;

typedef struct{
	uint64_t*	hashes;
	Symbol*		syms;
	int			size, fill;
}SymbolTable;

SymbolTable 	makeSymbolTable	(int);
int				insertSymbol	(SymbolTable*, char*, int, int, int, Symbol);
int 			lookupSymbol	(SymbolTable*, char*, int, int, int);


typedef enum{
	NK_NDF		=  0,
	
	NK_PNIL		=  1,
	NK_CNIL		=  2,
	NK_KNIL		=  3,
	
	NK_PAR		=  4,
	NK_BRK		=  5,
	NK_BRC		=  6,
	
	NK_ID		=  7,
	NK_TYP		=  8,
	NK_NUM		=  9,
	NK_QMK		= 10,
	NK_CLN		= 11,
	NK_SEM		= 12,
	NK_PER		= 13,
	NK_COM		= 14,
	
	NK_K_THR	= 15,
	NK_K_DEF	= 16,
	NK_K_AXM	= 17,
	NK_K_ALL	= 18,
	NK_K_EXS	= 19,
	NK_K_FNC	= 20
}NodeKind;

typedef struct{
	NodeKind kind;
	int      tkix;
	union{
		uint32_t	n;
		uint64_t	u;
		int64_t		i;
		TkType		t;
	};
}Node;

typedef struct{
	int head, tail, size;
	int parent, depth;
}Range;

typedef struct{
	Range*			ranges;
	Node*			nodes;
	int				nsize, nfill;
	
	uint32_t*		defs;
	int 			dsize, dfill;
}NodeTable;

int 	nodeParser		(TokenList*, NodeTable*, SymbolTable*);
void	printNodeTable	(NodeTable*);

#endif
