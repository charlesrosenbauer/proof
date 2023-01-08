#include "stdio.h"

#include "backend.h"




/*
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
			case BK_TPRD	: printf("tprd    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_TSUM	: printf("tsum    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_TFUN	: printf("tfun    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_REC		: printf("rec     #%03x #%03x\n"	, b.a, b.b		); break;

			// Statements
			case BK_FORALL	: printf("forall  #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_EXISTS	: printf("exists  #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_AND		: printf("and     #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_OR		: printf("or      #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_NOT		: printf("not     #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_IMPLIES	: printf("implies #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_IFF		: printf("iff     #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_EQUALS	: printf("equals  #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_T		: printf("#T      #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_F		: printf("#F      #%03x #%03x\n"	, b.a, b.b		); break;
	
			// Proof combinators
			case BK_MATCH	: printf("match   #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_SPLIT	: printf("split   #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_CASE	: printf("case    #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_CONTRA	: printf("contra  #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_ASSUME	: printf("assume  #%03x\n"			, b.a			); break;
			case BK_CONJCT	: printf("conjct  #%03x\n"			, b.a			); break;
			case BK_EQUIV	: printf("equiv   #%03x #%03x\n"	, b.a, b.b		); break;
			case BK_INDUCT	: printf("induct  #%03x #%03x\n"	, b.a, b.b		); break;
			
			default : printf("???\n"); break;
		}
	}
}



void printModel(Model m){
	printf("====MD %03x (%03x)====\n", m.id, m.def);
	printf("  FIELDS [%i]:\n", m.fct);
	for(int i = 0; i < m.fct; i++)
		printf("    #%03x : #%03x\n", m.field[i], m.fldef[i]);
	
	printf("  OPS [%i]:\n", m.oct);
	for(int i = 0; i < m.oct; i++)
		printf("    #%03x : #%03x\n", m.op   [i], m.opdef[i]);
	
	printf("  AXIOMS [%i]:\n", m.xct);
	for(int i = 0; i < m.xct; i++)
		printf("    #%03x : #%03x\n", m.axiom[i], m.axdef[i]);
}



void printTheorem(Theorem t){
	printf("====TH %03x (%03x)====\n", t.id, t.def);
	printf("  assume : %03x\n", t.assumption );
	printf("  imply  : %03x\n", t.implication);
	printf("  proof [%i]:\n"  , t.pct);
	for(int i = 0; i < t.pct; i++) printf("    %03x\n", t.proof[i]);
}
*/


void printMathTable(MathTable mtab){
	/*
	printf("<<DEFINITIONS [%i]>>\n\n", mtab.dct);
	for(int i = 0; i < mtab.dct; i++){
		printf("====[D %03x : %i/%i]====\n", i, mtab.ds[i].fill, mtab.ds[i].size);
		printDefinition(mtab.ds[i]);
		printf("\n");
	}
	
	printf("<<THEOREMS [%i]>>\n\n", mtab.tct);
	for(int i = 0; i < mtab.tct; i++){
		printTheorem(mtab.ts[i]);
		printf("\n");
	}
	
	printf("<<MODELS [%i]>>\n\n", mtab.mct);
	for(int i = 0; i < mtab.mct; i++){
		printModel(mtab.ms[i]);
		printf("\n");
	}*/
}
