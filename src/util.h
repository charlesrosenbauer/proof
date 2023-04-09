#ifndef __UTIL_HEADER__
#define __UTIL_HEADER__


#include "stdint.h"



typedef struct{
	uint64_t*	table;
	int			size;
}Bloom;

Bloom	makeBloom	(int);
int		checkBloom	(Bloom, uint64_t);
void	insertBloom	(Bloom, uint64_t);
int 	bloomFill	(Bloom);




typedef struct{
	uint64_t*	bits;
	int			size, length;
}BitList;

BitList	makeBitList		(int);
int		checkBitList	(BitList*, int);
void	insertBitList	(BitList*, int);
void	removeBitList	(BitList*, int);



typedef struct{
	int a, b, c;
}Trip;

typedef struct{
	Trip*	xs;
	int		len, cap;
}TripList;

TripList makeTripList  (int);
int      growTripList  (TripList*, int);
int      appendTripList(TripList*, Trip);



typedef struct{
	int a, b;
}Pair;

typedef struct{
	Pair*	xs;
	int		len, cap;
}PairList;

PairList	makePairList	(int);
int			growPairList	(PairList*, int);
int			appendPairList	(PairList*, Pair);
void		sortPairList	(PairList*);
void		printPairList	(PairList);


typedef struct{
	int*	xs;
	int		len, cap;
}IntList;


typedef struct{
	uint64_t	x, y;
}Bloom128;


IntList makeIntList  (int);
int     growIntList  (IntList*, int);
int     appendIntList(IntList*, int);


int      checkBloom128 (Bloom128 , uint64_t);
void     insertBloom128(Bloom128*, uint64_t);
int      bloom128Fill  (Bloom128);

void     leftpad    (int);
int      loadFile   (char*, int64_t**, int64_t*);
int		 dumpFile	(char*, int64_t* , int64_t );
void	 hexdump	(uint8_t*, int);

uint64_t hashString (char*);
uint64_t hashStrSize(char*, int);
uint64_t alignExp2  (uint64_t);

uint64_t fibHashU64 (uint64_t);
uint64_t hashU64    (uint64_t);


uint32_t unalignRU32(uint8_t*, int);
void     unalignWU32(uint8_t*, int, uint32_t);

int		 bitWidth   (int);


uint64_t rng        ();
uint64_t rngseed    (uint64_t, uint64_t);

#endif
