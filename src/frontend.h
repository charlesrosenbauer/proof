#ifndef __FRONTEND_HEADER__
#define __FRONTEND_HEADER__


#include "stdint.h"

typedef enum{
	TK_NIL     = 0,
	
	TK_BTM_WRP = 1,
	TK_OPN_PAR = 2,
	TK_OPN_BRK = 3,
	TK_OPN_BRC = 4,
	TK_MID_WRP = 5,
	TK_END_PAR = 6,
	TK_END_BRK = 7,
	TK_END_BRC = 8,
	TK_TOP_WRP = 9,

	TK_TYID    = 10,
	TK_ID      = 11,
	TK_NUM     = 12,
	TK_COM     = 13,
	
	TK_QMK     = 14,
	TK_COLON   = 15,
	TK_SEMI    = 16,
	TK_PERIOD  = 17,
	TK_COMMA   = 18
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
	NK_K_FNC	= 20,
	
	NK_XPAR		= 21,
	NK_XBRK		= 22,
	NK_XBRC		= 23
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
	int head, tail;			// range in token list
	int root, size;			// range in node  list
	int parent, depth;		// position in tree
}Range;

typedef struct{
	Range*			ranges;
	int				rct;
	
	Node*			nodes;
	int				nsize, nfill;
	
	uint32_t*		defs;
	int 			dsize, dfill;
}NodeTable;

typedef struct{
	NodeTable*		ntab;
	TokenList*		tkl;
	SymbolTable*	syms;
}FrontendFile;

int		parseNode		(FrontendFile);
void	printNodeTable	(NodeTable*);

int		parseDef		(FrontendFile, int);

int		matchPattern	(NodeTable*, Range, NodeKind*, int);
int		splitOnSymbol	(NodeTable*, Range, NodeKind , int*);
int 	containsToken	(NodeTable*, Range, NodeKind );
int 	setToken		(NodeTable*, Range, NodeKind*, int);
int 	checkKeyword	(FrontendFile, int, char*, int);



int 	parseDefine		(FrontendFile, int);



#endif
