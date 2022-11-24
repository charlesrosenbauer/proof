#include "stdio.h"
#include "stdlib.h"

#include "frontend.h"
#include "util.h"


int main(int ac, char** av){
	char** 		files = alloca(sizeof(char*)   * ac);
	int64_t*	sizes = alloca(sizeof(int64_t) * ac);
	
	int loadpass = 1;
	for(int i = 1; i < ac; i++){
		if(!loadFile(av[i], (int64_t**)&files[i], &sizes[i])){
			printf("Unable to load file %s\n", av[i]);
			loadpass = 0;
		}
	}
	if(!loadpass) return -1;
	
	
	
	
	
	
}
