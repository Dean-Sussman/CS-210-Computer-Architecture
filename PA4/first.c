#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<string.h>
#include<limits.h>


struct cacheLine{
	unsigned long long int tag; //tag bit; starts as 0 or a junk value for each node
	unsigned long long int time;	//used for insertion policy. newest insertion has highest time
	unsigned int valid;	//starts as 0 for each node
	struct cacheLine * next;
};

bool searchLRU(struct cacheLine*, unsigned long long int, bool);
bool searchLRU(struct cacheLine* cache, unsigned long long int tag, bool isPrefetch){
	struct cacheLine* updatePoint = NULL;
	struct cacheLine* updatePointAlt = NULL;
	struct cacheLine* ptr = cache;
	unsigned long long int highestTime = 0;
	struct cacheLine* ptr2 = cache;

	while(ptr2 != NULL){//verfied: this loop absolutely works as intended
		ptr2->time += 1;
		ptr2 = ptr2->next;

	}

	while(ptr != NULL){
		if(ptr->tag == tag && ptr->valid == 1){
			if(!isPrefetch){
				ptr->time = 0;
			}
			return true;
		}
		if(ptr->valid == 0){ //if there is a node w/ a 0 valid bit, we prioritize updating that one
			updatePoint = ptr;
		}
		if(ptr->time > highestTime){ //the node with the lowest time will get replaced if there are no 0 valids
			highestTime = ptr->time;
			updatePointAlt = ptr;
		}
		ptr = ptr->next;
	}

	//search was a failure; while we're here, may as well update the set
	if(updatePoint != NULL){ //we have a place with a 0 valid bit we can update
		updatePoint->valid = 1;
		updatePoint->tag = tag;
		updatePoint->time = 0;
	}else{
		updatePointAlt->tag = tag;
		updatePointAlt->time = 0;
	}

	return false;

}
bool searchFIFO(struct cacheLine*, unsigned long long int);
bool searchFIFO(struct cacheLine* cache, unsigned long long int tag){

	struct cacheLine* updatePoint = NULL;
	struct cacheLine* updatePointAlt = NULL;
	struct cacheLine* ptr = cache;
	unsigned long long int time = 0;
	unsigned int lowestTime = UINT_MAX;
	while(ptr != NULL){
		if(ptr->tag == tag && ptr->valid == 1){
			return true;
		}
		if(ptr->valid == 0){ //if there is a node w/ a 0 valid bit, we prioritize updating that one
			updatePoint = ptr;
		}
		if(ptr->time > time){ //the updated node will have an updated time greater than all other times in the set
			time = ptr->time;
		}
		if(ptr->time < lowestTime){ //the node with the lowest time will get replaced if there are no 0 valids
			lowestTime = ptr->time;
			updatePointAlt = ptr;
		}
		ptr = ptr->next;
	}

	//search was a failure; while we're here, may as well update the set
	if(updatePoint != NULL){ //we have a place with a 0 valid bit we can update
		updatePoint->valid = 1;
		updatePoint->tag = tag;
		updatePoint->time = time + 1;
	}else{//with FIFO, we want to yeet the node with the lowest time
		updatePointAlt->tag = tag;
		updatePointAlt->time = time + 1;
	}

	return false;

}

bool powerOfTwo(int);
bool powerOfTwo(int x){

	if((x & (x - 1)) == 0){
		return true;
	}
	return false;

}

