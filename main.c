//
// Created by Maryam Yasser on 09/12/2020.
//

//
// Created by Maryam Yasser on 09/12/2020.
//



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

FILE *matrixFile;
FILE *outputFile;
int rows1,rows2,col1,col2;
int**matrix1=NULL,**matrix2=NULL,**matrix3=NULL;


struct v {
    int i; /* row */
    int j; /* column */
    int **matrix3;
};

void *Multiplication(void *param); /* the thread */
void *MultiplicationRow(void *param);



void *MultiplicationRow(void *param) {
    struct v *data = param;
    int n, sum = 0; //the counter and sum

    //GETTING RESULT OF ROW
    for(int j = 0;j< col2;j++){
        for(n = 0; n< col1; n++){

            sum += matrix1[data->i][n] * matrix2[n][j];

        }

        data->matrix3[data->i][j] = sum;
        fprintf(outputFile, "%d ", data->matrix3[data->i][j]);
        printf("%d ",data->matrix3[data->i][j]);
        sum=0;

    }
    printf("\n");

    //Exit the thread
    pthread_exit(0);
}


void *Multiplication(void *param) {
    struct v *data = param;
    int n, sum = 0;

    //getting result of one element
    for(n = 0; n< col1; n++){
        sum += matrix1[data->i][n] * matrix2[n][data->j];
    }


    //putting the result in our matrix3
    data->matrix3[data->i][data->j] = sum;
    fprintf(outputFile, "%d ", data->matrix3[data->i][data->j]);
    printf("%d ",data->matrix3[data->i][data->j]);

    //Exit the thread
    pthread_exit(0);
}


void MultiplyByElement(){
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);


    printf("\nNew Matrix(Element):\n");
    fprintf(outputFile, "New Matrix(Element):\n");



    int i, j, count = 0;
    for (i = 0; i < rows1; i++) {
        for (j = 0; j < col2; j++) {
            //giving i,j to each index
            struct v *data = (struct v *) malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            data->matrix3=malloc(rows1*sizeof(int *));
            for(int i=0;i<rows1;i++)
                data->matrix3[i]=malloc(col2*sizeof(int));


            /* Now create the thread passing it data as a parameter */


            pthread_t tid;       //Thread ID

            pthread_create(&tid, NULL, Multiplication, data);
            //Make sure the parent waits for all thread to complete


            pthread_join(tid, NULL);
            count++;
        }
        printf("\n");
        fprintf(outputFile, "\n");
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed Time: %f\n",elapsed);
    fprintf(outputFile, "Elapsed Time: %f\n",elapsed);

    printf("Number of Threads: %d\n\n",count);
    fprintf(outputFile, "Number of Threads: %d\n\n",count);



}


void MultiplyByRow(){
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int i, j, count = 0;
    printf("New Matrix(Row):\n");
    fprintf(outputFile, "New Matrix(Row):\n");

    //assigning only i to each thread
    for (i = 0; i < rows1; i++) {


        struct v *data = (struct v *) malloc(sizeof(struct v));
        data->i = i;
        data->matrix3=malloc(rows1*sizeof(int *));
        for( int i=0;i<rows1;i++)
            data->matrix3[i]=malloc(col2*sizeof(int));


        //THREAD CREATION
        pthread_t tid;       //Thread ID
        pthread_create(&tid, NULL, MultiplicationRow, data);

        //THREAD JOINING
        pthread_join(tid, NULL);
        count++;
    }


    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed Time: %f\n",elapsed);
    fprintf(outputFile, "Elapsed Time: %f\n",elapsed);
    printf("Number of Threads: %d\n\n",count);
    fprintf(outputFile, "Number of Threads: %d\n\n",count);

}

//READING MATRICES FROM FILE AND INITIALIZING THEM

ReadMatrix(int ***matrix1,int ***matrix2,int *rows1,int *col2,int *col1,int ***matrix3){


    matrixFile  =fopen("/Users/maryamyasser/Documents/College/"
                       "7th Semester/Lab2_OS/matrixFile.txt","r");

    if(matrixFile==NULL)
        exit(1);

    fscanf(matrixFile,"%d %d",&(*rows1),&(*col1));
    printf("%d %d\n",(*rows1),(*col1));

    while (feof(matrixFile) == 0) {

        //MATRIX 1 INITIALIZATION

        *matrix1 = malloc((*rows1) * sizeof(int *));

        for (int i = 0; i < (*rows1); i++)
            (*matrix1)[i] = malloc((*col1) * sizeof(int));

        printf("MATRIX 1:\n");

        for (int j = 0; j < (*rows1); j++) {
            for (int k = 0; k < (*col1); k++) {
                fscanf(matrixFile, "%d", &(*matrix1)[j][k]);
                printf("%d ", (*matrix1)[j][k]);
            }
            printf("\n");
        }

        //MATRIX 2 INITIALIZATION

        fscanf(matrixFile, "%d %d", &rows2, &(*col2));
        printf("%d %d\n", rows2, (*col2));

        *matrix2 = malloc(rows2 * sizeof(int *));
        for (int i = 0; i < rows2; i++)
            (*matrix2)[i] = malloc((*col2) * sizeof(int));
        printf("\nMATRIX 2:\n");
        for (int j = 0; j < rows2; j++) {
            for (int k = 0; k < (*col2); k++) {
                fscanf(matrixFile, "%d", &(*matrix2)[j][k]);
                printf("%d ", (*matrix2)[j][k]);
            }
            printf("\n");
        }


    }
    fclose(matrixFile);

}




int main(int argc, char *argv[]) {

    outputFile  =fopen("/Users/maryamyasser/Documents/"
                       "College/7th Semester/Lab2_OS/outputFile.txt","w");
    ReadMatrix(&matrix1, &matrix2,&rows1,&col2,&col1,&matrix3);

    MultiplyByElement();
    MultiplyByRow();
}




//The thread will begin control in this function




















