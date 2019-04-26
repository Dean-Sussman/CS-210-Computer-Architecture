#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

FILE* fp;

enum GATE {NOT, AND, OR, NAND, NOR, XOR, DECODER, MULTIPLEXER};

struct inOutTempNode{
	char name[20];
	int value;
	struct inOutTempNode * next;
};

struct gate{
	enum GATE typeOfGate;
	struct inOutTempNode* inputArr;
	struct inOutTempNode* outputArr;
	struct inOutTempNode* selectorArr;
	unsigned long long int numInputs;
	unsigned long long int numOutputs;
	unsigned long long int numSelectors;	
	struct gate * next;
};

unsigned long long int * convertIntoGrey(unsigned long long int i, unsigned long long int numInputs){
	unsigned long long int * binaryArray;
	binaryArray = (unsigned long long int*)malloc(numInputs * sizeof(unsigned long long int));

	binaryArray[numInputs - 1] = i & 1;
	int k = 1;
	for(int j = numInputs - 2; j >= 0; j--){
		binaryArray[j] = (i>>k)&1;
		k++;
	}

	unsigned long long int * greyArray;
	greyArray = (unsigned long long int*)malloc(numInputs * sizeof(unsigned long long int));
	
	greyArray[0] = binaryArray[0];

	for(int i = 1; i < numInputs; i++){
		greyArray[i] = binaryArray[i] ^ binaryArray[i-1];
	}

	return greyArray;


}

int whichLogicGate(char** str){
	if(strcmp((*str), "NOT") == 0){
		return 0;
	}else if(strcmp((*str), "AND") == 0){
		return 1;
	}else if(strcmp((*str), "OR") == 0){
		return 2;
	}else if(strcmp((*str), "NAND") == 0){
		return 3;
	}else if(strcmp((*str), "NOR") == 0){
		return 4;
	}else if(strcmp((*str), "XOR") == 0){
		return 5;
	}else if(strcmp((*str), "DECODER") == 0){
		return 6;
	}else if(strcmp((*str), "MULTIPLEXER") == 0){
		return 7;
	}
printf("this should never print, check your input\n");
return -1;
}

void printIntputOutput(struct inOutTempNode** array, unsigned long long int numVars ){
	for(unsigned long long int i = 0; i < numVars; i++){
//		printf("%s %d ", (*array)[i].name, (*array)[i].value);
		printf("%d ", (*array)[i].value);


	}
//	printf("\n");
}


unsigned long long int indexSearch(struct inOutTempNode** array, char** targetName, unsigned long long int numVars){
	for(int i = 0; i < numVars; i++){
		if(strcmp((*targetName), (*array)[i].name) == 0){
			return i;
		}
	}
	return -1;
}



