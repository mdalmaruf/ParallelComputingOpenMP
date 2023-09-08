#define _CRT_SECURE_NO_WARNINGS //Consider using scanf_s instead. To disable deprecation, we have used _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h> // omp.h library for openMP
#include <math.h>


using namespace std;
#define MAX_NUM_THREADS 4

//users change the matrix size
int matrix_size = 1; //By default the matrix size is 1

void matrix_initialization(int matrix_size);
void serial_computing(int matrix_size);
void parallel_computing(int matrix_size);
void optimized_computing(int matrix_size);
void transpose(int matrix_size);


//globally defined matrix A and B
double A[6000][6000]; //max array size is defined to 6000 but it can be changed
double B[6000][6000];
double C[6000][6000]; //stores the matrix result in matrix C
double transposeB[6000][6000];



int main()
{
    int stopApp;
    int option;
    //int thread_num;

    //thread_num = omp_get_max_threads ( );
    printf ( "\t\t Summary of your computer\n" );
    //printf ( "  The number of processors available = %d\n", omp_get_num_procs ( ) );
    printf ( "  The number of processors available = %d\n", 4 );
    //printf ( "  The number of threads available    = %d\n", thread_num );
    printf ( "  The number of threads available    = %d\n", 4 );


    do
    {
        printf("\n\n \t\t Welcome to the Matrix Multiplication\n");
        printf("Please select the any options from the following\n \t"
               "\'1\': Select 1 for Serial Program Execution\n \t"
               "\'2\': Select 2 for Parallel Program Execution\n \t"
               "\'3\': Select 3 for Optimized Execution of the Matrix Multiplication \n \t"
               "\'-1\': Enter -1 for Exiting from the program \n ");


        scanf("%d", &option);

        stopApp = option;

        switch (option)
        {
        case 1:

            printf("Please provide the matrix size N for calculation (Enter >2000 for getting order of 10 seconds): ");
            scanf("%d", &matrix_size);
            printf("\t\t ---------Starting Serial matrix multiplication--------\n");

            serial_computing(matrix_size);
            break;

        case 2:

            printf("Please provide the matrix size N for calculation (Enter >2000 for getting order of 10 seconds): ");
            scanf("%d", &matrix_size);
            printf("\t\t ---------Starting Parallel matrix multiplication using OpenMP--------\n");

            parallel_computing(matrix_size);
            break;

        case 3:

            printf("Please provide the matrix size N for calculation (Enter >2000 for getting order of 10 seconds): ");
            scanf("%d", &matrix_size);
            printf("\t\t ---------Starting Optimized Parallel (Updated in the Github)--------\n");

            optimized_computing(matrix_size);
            break;

        case -1:

            return 0; //return to program exit

        default:

            break;

        }
    }
    while (stopApp != -1);


    return 0;

}





/* Base code for matrix-matrix multiplication. */
void matrix_initialization(int matrix_size)
{
    //sets the starting point for producing a series of pseudo-random integers
    //srand(clock()); // Seed the PRNG (using the number of "clock ticks" since the execution started as seed).
    srand(time(NULL));


    //initialize the matrices
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            A[i][j] = rand();
            B[i][j] = rand();
        }
    }
}

//function implementation of serial_computing
void serial_computing(int matrix_size)
{
    int i, j, k;
    clock_t time;

    matrix_initialization(matrix_size); //initialize the matrix with defined size

    omp_set_num_threads(1); //single thread running in a single core for in serial execution


    time = clock();

    

    for (i = 0; i < matrix_size; i++){
		for (j = 0; j < matrix_size; j++){
			C[i][j] = 0;
			for (k = 0; k < matrix_size; k++){
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
			}
		}
	}


    printf("\t\t ------Finished Serial matrix multiplication-------\n");
    time = clock() - time;
    printf("Time taken for serial execution was %f (s)\n", (double)time / CLOCKS_PER_SEC);

}





//function implementation of parallel_computing
void parallel_computing(int matrix_size)

{

    int i, j, k;
    double wtime;
    srand(time(NULL));


    

    for (int th = 1; th <= 4; th++)
    {
     omp_set_num_threads(th);
    
     printf("Print number of threads %d:", omp_get_max_threads());
     wtime = omp_get_wtime ( );

	# pragma omp parallel shared ( A, B ) private (i, j, k)
	{    
	for (int i = 0; i < matrix_size; i++)
	{
		for (int j = 0; j < matrix_size; j++)
		{
		    A[i][j] = rand();
		    B[i][j] = rand();
		}
	}







	/*Loop 3: Compute C = A * B.	*/
	# pragma omp for
	for ( i = 0; i < matrix_size; i++ )
	{
		for ( j = 0; j < matrix_size; j++ )
		{
			C[i][j] =0.0;
			for ( k = 0; k < matrix_size; k++ )
			{
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
			}
		}
	}

	}
        wtime = omp_get_wtime ( ) - wtime;
	printf ( "  Elapsed seconds = %g\n", wtime );
}
    

    printf("\t\t ------Finished Parallel execution for matrix multiplication-------\n");

}

//This function helps to transpose the Matrix B
void transpose(int matrix_size)
{
    int i,j;
    for(i=0; i<matrix_size; i++)
    {
        for(j=0; j<matrix_size; j++)
        {
            transposeB[i][j] = B[j][i];
        }
    }
}

//function implementation of optimized parallel_computing
void optimized_computing(int matrix_size)
{

    int i, j, k;
    double wtime;
    srand(time(NULL));


    

    for (int th = 1; th <= 4; th++)
    {
     omp_set_num_threads(th);
    
     printf("Print number of threads %d:", omp_get_max_threads());
     wtime = omp_get_wtime ( );

	# pragma omp parallel shared ( A, B ) private (i, j, k)
	{    
	for (int i = 0; i < matrix_size; i++)
	{
		for (int j = 0; j < matrix_size; j++)
		{
		    A[i][j] = rand();
		    B[i][j] = rand();
		}
	}
       


	/*Loop 3: Compute C = A * B.	*/
	# pragma omp for
	for ( i = 0; i < matrix_size; i++ )
	{
		for ( j = 0; j < matrix_size; j++ )
		{
			C[i][j] =0.0;
			for ( k = 0; k < matrix_size; k++ )
			{
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
			}
		}
	}

	}
        wtime = omp_get_wtime ( ) - wtime;
	printf ( "  Elapsed seconds = %g\n", wtime );
}
    

    printf("\t\t ------Finished Optimized Parallel execution for matrix multiplication-------\n");





  

}
