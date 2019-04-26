#include<stdio.h>
#include<stdlib.h>

void twoDmalloc(double***, int, int);
void twoDmalloc(double*** matrix, int rows, int columns){
	*matrix = (double **)malloc(rows * sizeof(double *));
	for(int i = 0; i < rows; i++){
		(*matrix)[i] = (double *)malloc(columns * sizeof(double));
	}
}

void print(double***, int, int);
void print(double*** matrix, int rows, int columns){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			printf("%lf ", (*matrix)[i][j]);
		}
		printf("\n");
	}
}

double ** multiply(double ***, double***, int, int, int);
double ** multiply(double *** matrix1, double *** matrix2, int rows1, int columns2, int rows2){
	double entry = 0.000000;
	double ** product = NULL;
	twoDmalloc(&product, rows1, columns2);
	for(int i = 0; i < rows1; i++){
		for(int j = 0; j < columns2; j++){
			for(int k = 0; k < rows2; k++){
				entry += (*matrix1)[i][k] * (*matrix2)[k][j];
			}
		product[i][j] = entry;
		entry = 0.000000;
		}
	}
	return product;
}

double ** transpose(double***, int, int);
double ** transpose(double*** matrix, int rows, int columns){ //takes in a matrix and returns its transpose
	double ** temp = NULL;
	twoDmalloc(&temp, columns, rows);
	
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			temp[j][i] = (*matrix) [i][j];
		}
	}
	return temp;
}

double ** inverse(double***, int, int);
double ** inverse(double *** augM, int rows, int columns){
	double scalar = 1.000000;
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			if(i == j){ //on the diagonal
			
				if((*augM)[i][j] != 1 && (*augM)[i][j] != 0){ //multiply row by appropriate scalar
					//multiply from [i][0] to [i][columns -1] by 1/augM[i][j]
					scalar = 1.000000 / (*augM)[i][j];
			
					for(int k = 0; k < columns; k++){
						(*augM)[i][k] *= scalar;
					}				
				}
				//[i][i] is now 1; now have to get all entries above and below it to 0 through row addition/subtraction
				//iterate from [0][j] to [rows-1][j] (for loop [k][j]) 
				//if [k][j] > 0: subtract that number * the original row for the current row
				//else if [k][j] < 0: add that number times the original row times -1 to the new one

				//checks the rows above and below the 1 entry to set them to 0
				for(int k = 0; k < rows; k++){
					if(k == j){
						continue;
					}else if((*augM)[k][j] > 0){
						scalar = (*augM)[k][j];
					//found a row we need to reduce
					//iterate through row k, front to back, from [k][0] to [k][columns-1]
					//at each position, aug[k][l] -= (scalar * aug[i][l])
					//goes through the whole row of the thing we want to set to 0 and does the appropriate row op
						for(int l = 0; l < columns; l++){	
							(*augM)[k][l] -= (scalar * (*augM)[i][l]);
						}
						

					}else if((*augM)[k][j] < 0){
						scalar = (*augM)[k][j] * -1.000000;
						for(int l = 0; l < columns; l++){						
							(*augM)[k][l] += (scalar * (*augM)[i][l]);
						}
						

					}
				}
			}
		}
	}
//	printf("\naugM: \n");
//	print(augM, rows, columns);

	//matrix is now in rref form; take the back half of it and return it
	double ** inv = NULL;
	twoDmalloc(&inv, rows, rows);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < rows; j++){
			inv[i][j] = (*augM)[i][j+rows]; //could swap rows for columns/2
		}
	}
	return inv;
}