void addToCircuitLL(struct gate* node, struct gate** rear, enum GATE typeOfGate, struct inOutTempNode** frontTemp){

	switch(typeOfGate){
		case 0:
			(*node).numInputs = 1;
			(*node).numOutputs = 1;
			(*node).numSelectors = 0;
			break;
		case 6:
			fscanf(fp, " %llu", &(*node).numInputs);
			(*node).numOutputs = pow(2, (*node).numInputs);
			(*node).numSelectors = 0;
			break;
		case 7:
			fscanf(fp, " %llu", &(*node).numInputs);
			(*node).numSelectors = log2((*node).numInputs);
			(*node).numOutputs = 1;
			break;
		default:
			(*node).numInputs = 2;
			(*node).numOutputs = 1;
			(*node).numSelectors = 0;
	}

	(*node).inputArr = (struct inOutTempNode*)malloc((*node).numInputs * sizeof(struct inOutTempNode));
	(*node).outputArr = (struct inOutTempNode*)malloc((*node).numOutputs * sizeof(struct inOutTempNode));
	(*node).selectorArr = (struct inOutTempNode*)malloc((*node).numSelectors * sizeof(struct inOutTempNode));
	bool found = false;

	for(int i = 0; i < (*node).numInputs; i++){
		fscanf(fp, " %s", (*node).inputArr[i].name);
		if(islower((*node).inputArr[i].name[0])){ //circuit accepts a temp variable as input
			struct inOutTempNode* ptr = (*frontTemp);
			while(ptr != NULL){
				if(strcmp(ptr->name, (*node).inputArr[i].name) == 0){
					found = true;
				}
				ptr = ptr->next;
			}
			if(!found){ 
				struct inOutTempNode* tempNode;	
				tempNode = (struct inOutTempNode*)malloc(sizeof(struct inOutTempNode));
				(*tempNode).next = (*frontTemp);
				(*frontTemp) = tempNode;
				strcpy((*tempNode).name, (*node).inputArr[i].name);
				(*tempNode).value = -1;
			}
		}
	}	

	found = false;
	for(int i = 0; i < (*node).numSelectors; i++){
		fscanf(fp, " %s", (*node).selectorArr[i].name);
		if(islower((*node).selectorArr[i].name[0])){ //circuit accepts a temp variable as input
			struct inOutTempNode* ptr = (*frontTemp);
			while(ptr != NULL){
				if(strcmp(ptr->name, (*node).selectorArr[i].name) == 0){
					found = true;
				}
				ptr = ptr->next;
			}
			if(!found){ 
				struct inOutTempNode* tempNode;	
				tempNode = (struct inOutTempNode*)malloc(sizeof(struct inOutTempNode));
				(*tempNode).next = (*frontTemp);
				(*frontTemp) = tempNode;
				strcpy((*tempNode).name, (*node).selectorArr[i].name);
				(*tempNode).value = -1;
			}
		}
	
	}

	found = false;
	for(int i = 0; i < (*node).numOutputs; i++){
		fscanf(fp, " %s", (*node).outputArr[i].name);
		if(islower((*node).outputArr[i].name[0])){ //circuit accepts a temp variable as input
			struct inOutTempNode* ptr = (*frontTemp);
			while(ptr != NULL){
				if(strcmp(ptr->name, (*node).outputArr[i].name) == 0){
					found = true;
				}
				ptr = ptr->next;
			}
			if(!found){ 
				struct inOutTempNode* tempNode;	
				tempNode = (struct inOutTempNode*)malloc(sizeof(struct inOutTempNode));
				(*tempNode).next = (*frontTemp);
				(*frontTemp) = tempNode;
				strcpy((*tempNode).name, (*node).outputArr[i].name);
				(*tempNode).value = -1;
			}
		}
	}
}

void populateInput(struct inOutTempNode** inputArray, unsigned long long int numInputs, unsigned long long int ** greyArray,struct inOutTempNode** outputArray,  unsigned long long int numOutputs, struct inOutTempNode** tempFront){

	for(int i = 0; i < numInputs; i++){
		(*inputArray)[i].value = (*greyArray)[i];
	}

	for(int i = 0; i < numOutputs; i++){
		(*outputArray)[i].value = -1;
	}

	struct inOutTempNode* ptr = (*tempFront);
	while(ptr != NULL){
		ptr->value = -1;
		ptr = ptr->next;
	}
}

