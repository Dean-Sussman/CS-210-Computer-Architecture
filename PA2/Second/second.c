#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<stdbool.h>

void print(int***);
void print(int*** grid){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 8; j ++){
			printf("%d	", (*grid)[i][j]);
		}
		printf("%d\n", (*grid)[i][8]);
	}
}
int ** grid;
bool areZeroes(int ***);
bool areZeroes(int *** grid){

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if((*grid)[i][j] == 0){
				return true;
			}
		}
	}
	return false;

}

int* populateSubgrid(int*** grid, int rows, int columns){
	int * subgrid = (int*)malloc(9 * sizeof(int));
	int k = 0;
	for(int i = rows - 2; i < rows+1; i++){
		for(int j = columns - 2; j < columns+1; j++){
			subgrid[k] = (*grid)[i][j];
			k++;
		}
	}
	return subgrid;

}
int * dsort(int** arr){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8-i; j++){
			if((*arr)[j] > (*arr)[j+1]){
				int temp = (*arr)[j];
				(*arr)[j] = (*arr)[j+1];
				(*arr)[j+1] = temp;
			}
		}
	}
	return (*arr);

}

bool dDupes(int** arr){

	for(int i = 0; i < 8; i++){
		if((*arr)[i] == (*arr)[i+1] && (*arr)[i] != 0){
			return false;
		}
	}
	return true;

}

bool tChecker(int ***, int, int);
bool tChecker(int *** grid, int row, int col){
	//checks each row and column for duplicate starting entries
	int * arr = (int*)malloc(9 * sizeof(int));
	int * arr2 = (int*)malloc(9*sizeof(int));

	for(int i = 0; i < 9; i++){
		arr[i] = (*grid)[i][col];
		arr2[i] = (*grid)[row][i];
	}

	arr = dsort(&arr);
	arr2 = dsort(&arr2);

	return dDupes(&arr) && dDupes(&arr2);
}

bool subgridChecker(int ***);
bool subgridChecker(int *** grid){

	int * subgrida = populateSubgrid(&(*grid), 2, 2);
	int * subgridb = populateSubgrid(&(*grid), 2, 5);
	int * subgridc = populateSubgrid(&(*grid), 2, 8);
	int * subgridd = populateSubgrid(&(*grid), 5, 2);
	int * subgride = populateSubgrid(&(*grid), 5, 5);
	int * subgridf = populateSubgrid(&(*grid), 5, 8);
	int * subgridg = populateSubgrid(&(*grid), 8, 2);
	int * subgridh = populateSubgrid(&(*grid), 8, 5);
	int * subgridi = populateSubgrid(&(*grid), 8, 8);

	subgrida = dsort(&subgrida);
	subgridb = dsort(&subgridb);
	subgridc = dsort(&subgridc);
	subgridd = dsort(&subgridd);
	subgride = dsort(&subgride);
	subgridf = dsort(&subgridf);
	subgridg = dsort(&subgridg);
	subgridh = dsort(&subgridh);
	subgridi = dsort(&subgridi);

	return (dDupes(&subgrida) && dDupes(&subgridb) && dDupes(&subgridc) && dDupes(&subgridd) && dDupes(&subgride) && dDupes(&subgridf) && dDupes(&subgridg) && dDupes(&subgridh) && dDupes(&subgridi));
}

bool tDuplicates(int***, int, int, int);
bool tDuplicates(int*** grid, int row, int column, int entry){
	//checks for duplicates in a 't' around the given index
	for(int i = 0; i < 9; i++){
		if((*grid)[row][i] == entry){
			return true;
		}
		if((*grid)[i][column] == entry){
			return true;
		}
	}

	return false;
}


bool subgridDuplicates(int***, int, int, int);
bool subgridDuplicates(int *** grid, int subRow, int subCol, int entry){
	int * subgrid = (int*)malloc(9 * sizeof(int));

	if(subRow == 0){//top row; rows 0,1,2
		if(subCol == 0){ //top row, left column; columns 0,1,2
			subgrid = populateSubgrid(&(*grid), 2, 2);
		}else if(subCol == 1){ //top row, middle column; columns 3,4,5
			subgrid = populateSubgrid(&(*grid), 2, 5);
		}else if(subCol == 2){ //top row, right column; columns 6,7,8
			subgrid = populateSubgrid(&(*grid), 2, 8);
		}

	}else if(subRow == 1){//middle row; rows 3,4,5
		if(subCol == 0){ //middle row, left column; columns 0,1,2
			subgrid = populateSubgrid(&(*grid), 5, 2);
		}else if(subCol == 1){ //middle row, middle column; columns 3,4,5
			subgrid = populateSubgrid(&(*grid), 5, 5);
		}else if(subCol == 2){ //middle row, right column; columns 6,7,8
			subgrid = populateSubgrid(&(*grid), 5, 8);
		}

	}else if(subRow == 2){ //bottom row; rows 6,7,8
		if(subCol == 0){ //bottom row, left column; columns 0,1,2
			subgrid = populateSubgrid(&(*grid), 8, 2);
		}else if(subCol == 1){ //bottom row, middle column; columns 3,4,5
			subgrid = populateSubgrid(&(*grid), 8, 5);
		}else if(subCol == 2){ //bottom row, right column; columns 6,7,8
			subgrid = populateSubgrid(&(*grid), 8, 8);
		}
	}

	for(int i = 0; i < 9; i++){
		if(subgrid[i] == entry){
			return true;
		}else{
			continue;
		}
	}
	return false;
}
int main(int argc, char** argv){

	FILE *fp;
	fp = fopen(argv[1], "r");
	if(fp == NULL){
		printf("no-solution");
		return 0;
	}

	//allocates memory for 9x9 sudoku grid
	grid = (int**)malloc(9 * sizeof(int*));
	int ** grid2 = (int**)malloc(9 * sizeof(int*));

	for(int i = 0; i < 9; i++){
		grid[i] = (int*)malloc(9*sizeof(int));
		grid2[i] = (int*)malloc(9*sizeof(int));
	}



	char* str = (char*)malloc(18 * sizeof(char));
	int row = 0;
	int column = 0;


	while(fgets(str, 20, fp) != NULL){
		for(int i = 0; i < 17; i++){
			if(isdigit(str[i])){
				grid[row][column] = str[i] - '0';
				grid2[row][column] = str[i] - '0';
				column++;
			}else if (isspace(str[i])){
				continue;
			}else{
				grid[row][column] = 0;
				grid2[row][column] = 0;
				column++;
			}
		}
		column = 0;
		row++;

	}
	if(!subgridChecker(&grid)){
		printf("no-solution");
		return 0;
	}
	for(int i = 0; i < 9; i++){
		if(!tChecker(&grid, i, i)){
			printf("no-solution");
			return 0;
		}
	}

	int possibleEntries = 0;
	bool changesMade = false;
	int temp = 0;
	while(areZeroes(&grid)){
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				if(grid[i][j]==0){
					possibleEntries = 0;
					for(int k = 1; k <= 9; k++){
						if(!tDuplicates(&grid, i, j, k) && !subgridDuplicates(&grid, i/3, j/3, k)){			
							possibleEntries++;
							if(possibleEntries == 1){
								temp = k;
							}
						}
					}
					if(possibleEntries == 1){
						grid[i][j]=temp;
						changesMade=true;
					}
				}
			}
		}
		if(!changesMade){
			printf("no-solution");
			return 0;
		}
		changesMade = false;

	}
	print(&grid);

	return 0;
}