int main(int argc, char** argv){
	FILE *fptrain;
	FILE *fptest;

	fptrain = fopen(argv[1], "r");
	fptest = fopen(argv[2], "r");
	if(fptrain == NULL || fptest == NULL){
		return 0;
	}	
	int testPoints = 0;

	fscanf(fptest, "%d\n", &testPoints);
	double ** testMatrix;
	
	int attributes = 0; //number of training attributes; K
	int examples = 0; //number of training examples; N
	fscanf(fptrain, "%d\n", &attributes);
	fscanf(fptrain, "%d\n", &examples);

	twoDmalloc(&testMatrix, testPoints, (attributes+1));

	//populates test matrix
	for(int i = 0; i < testPoints; i++){
		for(int j = 1; j < attributes; j++){
			fscanf(fptest, "%lf,", &testMatrix[i][j]);		
		}
		fscanf(fptest, "%lf\n", &testMatrix[i][attributes]);
	}
	for(int i = 0; i < testPoints; i++){
		testMatrix[i][0] = 1;
	}


	//allocates space for the attribute training matrix 
	double ** attMatrix = NULL;
	twoDmalloc(&attMatrix, examples, (attributes + 1));
	for(int i = 0; i < examples; i++){
		attMatrix[i][0] = 1;
	}


	//allocates memory for the price training matrix
	double ** priceMatrix = NULL;
	twoDmalloc(&priceMatrix, examples, 1);
	//allocates memory for the weight training matrix
	double ** weightMatrix;
	twoDmalloc(&weightMatrix, (attributes + 1), 1);	

	//all matrices allocated; now must populate attribute and prices matrices


	//scans down each line of the file, populating the attribute matrix
	for(int i = 0; i < examples; i++){

		for(int j = 1; j < attributes+1; j++){
			fscanf(fptrain, "%lf,", &attMatrix[i][j]);
		}
		fscanf(fptrain, "%lf\n", &priceMatrix[i][0]); //last entry in the row is the price

	}
/*
	printf("\nattMatrix: \n");
	print(&attMatrix, examples, attributes+1);
	printf("\npriceMatrix: \n");
	print(&priceMatrix, examples, 1);
	printf("\ntestMatrix: \n");
	print(&testMatrix, testPoints, (attributes + 1));
*/

	double ** attMatrixTpose = NULL;
	twoDmalloc(&attMatrixTpose, (attributes + 1), examples);

	attMatrixTpose = transpose(&attMatrix, examples, (attributes + 1));
//	printf("\nattMatrixTpose: \n");
//	print(&attMatrixTpose, (attributes + 1), examples);

	double ** matrixProduct = NULL;
	twoDmalloc(&matrixProduct, (attributes + 1), (attributes + 1));


	//matrix multiplication; attTpose * att
	

	//returns the new double** product matrix 
	matrixProduct = multiply(&attMatrixTpose, &attMatrix, (attributes + 1), (attributes + 1), examples);

//	printf("\n matrix product: \n");
//	print(&matrixProduct, (attributes + 1), (attributes + 1));

	//invert product matrix
	//declare In identity matrix, append it to product matrix
	double ** identity = NULL;
	twoDmalloc(&identity, (attributes + 1), (attributes + 1));
	for(int i = 0; i < (attributes + 1); i++){
		identity[i][i] = 1;
	}

//	printf("\n identity matrix: \n");
//	print(&identity, (attributes + 1), (attributes + 1));
	//declare new matrix that has twice as many columns, same number of rows
	double ** aug = NULL;
	twoDmalloc(&aug, (attributes + 1), (2 * (attributes + 1)));

	for(int i = 0; i < (attributes + 1); i++){
		for(int j = 0; j < (2*(attributes + 1)); j++){
			if(j < (attributes + 1)){ // first half of the matrix is the original matrix
				aug[i][j] = matrixProduct[i][j]; 
			}else{			 // second half of the matrix is the identity matrix
				aug[i][j] = identity[i][j-(attributes +1)]; 
			}	
		}
	}

//	printf("\naugmented matrix: \n");
//	print(&aug, (attributes + 1), (2*(attributes +1)));

	double ** inverseM = NULL;
	twoDmalloc(&inverseM, (attributes + 1), (attributes + 1));

	inverseM = inverse(&aug, (attributes + 1), (2*(attributes + 1)));

//	printf("\ninverse matrix: \n");
//	print(&inverseM, (attributes + 1), (attributes + 1));

	//have inverse of xT*x, multiply it by xT
	
	double ** mPtwo = NULL;
	twoDmalloc(&mPtwo, (attributes + 1), examples);


	mPtwo = multiply(&inverseM, &attMatrixTpose, (attributes + 1), examples, (attributes + 1));
	//multiply this product by the prices

//	printf("\nmPtwo: \n");
//	print(&mPtwo, (attributes + 1), examples);
	
	weightMatrix = multiply(&mPtwo, &priceMatrix, (attributes + 1), 1, examples);


//	printf("\nweights: \n");
//	print(&weightMatrix, (attributes + 1), 1);

	//have weight matrix; multiply by test matrix to get predicted prices, then print prices 
	
	double ** predictedPrices = NULL;
	twoDmalloc(&predictedPrices, testPoints, 1);

	predictedPrices = multiply(&testMatrix, &weightMatrix, testPoints, 1, (attributes + 1));

	for(int i = 0; i < testPoints; i++){
		printf("%0.0lf\n", predictedPrices[i][0]);
	}

	return 0;
}
