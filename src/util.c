#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "util.h"





Bloom	makeBloom(int size){
	Bloom ret;
	size		= alignExp2(size);
	size		= (size < 64)? 64 : size;
	ret.table	= malloc(sizeof(uint64_t) * size / 64);
	ret.size	= size;
	for(int i   = 0; i < size / 64; i++) ret.table[i] = 0;
	return ret;
}


int		checkBloom(Bloom b, uint64_t n){
	uint64_t w = hashU64( n );
	uint64_t x = hashU64((n >> 17) | (n << (64-17)));
	uint64_t y = hashU64((n >> 23) | (n << (64-23)));
	uint64_t z = hashU64((n >> 51) | (n << (64-51)));
	
	w &= b.size-1;
	x &= b.size-1;
	y &= b.size-1;
	z &= b.size-1;
	
	return (b.table[w / 64] & (1l << (w % 64)))
		&& (b.table[x / 64] & (1l << (x % 64)))
		&& (b.table[y / 64] & (1l << (y % 64)))
		&& (b.table[z / 64] & (1l << (z % 64)));
}

void	insertBloom(Bloom b, uint64_t n){
	uint64_t w = hashU64( n );
	uint64_t x = hashU64((n >> 17) | (n << (64-17)));
	uint64_t y = hashU64((n >> 23) | (n << (64-23)));
	uint64_t z = hashU64((n >> 51) | (n << (64-51)));
	
	w &= b.size-1;
	x &= b.size-1;
	y &= b.size-1;
	z &= b.size-1;
	
	b.table[w / 64] |= (1l << (w % 64));
	b.table[x / 64] |= (1l << (x % 64));
	b.table[y / 64] |= (1l << (y % 64));
	b.table[z / 64] |= (1l << (z % 64));
}

int 	bloomFill(Bloom b){
	int   ret = 0;
	for(int i = 0; i < b.size/64; i++) ret += __builtin_popcountl(b.table[i]);
	return ret;
}




BitList	makeBitList	(int size){
	BitList ret;
	ret.size   =  size;
	ret.length = (size / 64) + ((size % 64) != 0);
	ret.bits   = malloc(sizeof(uint64_t) * ret.length);
	for(int i  = 0; i < ret.length; i++) ret.bits[i] = 0;
	return ret;
}

int	checkBitList(BitList* b, int ix){
	if((ix < 0) || (ix >= b->size)) return 0;
	return (b->bits[ix/64] & (1l << (ix % 64))) != 0;
}

void insertBitList(BitList* b, int ix){
	if((ix < 0) || (ix >= b->size)) return;
	b->bits[ix/64] |=  (1l << (ix % 64));
}

void removeBitList(BitList* b, int ix){
	if((ix < 0) || (ix >= b->size)) return;
	b->bits[ix/64] &= ~(1l << (ix % 64));
}





TripList makeTripList(int size){
	TripList ret;
	ret.xs  = malloc(sizeof(Trip) * size);
	ret.len = 0;
	ret.cap = size;
	return ret;
}

int growTripList(TripList* lst, int size){
	if(lst->cap < size){
		Trip* xs  = lst->xs;
		lst->xs   = malloc(sizeof(Trip) * size);
		for(int i = 0; i < lst->len; i++) lst->xs[i] = xs[i];
		lst->cap  = size;
		free(xs);
	}
	return lst->cap;
}

int appendTripList(TripList* lst, Trip x){
	if(lst->len+5 >= lst->cap) growTripList(lst, lst->cap * 2);
	lst->xs[lst->len] = x;
	lst->len++;
	return lst->len-1;
}



PairList makePairList(int size){
	PairList ret;
	ret.xs  = malloc(sizeof(Pair) * size);
	ret.len = 0;
	ret.cap = size;
	return ret;
}