void processCircuits(struct gate** frontOfCircuits,struct inOutTempNode** inputArray, unsigned long long int numInputs, struct inOutTempNode** outputArray,  unsigned long long int numOutputs, struct inOutTempNode** tempFront){

	bool skips = true;
	while(skips){
		struct gate* ptr = (*frontOfCircuits);
		skips = false;
		while(ptr != NULL){

			switch(ptr->typeOfGate){

				case 0:
					//not 
					if(!isdigit(ptr->inputArr[0].name[0]) && !islower(ptr->inputArr[0].name[0])){
						//input is an input variable, but not temp
						ptr->outputArr[0].next->value = !ptr->inputArr[0].next->value;
					}else if(isdigit(ptr->inputArr[0].name[0])){
						//input is not a variable or a temp (0 or 1)
						ptr->outputArr[0].next->value = !ptr->inputArr[0].value;
					}else if(islower(ptr->inputArr[0].name[0]) && ptr->inputArr[0].next->value != -1){
						//input is a temp that's been initialized
						ptr->outputArr[0].next->value = !ptr->inputArr[0].next->value;
					}else{
						//input is an unitialized temp
						skips = true;
					}
					break;
				case 1:
					//and

					if(isdigit(ptr->inputArr[0].name[0]) && isdigit(ptr->inputArr[1].name[0])){//both digits
						ptr->outputArr[0].next->value = ptr->inputArr[0].value && ptr->inputArr[1].value;
					}else if(isdigit(ptr->inputArr[0].name[0])){
						if(ptr->inputArr[1].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = ptr->inputArr[0].value && ptr->inputArr[1].next->value;
						}
					}else if(isdigit(ptr->inputArr[1].name[0])){
						if(ptr->inputArr[0].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = ptr->inputArr[1].value && ptr->inputArr[0].next->value;
						}
					}else if(ptr->inputArr[0].next->value == -1 || ptr->inputArr[1].next->value == -1){ //uninitialized temp
						skips = true;
					}else{
						ptr->outputArr[0].next->value = ptr->inputArr[0].next->value && ptr->inputArr[1].next->value;
					}

					break;

				case 2:
					//or

					if(isdigit(ptr->inputArr[0].name[0]) && isdigit(ptr->inputArr[1].name[0])){//both digits
						ptr->outputArr[0].next->value = ptr->inputArr[0].value || ptr->inputArr[1].value;
					}else if(isdigit(ptr->inputArr[0].name[0])){
						if(ptr->inputArr[1].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = ptr->inputArr[0].value || ptr->inputArr[1].next->value;
						}
					}else if(isdigit(ptr->inputArr[1].name[0])){
						if(ptr->inputArr[0].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = ptr->inputArr[1].value || ptr->inputArr[0].next->value;
						}
					}else if(ptr->inputArr[0].next->value == -1 || ptr->inputArr[1].next->value == -1){ //uninitialized temp
						skips = true;
					}else{
						ptr->outputArr[0].next->value = ptr->inputArr[0].next->value || ptr->inputArr[1].next->value;
					}

					break;

				case 3:
					//nand

					if(isdigit(ptr->inputArr[0].name[0]) && isdigit(ptr->inputArr[1].name[0])){//both digits
						ptr->outputArr[0].next->value = !(ptr->inputArr[0].value && ptr->inputArr[1].value);
					}else if(isdigit(ptr->inputArr[0].name[0])){
						if(ptr->inputArr[1].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = !(ptr->inputArr[0].value && ptr->inputArr[1].next->value);
						}
					}else if(isdigit(ptr->inputArr[1].name[0])){
						if(ptr->inputArr[0].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = !(ptr->inputArr[1].value && ptr->inputArr[0].next->value);
						}
					}else if(ptr->inputArr[0].next->value == -1 || ptr->inputArr[1].next->value == -1){ //uninitialized temp
						skips = true;
					}else{
						ptr->outputArr[0].next->value = !(ptr->inputArr[0].next->value && ptr->inputArr[1].next->value);
					}

					break;

				case 4:
					//nor

					if(isdigit(ptr->inputArr[0].name[0]) && isdigit(ptr->inputArr[1].name[0])){//both digits
						ptr->outputArr[0].next->value = !(ptr->inputArr[0].value || ptr->inputArr[1].value);
					}else if(isdigit(ptr->inputArr[0].name[0])){
						if(ptr->inputArr[1].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = !(ptr->inputArr[0].value || ptr->inputArr[1].next->value);
						}
					}else if(isdigit(ptr->inputArr[1].name[0])){
						if(ptr->inputArr[0].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = !(ptr->inputArr[1].value || ptr->inputArr[0].next->value);
						}
					}else if(ptr->inputArr[0].next->value == -1 || ptr->inputArr[1].next->value == -1){ //uninitialized temp
						skips = true;
					}else{
						ptr->outputArr[0].next->value = !(ptr->inputArr[0].next->value || ptr->inputArr[1].next->value);
					}

					break;

				case 5:
					//xor

					if(isdigit(ptr->inputArr[0].name[0]) && isdigit(ptr->inputArr[1].name[0])){//both digits
						ptr->outputArr[0].next->value = ((ptr->inputArr[0].value || ptr->inputArr[1].value) && !(ptr->inputArr[0].value && ptr->inputArr[1].value));
					}else if(isdigit(ptr->inputArr[0].name[0])){
						if(ptr->inputArr[1].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = ((ptr->inputArr[0].value || ptr->inputArr[1].next->value) && !(ptr->inputArr[0].value && ptr->inputArr[1].next->value));
						}
					}else if(isdigit(ptr->inputArr[1].name[0])){
						if(ptr->inputArr[0].next->value == -1){
							skips = true;
						}else{
							ptr->outputArr[0].next->value = ((ptr->inputArr[1].value || ptr->inputArr[0].next->value) && !(ptr->inputArr[1].value && ptr->inputArr[0].next->value));
						}
					}else if(ptr->inputArr[0].next->value == -1 || ptr->inputArr[1].next->value == -1){ //uninitialized temp
						skips = true;
					}else{
						ptr->outputArr[0].next->value = ((ptr->inputArr[0].next->value || ptr->inputArr[1].next->value) && !(ptr->inputArr[0].next->value && ptr->inputArr[1].next->value));
					}

					break;

				case 6: ;
					//decoder
					
					bool flag2 = false;
	
					int * greyArray2;
					greyArray2 = (int *)malloc(ptr->numInputs * sizeof(int));

					for(int i = 0;  i < ptr->numInputs; i++){
						if(isdigit(ptr->inputArr[i].name[0])){ //selector is 0 or 1
							greyArray2[i] = ptr->inputArr[i].value;
						}else if(isupper(ptr->inputArr[i].name[0])){ //selector is an input
							greyArray2[i] = ptr->inputArr[i].next->value;
						}else if(ptr->inputArr[i].next->value == -1){ //uninitialized temp
							skips = true;
							flag2 = true;
							break;
						}else{ //initialized temp
							greyArray2[i] = ptr->inputArr[i].next->value;
						}
					}

					if(flag2){
						break;
					}

					int * binaryArray2;
					binaryArray2 = (int *)malloc(ptr->numInputs * sizeof(int));
					binaryArray2[0] = greyArray2[0];
					for(int i = 1; i < ptr->numInputs; i++){
						binaryArray2[i] = binaryArray2[i-1] ^ greyArray2[i];
					}

					int index2 = 0;
					for(int i = 0; i < ptr->numInputs; i++){
						index2 += binaryArray2[i] * pow(2, ptr->numInputs - 1 - i);
					}
				
					for(int i = 0; i < ptr->numOutputs; i++){

						if(i == index2){
							ptr->outputArr[i].next->value = 1;
						}else{
							ptr->outputArr[i].next->value = 0;
						}
					}
	
					break;
				case 7: ;
					//multiplexer
			
					bool flag = false;
	
					int * greyArray;
					greyArray = (int *)malloc(ptr->numSelectors * sizeof(int));

					for(int i = 0;  i < ptr->numSelectors; i++){
						if(isdigit(ptr->selectorArr[i].name[0])){ //selector is 0 or 1
							greyArray[i] = ptr->selectorArr[i].value;
						}else if(isupper(ptr->selectorArr[i].name[0])){ //selector is an input
							greyArray[i] = ptr->selectorArr[i].next->value;
						}else if(ptr->selectorArr[i].next->value == -1){ //uninitialized temp
							skips = true;
							flag = true;
							break;
						}else{ //initialized temp
							greyArray[i] = ptr->selectorArr[i].next->value;
						}
					}

					if(flag){
						break;
					}

					int * binaryArray;
					binaryArray = (int *)malloc(ptr->numSelectors * sizeof(int));
					binaryArray[0] = greyArray[0];
					for(int i = 1; i < ptr->numSelectors; i++){
						binaryArray[i] = binaryArray[i-1] ^ greyArray[i];
					}

					unsigned long long int index = 0;
					for(int i = 0; i < ptr->numSelectors; i++){
						index += binaryArray[i] * pow(2, ptr->numSelectors - 1 - i);
					}

					if(isdigit(ptr->inputArr[index].name[0])){
						ptr->outputArr[0].next->value = ptr->inputArr[index].value;
					}else if(ptr->inputArr[index].next->value == -1){
						skips = true;
						break;
					}else{
						ptr->outputArr[0].next->value = ptr->inputArr[index].next->value;
					}
				break;
			}
			ptr = ptr->next;	
		}		
	}
}


