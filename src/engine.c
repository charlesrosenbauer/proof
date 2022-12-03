#include "stdio.h"

#include "backend.h"





void printDefinition(Definition d){
	for(int i = 1; i < d.fill; i++){
		printf("#%03x | ", i);
		Bytecode b = d.code[i];
		switch(b.kind){
			// Terms, constructors
			case BK_NIL		: printf("nil\n"									); break;
			case BK_TERM	: printf("term    #%03x\n"			, b.a			); break;
			case BK_TYPE	: printf("type    #%03x\n"			, b.a			); break;
			case BK_NUM		: printf("num     #%03x\n"			, b.a			); break;
			case BK_PAR		: printf("par     #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_LIST	: printf("list    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_TPRD	: printf("tprd    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_TSUM	: printf("tsum    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_TFUN	: printf("tfun    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_BIND	: printf("bind    #%03x #%03x\n"	, b.a, b.b		); break;

			// Statements
			case BK_FORALL	: printf("forall  #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_EXISTS	: printf("exists  #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_AND		: printf("and     #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_OR		: printf("or      #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_NOT		: printf("not     #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_FUNC	: printf("func    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_APPLY	: printf("apply   #%03x #%03x\n"	, b.a, b.b		); break;
	
			// Proof combinators
			case BK_MATCH	: printf("match   #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_SPLIT	: printf("split   #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_CASE	: printf("case    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_CONTRA	: printf("contra  #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_ASSUME	: printf("assume  #%03x\n"			, b.a			); break;
			case BK_CONJCT	: printf("conjct  #%03x\n"			, b.a			); break;
			case BK_EQUIV	: printf("equiv   #%03x #%03x\n"	, b.a, b.b		); break;
			
			default : printf("???\n"); break;
		}
	}
}
