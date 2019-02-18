// Coded by Pietro Squilla
// Compile with -lm
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

double **createMatrix(int rows, int columns);
double average(double **D, int i, int j, int SMAT);
double sigma(double **D,double med,int i, int j, int SMAT);
void resetMatrix(int rows, int columns, double **m);
void freeMatrix(int rows, double **m);

int main(void) {
    int i, j, DIM=0, SDIM=0, SMAT=0, k=0, l=0;
    double med=0;
    char data_file[50];
    char averages_file[] = "Averages.dat";
    char sigma_file[] = "Sigma.dat";
    
    // file with matrix of data
    printf("Enter data file name: ");
    scanf("%s",data_file);
    
    // matrix size
    printf("Enter dimension N of the square matrix NxN of the data: ");
    scanf("%d",&DIM);
    
    // submatrix size
    printf("Insert dimension M of the square submatrices MxM (extrapolated from NxN matrix): ");
    scanf("%d",&SMAT);
    
    // matrix size control
    if((DIM*DIM) % (SMAT*SMAT) == 0){
        SDIM = (DIM/SMAT);
    }else{
        printf("\nIncorrect dimensions.\n\n");
        exit(1);
    }
    
    double **D = createMatrix(DIM,DIM);
    double **M = createMatrix(SDIM,SDIM);
    double **S = createMatrix(SDIM,SDIM);
    
    resetMatrix(DIM,DIM,D);
    resetMatrix(SDIM,SDIM,M);
    resetMatrix(SDIM,SDIM,S);
    
    // read data
    FILE *read = fopen(data_file,"r");
    if(read == NULL){
        perror("\nError");
        printf("\n");
        exit(1);
    }
    
    for(i=0; i<DIM; i++){
        for(j=0; j<DIM; j++){
            fscanf(read,"%lf    ",&D[i][j]);
        }
    }
    
    fflush(read);
    fclose(read);
    
    // analysis and calculations of data matrices on sub-matrices
    for(i=0; i<=DIM-SMAT; i=i+SMAT){
        for(j=0; j<=DIM-SMAT; j=j+SMAT) {
            M[k][l] = average(D,i,j,SMAT);
            med = M[k][l];
            S[k][l] = sigma(D,med,i,j,SMAT);
            l++;
        }
        l=0;
        k++;
    }
    
    // write averages file
    FILE *writeAverages = fopen(averages_file,"w");
    if(writeAverages == NULL){
        perror("\nError");
        printf("\n");
        exit(1);
    }
    
    for(i=0; i<SDIM; i++){
        for(j=0; j<SDIM; j++){
            fprintf(writeAverages,"%lf    ",M[i][j]);
        }
        fprintf(writeAverages,"\n");
    }
    
    fflush(writeAverages);
    fclose(writeAverages);
    
    // write sigma file
    FILE *writeSigma = fopen(sigma_file,"w");
    if(writeSigma == NULL){
        perror("\nError");
        printf("\n");
        exit(1);
    }
    
    for(i=0; i<SDIM; i++){
        for(j=0; j<SDIM; j++){
            fprintf(writeSigma,"%lf	",S[i][j]);
        }
        fprintf(writeSigma,"\n");
    }
    
    fflush(writeSigma);
    fclose(writeSigma);

    freeMatrix(DIM,D);
    freeMatrix(SDIM,M);
    freeMatrix(SDIM,S);
    
    printf("Done.\n");
    
    return 0;
}

double **createMatrix(int rows, int columns){
    int i, j;
    
    double **m = (double **) malloc(rows * sizeof(double *));
    if(m == NULL){
        perror("\nError");
        printf("\n");
        exit(2);
    }
    
    for(i=0; i<rows; i++){
        m[i] = (double *) malloc(columns * sizeof(double));
        if(m == NULL){
            perror("\nError");
            printf("\n");
            exit(3);
        }
    }
    
    return m;
}

void resetMatrix(int rows, int columns, double **m){
    int i, j;
    for(i=0; i<rows; i++)
    for(j=0; j<columns; j++)
        m[i][j] = 0;
}

void freeMatrix(int rows, double **m) {
    while(--rows > -1){
        free(m[rows]);
    }
    free(m);
}

double average(double **D, int i, int j, int SMAT){
    double average = 0;
    double tmp;
    int k=i, l=j;
    
    for(i; i<k+SMAT; i++){
        for(j; j<l+SMAT; j++){
            tmp = D[i][j];     
            average = average + tmp;
        }
        j=l;
    }
    
    average = average/(SMAT*SMAT);
    
    return average;
}

double sigma(double **D,double med,int i, int j, int SMAT){
	double sigma=0;
	double tmp;
    int k=i, l=j;
    
    for(i; i<k+SMAT; i++){
        for(j; j<l+SMAT; j++){
            tmp = pow((D[i][j] - med),2);
            sigma = sigma + tmp;  
        }
        j=l;
    }
    
    sigma = sqrt((sigma/((SMAT*SMAT)-1)));
	
	return sigma;
}