int main(int argc, char** argv){

	fp = fopen(argv[1], "r");
	if(fp == NULL){
		return 0;
	}

	unsigned long long int numInputVars, numOutputVars = 0;
	fscanf(fp, "%*s %llu", &numInputVars);

	struct inOutTempNode * inputArray;	
	inputArray = (struct inOutTempNode*)malloc(numInputVars * sizeof(struct inOutTempNode));

	//initializes input array	
	for(int i = 0; i < numInputVars; i++){
		fscanf(fp, " %s", inputArray[i].name);
	}
	
	fscanf(fp, "\n%*s %llu", &numOutputVars);

	//initializes output array
	struct inOutTempNode * outputArray;
	outputArray = (struct inOutTempNode*)malloc(numOutputVars * sizeof(struct inOutTempNode));

	for(int i = 0; i < numOutputVars; i++){
		fscanf(fp, " %s", outputArray[i].name);
	}
	fscanf(fp, "\n");

	char* gateName;
	gateName = (char*)malloc(20*sizeof(char));
	struct gate* endOfCircuits;
	endOfCircuits = (struct gate*)malloc(sizeof(struct gate));
	struct gate* circuitt;
	circuitt = (struct gate*)malloc(sizeof(struct gate));
	endOfCircuits = circuitt;
	struct inOutTempNode* tempFrontPtr;
	tempFrontPtr = (struct inOutTempNode*)malloc(sizeof(struct inOutTempNode));
	tempFrontPtr = NULL;

	while(fscanf(fp, "%s\n", gateName) != EOF){
		enum GATE wlg = whichLogicGate(&gateName);
		struct gate* circuit;
		circuit = (struct gate*)malloc(sizeof(struct gate));
		(*circuit).typeOfGate = wlg;
		endOfCircuits->next = circuit;
		endOfCircuits = circuit;
		circuit->next = NULL;
		addToCircuitLL(circuit, &endOfCircuits, wlg, &tempFrontPtr);
	}
//deletes dummy/empty node "circuitt"
	struct gate* one = circuitt;
	struct gate* two = NULL;
	two = one;
	one = one->next;
	two->next = NULL;
	//we now have the input array, output array, temp linked list, and circuit linked lists all read in and initialized

	unsigned long long int * greyArray;
	greyArray = (unsigned long long int *)malloc(numInputVars * sizeof(unsigned long long int));

	struct gate* pt = one;
	while(pt != NULL){
		for(int i = 0; i < pt->numInputs; i++){
			if(islower(pt->inputArr[i].name[0])){
				struct inOutTempNode* tptr = tempFrontPtr;
				while(tptr != NULL){
					if(strcmp(pt->inputArr[i].name, tptr->name) == 0){
						pt->inputArr[i].next = tptr;
						break;
					}
					tptr = tptr->next;
				}
			}else if(isdigit(pt->inputArr[i].name[0])){
				pt->inputArr[i].value = atoi(&pt->inputArr[i].name[0]);
			}else{
				for(int j = 0; j < numInputVars; j++){
					if(strcmp(pt->inputArr[i].name, inputArray[j].name) == 0){
						pt->inputArr[i].next = &inputArray[j];
						break;
					}
				}
			}
		}

		for(int i = 0; i < pt->numOutputs; i++){
			if(islower(pt->outputArr[i].name[0])){
				struct inOutTempNode* tptr = tempFrontPtr;
				while(tptr != NULL){
					if(strcmp(pt->outputArr[i].name, tptr->name) == 0){
						pt->outputArr[i].next = tptr;
						break;
					}
					tptr = tptr->next;
				}
			}else{
				for(int j = 0; j < numOutputVars; j++){
					if(strcmp(pt->outputArr[i].name,outputArray[j].name) == 0){
						pt->outputArr[i].next = &outputArray[j];
						break;
					}
				}
			}
		}
		for(int i = 0; i < pt->numSelectors; i++){
			if(islower(pt->selectorArr[i].name[0])){
				struct inOutTempNode* tptr = tempFrontPtr;
				while(tptr != NULL){
					if(strcmp(pt->selectorArr[i].name, tptr->name) == 0){
						pt->selectorArr[i].next = tptr;
						break;
					}
					tptr = tptr->next;
				}
			}else if(isdigit(pt->selectorArr[i].name[0])){
				pt->selectorArr[i].value = atoi(&pt->selectorArr[i].name[0]);
			}else{
				for(int j = 0; j < numInputVars; j++){
					if(strcmp(pt->selectorArr[i].name, inputArray[j].name) == 0){
						pt->selectorArr[i].next = &inputArray[j];
						break;
					}
				}
			}
		}


		pt = pt->next;
	}


	for(unsigned long long int i = 0; i < pow(2, numInputVars); i++){
		greyArray = convertIntoGrey(i, numInputVars);
/*		for(int j = 0; j < numInputVars; j++){
			printf("%llu ", greyArray[j]);
		}
		printf("\n");
*/		populateInput(&inputArray,numInputVars, &greyArray, &outputArray, numOutputVars, &tempFrontPtr);
		processCircuits(&one,&inputArray,numInputVars, &outputArray, numOutputVars, &tempFrontPtr);
		printIntputOutput(&inputArray, numInputVars);
		printIntputOutput(&outputArray, numOutputVars);
		if(i < pow(2, numInputVars) - 1){
			printf("\n");
		}
	}



/*	//prints temp LL	
	struct inOutTempNode* ptr0 = tempFrontPtr;
	while(ptr0 != NULL){
		printf("%s %d ", ptr0->name, ptr0->value);
		ptr0 = ptr0->next;
	}
	printf("\n");
*/
/*	//prints circuit LL
	
	struct gate* ptr = one;
	while(ptr != NULL){
		printf("this gate is %u ", ptr->typeOfGate);
		for(int i = 0; i < ptr->numInputs; i++){
			if(isdigit(ptr->inputArr[i].name[0])){
				printf("and input %s ", ptr->inputArr[i].name);
			}else{
				printf("and input %s ", ptr->inputArr[i].next->name);
			}
		}
		for(int i = 0; i < ptr->numSelectors; i++){
			if(isdigit(ptr->selectorArr[i].name[0])){
				printf("and selector %s ", ptr->selectorArr[i].name);
			}else{
				printf("and selector %s ", ptr->selectorArr[i].next->name);
			}

		}
		for(int i = 0; i < ptr->numOutputs; i++){
			printf("and output %s ", ptr->outputArr[i].next->name);
		}
		printf("\n");
		ptr = ptr->next;
		

	}
*/

//	printIntputOutput(&inputArray, numInputVars);
//	printIntputOutput(&outputArray, numOutputVars);

	return 0;
}
