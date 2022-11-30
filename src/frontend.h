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



typedef struct{
	union{
		uint64_t	num;
		uint64_t	sym;
		uint32_t	sub;
	};
	uint32_t	next;
	TkType		kind;
}Node;

typedef struct{
	Node*	nodes;
	int		fill;
}NodeTable;

int 	nodeParser		(TokenList*, NodeTable*, SymbolTable*);


#endif