int growPairList(PairList* lst, int size){
	if(lst->cap < size){
		Pair* xs  = lst->xs;
		lst->xs   = malloc(sizeof(Pair) * size);
		for(int i = 0; i < lst->len; i++) lst->xs[i] = xs[i];
		lst->cap  = size;
		free(xs);
	}
	return lst->cap;
}

int appendPairList(PairList* lst, Pair x){
	if(lst->len+5 >= lst->cap) growPairList(lst, lst->cap * 2);
	lst->xs[lst->len] = x;
	lst->len++;
	return lst->len-1;
}


void swapPair(Pair* a, Pair* b){
	Pair t = *a;
	*a     = *b;
	*b     = t;
}

void sortPairSubList(PairList* lst, int lo, int hi){
	if(lo < hi){
		int pivot = lst->xs[hi].a;
		int index = lo-1;
	
		for(int i = lo; i < hi; i++){
			if(lst->xs[i].a <= pivot){
				index++;
				swapPair(&lst->xs[index], &lst->xs[i]);
			}
		}
		swapPair(&lst->xs[index+1], &lst->xs[hi]);
		
		
		int p = index+1;
		
		sortPairSubList(lst, lo, p-1);
		sortPairSubList(lst, p+1, hi);
	}
}

void sortPairList(PairList* lst){
	sortPairSubList(lst, 0, lst->len-1);
}

void printPairList(PairList lst){
	printf("====%i/%i====\n", lst.len, lst.cap);
	for(int i = 0; i < lst.len; i++)
		printf("%i %i\n", lst.xs[i].a, lst.xs[i].b);
	printf("\n");
}




IntList makeIntList(int size){
	IntList ret;
	ret.xs  = malloc(sizeof(int) * size);
	ret.len = 0;
	ret.cap = size;
	return ret;
}

int growIntList(IntList* lst, int size){
	if(lst->cap < size){
		int* xs = lst->xs;
		lst->xs = malloc(sizeof(int) * size);
		for(int i = 0; i < lst->len; i++) lst->xs[i] = xs[i];
		lst->cap  = size;
		free(xs);
	}
	return lst->cap;
}

int appendIntList(IntList* lst, int x){
	if(lst->len+5 >= lst->cap) growIntList(lst, lst->cap * 2);
	lst->xs[lst->len] = x;
	lst->len++;
	return lst->len-1;
}




void leftpad(int pad){
	for(int i = 0; i < pad; i++) printf(" ");
}


uint64_t hashU64(uint64_t val){
	uint64_t hash = val;
	hash  = ((hash - 4484849178131) * 5159830179831) - 3511938889975819;
	hash ^= (hash >> 37) ^ (hash << 11);
	return hash;
}


int checkBloom128(Bloom128 b, uint64_t hash){
	hash = hashU64(hash);
	uint64_t x = 1l << ((hash >>  5) % 64);
	uint64_t y = 1l << ((hash >> 17) % 64);
	return (b.x & x) | (b.y & y);
}


void insertBloom128(Bloom128* b, uint64_t hash){
	hash = hashU64(hash);
	uint64_t x = 1l << ((hash >>  5) % 64);
	uint64_t y = 1l << ((hash >> 17) % 64);
	b->x  |= x;
	b->y  |= y;
}


int bloom128Fill(Bloom128 b){
	return __builtin_popcountl(b.x) + __builtin_popcount(b.y);
}



int loadFile(char* fname, int64_t** buf, int64_t* fsize){
	FILE*  pFile = fopen (fname,"r");
	size_t result;
	if (pFile == NULL){ printf("Cannot locate file %s.\n", fname); return 0; }
	fseek (pFile , 0 , SEEK_END);
	*fsize = ftell (pFile);
	rewind (pFile);

	char** buffer = (char**)buf;

	// allocate memory to contain the whole file:
	*buffer = malloc((sizeof(char)*(*fsize)) + 4);
	if (buffer == NULL) { printf("Memory error\n",stderr); exit(2); }

	// copy the file into the buffer:
	result = fread (*buffer,1,(*fsize),pFile);
	if (result != (*fsize)) { printf("Reading error\n",stderr); exit(3); }
	
	fclose(pFile);
	return 1;
}