int main(int argc, char** argv){
//user enters cache size, associativity, cache policy, block size, and trace file, in that order


	char* temp = argv[1];
	unsigned int cacheSize = atoi(temp);

	char* associativity = argv[2];
	//must be "direct", "assoc", or "assoc:" followed by a power of two
	int test1 = strcmp("direct", associativity);
	int test2 = strcmp("assoc", associativity);
	int test3 = strncmp("assoc:", associativity, 6);
	if(test1 != 0 && test2 != 0 && test3 != 0){
		printf("error");
		return 0;
	}


	char* fifoOrLRU = argv[3];
	//replacement policy must be fifo or lru
	if(strcmp("fifo", fifoOrLRU) != 0 && strcmp("lru", fifoOrLRU) != 0){
		printf("error");
		return 0;
	}

	temp = argv[4];
	unsigned long long int blockSize = atoi(temp);

	unsigned long long int numWays = 1; //direct has only 1 cache line (node) per set
	if(test3 == 0){	//set-associative has n blocks; C A and B provided, solve for C = SAB .'. S + C/AB
		numWays = argv[2][6] - '0';
	}else if(test2 == 0){ //fully associative has cacheSize/blockSize cache lines; S = 1, solve C = 1AB .'. A = C/B
		numWays = cacheSize / blockSize;
	}

	//all of these variables must be a power of two
	if(!powerOfTwo(cacheSize) || !powerOfTwo(blockSize) || !powerOfTwo(numWays)){
		printf("error");
		return 0;
	}

	FILE *fp;
	fp = fopen(argv[5], "r");
	if(fp == NULL){
		printf("error");
		return 0;
	}
	
	unsigned long long int sets = 0;
	if(test1 == 0){//direct mapped cache; S = C/AB, A = 1
		sets = cacheSize / blockSize;
	}else if(test2 == 0){	//fully associative cache; S = 1
		sets = 1;
	}else{			//set-associative cache; S = C/AB
		sets = numWays * blockSize;
		sets = cacheSize / sets;
	}
	
		
	struct cacheLine** cache; //array of sets, where each set is an array of cache lines	
	cache = (struct cacheLine**)malloc(sets * sizeof(struct cacheLine*));
	
	struct cacheLine* tempx;
	for(int i = 0; i < sets; i++){
		for(int j = 0; j < numWays; j++){
			tempx = (struct cacheLine *)malloc(1 * sizeof(struct cacheLine));
			tempx->tag = 0;
			tempx->valid = 0;
			tempx->time = 0;
			tempx->next = cache[i];
			cache[i] = tempx;
		}
	}
	//cache is now initialized with all the nodes that will ever be inserted into it, with its valid bits all at 0
	
	unsigned long long int offsetBits = log2(blockSize);
	unsigned long long int indexBits = log2(sets);
	unsigned long long int tagShifts = offsetBits + indexBits;	//number of bits address must be shifted to get tag

	unsigned long long int cacheHits, cacheMisses, memoryReads, memoryWrites = 0;

	unsigned long long int garbo, address, tag, index, prefetchedAddress, prefetchedIndex, prefetchedTag;
	char readOrWrite;



int chk = strcmp("fifo", fifoOrLRU);
int chk2 = strcmp("lru", fifoOrLRU);
if(chk == 0){
	while(fscanf(fp, "0x%llx: %c 0x%llx\n", &garbo, &readOrWrite, &address) > 0){

		index = (address>>offsetBits) % sets;
		tag = (address>>tagShifts);
		if(searchFIFO(cache[index], tag)){ 
			cacheHits++;
			if(readOrWrite == 'W'){ 
				memoryWrites++;
			}
		}else{
			cacheMisses++;
			memoryReads++;
			if(readOrWrite == 'W'){ 
				memoryWrites++;
			}
		}
	}
	printf("no-prefetch\nMemory reads: %llu\nMemory writes: %llu\nCache hits: %llu\nCache misses: %llu\n", memoryReads, memoryWrites, cacheHits, cacheMisses);

//resets cache and counters
	cacheHits = 0; 
	cacheMisses = 0;
	memoryReads = 0;
	memoryWrites = 0;
	struct cacheLine* ptr = NULL;
	for(int i = 0; i < sets; i++){
		ptr = cache[i];
		while(ptr != NULL){
			ptr->valid = 0;
			ptr = ptr->next;
		}
	}
	rewind(fp);

	while(fscanf(fp, "0x%llx: %c 0x%llx\n", &garbo, &readOrWrite, &address) > 0){ //prefetching
		index = (address>>offsetBits) % sets;
		tag = (address>>tagShifts);
		prefetchedAddress = address + blockSize;
		prefetchedIndex = (prefetchedAddress>>offsetBits) % sets;
		prefetchedTag = (prefetchedAddress>>tagShifts);

		if(searchFIFO(cache[index], tag)){
			cacheHits++;
			if(readOrWrite == 'W'){
				memoryWrites++;
			}
		}else{
			cacheMisses++;
			memoryReads++;
			if(readOrWrite == 'W'){
				memoryWrites++;
			}

			if(!searchFIFO(cache[prefetchedIndex], prefetchedTag)){
				memoryReads++;
			}
		}
	}
	printf("with-prefetch\nMemory reads: %llu\nMemory writes: %llu\nCache hits: %llu\nCache misses: %llu\n", memoryReads, memoryWrites, cacheHits, cacheMisses);
}

if(chk2 == 0){

//2d array instead of LL because the LL searchLRU is fucking up somehow
	while(fscanf(fp, "0x%llx: %c 0x%llx\n", &garbo, &readOrWrite, &address) > 0){

		index = (address>>offsetBits) % sets;
		tag = (address>>tagShifts);
		if(searchLRU(cache[index], tag, false)){ 
			cacheHits++;
			if(readOrWrite == 'W'){ 
				memoryWrites++;
			}
		}else{
			cacheMisses++;
			memoryReads++;
			if(readOrWrite == 'W'){ 
				memoryWrites++;
			}
		}
	}
	printf("no-prefetch\nMemory reads: %llu\nMemory writes: %llu\nCache hits: %llu\nCache misses: %llu\n", memoryReads, memoryWrites, cacheHits, cacheMisses);

//resets cache and counters
	cacheHits = 0; 
	cacheMisses = 0;
	memoryReads = 0;
	memoryWrites = 0;
	struct cacheLine* ptr = NULL;
	for(int i = 0; i < sets; i++){
		ptr = cache[i];
		while(ptr != NULL){
			ptr->valid = 0;
			ptr = ptr->next;
		}
	}
	rewind(fp);

	while(fscanf(fp, "0x%llx: %c 0x%llx\n", &garbo, &readOrWrite, &address) > 0){ //prefetching
		index = (address>>offsetBits) % sets;
		tag = (address>>tagShifts);
		prefetchedAddress = address + blockSize;
		prefetchedIndex = (prefetchedAddress>>offsetBits) % sets;
		prefetchedTag = (prefetchedAddress>>tagShifts);

		if(searchLRU(cache[index], tag, false)){
			cacheHits++;
			if(readOrWrite == 'W'){
				memoryWrites++;
			}
		}else{
			cacheMisses++;
			memoryReads++;
			if(readOrWrite == 'W'){
				memoryWrites++;
			}

			if(!searchLRU(cache[prefetchedIndex], prefetchedTag, true)){
				memoryReads++;
			}
		}
	}
	printf("with-prefetch\nMemory reads: %llu\nMemory writes: %llu\nCache hits: %llu\nCache misses: %llu\n", memoryReads, memoryWrites, cacheHits, cacheMisses);


}
/*
	struct cacheLine* prev = NULL;
	for(int i = 0; i < sets; i++){
		for(int j = 0; j<numWays; j++){
			prev = cache[i];
			cache[i] = cache[i]->next;		
			free(prev);
		}
	}
*/

	return 0;
}
