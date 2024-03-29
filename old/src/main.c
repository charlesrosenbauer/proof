#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "frontend.h"

int loadFile(char* fname, char** buffer, int64_t* fsize){
	FILE*  pFile = fopen (fname,"r");
	size_t result;
	if (pFile == NULL){ printf("Cannot locate file %s.\n", fname); return 0; }
	fseek (pFile , 0 , SEEK_END);
	*fsize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	*buffer = malloc((sizeof(char)*(*fsize)) + 4);
	if (buffer == NULL) { printf("Memory error\n",stderr); exit(2); }

	// copy the file into the buffer:
	result = fread (*buffer,1,(*fsize),pFile);
	if (result != (*fsize)) { printf("Reading error\n",stderr); exit(3); }
	
	fclose(pFile);
	return 1;
}



int main(int ac, char** as){
	if(ac < 2){
		// We should eventually have a REPL or something
		printf("No files to load\n");
		return -1;
	}

	char**   fs = alloca(sizeof(char*) * (ac-1));
	int64_t* ss = alloca(sizeof(int  ) * (ac-1));
	for(int i = 1; i < ac; i++){
		if(!loadFile(as[i], &fs[i-1], &ss[i-1])){
			printf("Could not load file %s\n", as[i]);
			return -1;
		}
	}
	
	SymbolTable stab = makeSymbolTable(2048);
	
	for(int i = 0; i < ac-1; i++){
		printf("%s [%li] :\n%s\n", as[i+1], ss[i], fs[i]);
		
		TokenList tkl = (TokenList){.text=fs[i], .textlen=ss[i]};
		if(!lexer(&tkl, i)){
			printf("ERROR : lexer failed on file %s\n", as[i-1]);
			return -1;
		}
		printTokens(tkl.tks, tkl.fill);
		NodeTable ntab = makeNodeTable(tkl.fill * 2);
		parse(&stab, &tkl, &ntab, i);
		printNodeTable(&ntab);
	}
	
	printSymbolTable(&stab, fs[0]);
}