int dumpFile(char* fname, int64_t* buffer, int64_t fsize){
	FILE *write_ptr = fopen(fname,"wb");
	fwrite(buffer, fsize, 1, write_ptr);
	return 1;
}


void hexdump(uint8_t* bs, int len){
	for(int i = 0; i < len; i++){
		printf("%02x ", bs[i]);
		if(!(i % 64)) printf("\n");
	}
	printf("\n");
}




uint64_t hashString(char* text){
	uint8_t* txt = (uint8_t*)text;
	int        i = 0;
	uint64_t ret = 93798170789;
	while(txt[i]){
		int rot = (txt[i] ^ ret) % 64;
		ret     = (ret >> rot) | (ret << (64-rot));
		ret    -= (txt[i] * 356189466161110287) + 517846210013319283 ^ i;
        i++;
	}
	ret = ret? ret : i;
	return ret;
}


uint64_t hashStrSize(char* text, int len){
	uint8_t* txt = (uint8_t*)text;
	uint64_t ret = 93798170789;
	for(int i = 0; i < len; i++){
		int rot = (txt[i] ^ ret) % 64;
		ret     = (ret >> rot) | (ret << (64-rot));
		ret    -= (txt[i] * 356189466161110287) + 517846210013319283 ^ i;
	}
	ret = ret? ret : len;
	return ret;
}



uint64_t alignExp2(uint64_t x){
	if(__builtin_popcountl(x) > 1)
		x = 1l << (64-__builtin_clzl(x));
	return x;
}


/*
	The purpose of this function is to provide a decent and fast hash function
	for U64s that never returns 0 and minimizes conflicts.
*/
uint64_t fibHashU64(uint64_t x){
	x ^= 17538131390810913567llu;	// Shuffle stuff around a bit
	x *= 11400714819323198485llu;	// Fibonacci hashing
	return x? x : 10800578177100189273llu;	// Make sure it never returns 0
}




uint32_t unalignRU32(uint8_t* buffer, int ix){
	uint32_t ret = 0;
	ret  |= buffer[ix+3];
	ret <<= 8;
	ret  |= buffer[ix+2];
	ret <<= 8;
	ret  |= buffer[ix+1];
	ret <<= 8;
	ret  |= buffer[ix  ];
	return ret;
}


void unalignWU32(uint8_t* buffer, int ix, uint32_t x){
	buffer[ix  ] = x;
	buffer[ix+1] = x >>  8;
	buffer[ix+2] = x >> 16;
	buffer[ix+3] = x >> 24;
}



uint64_t RNGSTATEA = 839157918718971;
uint64_t RNGSTATEB = 999380912389113;

uint64_t rng(){
	int rot;
	RNGSTATEA = (RNGSTATEA * 735189719777) - 38591739817981;
	RNGSTATEB = (RNGSTATEB * 993851971897) ^ 83878838831753;
	
	rot       =  RNGSTATEA % 64;
	RNGSTATEA = (RNGSTATEA >> rot) | (RNGSTATEA << (64-rot));
	RNGSTATEB = (RNGSTATEB >>  17) ^ (RNGSTATEB <<  41);
	
	RNGSTATEB = (RNGSTATEA & 0x8f91)? RNGSTATEB : RNGSTATEB - RNGSTATEA;
	return RNGSTATEA ^ RNGSTATEB;
}

uint64_t rngseed(uint64_t a, uint64_t b){
	uint64_t x = rng();
	RNGSTATEB ^= b;
	x         += rng();
	RNGSTATEA ^= a;
	return x + rng();
}



int bitWidth(int x){
	uint64_t n = x;
	return 64-__builtin_clzl(n);
}





