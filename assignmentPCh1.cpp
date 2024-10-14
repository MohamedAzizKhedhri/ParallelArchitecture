/**
 * This lab explores and quantifies the impact of data locality on sequential programs performance,
 * using matrix multiplication as an example.
 * The aim is to illustrate how does locality influence performance.
 * It relates to chapters 1 and 3 of "Architectures avancees et modeles de programmation parallele".
 **/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// #define NRA 2000 // number of rows in matrix A
// #define NCA 2000 // number of columns in matrix A
// #define NCB 2000 // number of columns in matrix B
int NRA, NCA, NCB;

//  timing routine
double read_timer();

// Matrix multiply routines
void matrix_multiply(double **a, double **b, double **c);
void matrix_multiplyLoopInterchange(double **a, double **b, double **c);

// Function to initialize matrices and perform matrix multiplication
void initMatrix();
void initMatrixLoopInterchange();

int main(int argc, char *argv[])
{
    printf("Enter an NRA: ");
    scanf("%d", &NRA);
    printf("Enter an NCA: ");
    scanf("%d", &NCA);
    printf("Enter an NCB: ");
    scanf("%d", &NCB);

    long sizeMA = NRA*NCA*sizeof(double);
    long sizeMB = NCA*NCB*sizeof(double);
    long sizeMC = NRA*NCB*sizeof(double);

    printf("Size of matrix A: %ld o\n", sizeMA);
    printf("Size of matrix B: %ld o\n", sizeMB);
    printf("Size of matrix C: %ld o\n", sizeMC);

    long totalSize = sizeMA + sizeMB + sizeMC;

    printf("Total size: %ld o\n", totalSize);

    double starttime, stoptime;
    double nonCashFriendlyTime = 0, cashFriendlyTime = 0;

    // This part is for matrix multiplication without loop interchange
    starttime = read_timer();

    // Call to matrix multiplication
    initMatrix();

    // Stop the timer
    stoptime = read_timer();

    // Print the execution time
    nonCashFriendlyTime = stoptime - starttime;
    printf("Time for sequential matrix multiplication: %3.8f s\n",
           nonCashFriendlyTime);

    // This part is for loop interchange
    starttime = read_timer();

    // Call to matrix multiplication with loop interchange
    initMatrixLoopInterchange();

    // Stop the timer
    stoptime = read_timer();

    // Print the execution time
    cashFriendlyTime = stoptime - starttime;
    printf("Time for sequential matrix multiplication with loop interchange: %3.8f s\n",
           cashFriendlyTime);
    



    double speedUp = nonCashFriendlyTime/cashFriendlyTime;
    printf("spped up : %3.8f \n",
           speedUp);

    FILE *file;
    file = fopen("chadlia.txt", "a");
    fprintf(file, "NRA : %d\t", NRA);
    fprintf(file, "NCA : %d\t", NCA);
    fprintf(file, "NCB : %d\n", NCB);
    fprintf(file, "size A:  %ld o\n", sizeMA);
    fprintf(file, "size B:  %ld o\n", sizeMB);
    fprintf(file, "size C:  %ld o\n", sizeMC);
    fprintf(file, "total :  %ld o\n", totalSize);
    fprintf(file, "Non Cash Friendly Time: %3.8f s\n", nonCashFriendlyTime);
    fprintf(file, "Cash Friendly Time: %3.8f s\n", cashFriendlyTime);
    fprintf(file, "Speedup: %3.8f s\n", speedUp);
    fprintf(file, "\n\t-----------------------------------------------------\n\n");
    fclose(file);

    printf("Done.\n");
    return 0;
}

// Timing routine implementation
double read_timer()
{
    static int initialized = 0;
    static struct timeval start;
    struct timeval end;
    if (!initialized)
    {
        gettimeofday(&start, NULL);
        initialized = 1;
    }
    gettimeofday(&end, NULL);
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

// Function to initialize matrices and perform matrix multiplication
void initMatrix()
{
    int i, j;
    double **a, **b, **c;
    double *a_block, *b_block, *c_block;

    // Matrices a, b (to be multiplied), and c (the result matrix)
    a = (double **)malloc(NRA * sizeof(double *));
    b = (double **)malloc(NCA * sizeof(double *));
    c = (double **)malloc(NRA * sizeof(double *));

    // Memory allocation for matrices
    a_block = (double *)malloc(NRA * NCA * sizeof(double));
    b_block = (double *)malloc(NCA * NCB * sizeof(double));
    c_block = (double *)malloc(NRA * NCB * sizeof(double));

    // Initializing pointers to a, b, and c
    for (i = 0; i < NRA; i++)
        a[i] = a_block + i * NCA;
    for (i = 0; i < NCA; i++)
        b[i] = b_block + i * NCB;
    for (i = 0; i < NRA; i++)
        c[i] = c_block + i * NCB;

    printf("Starting sequential matrix multiplication\n");
    printf("Initializing matrices...\n");

    // Initialize matrices
    for (i = 0; i < NRA; i++)
        for (j = 0; j < NCA; j++)
            a[i][j] = (double)(i + j);
    for (i = 0; i < NCA; i++)
        for (j = 0; j < NCB; j++)
            b[i][j] = (double)(i * j);
    for (i = 0; i < NRA; i++)
        for (j = 0; j < NCB; j++)
            c[i][j] = 0.0;

    // Perform matrix multiplication
    matrix_multiply(a, b, c);

    // Free allocated memory
    free(a_block);
    free(b_block);
    free(c_block);
    free(a);
    free(b);
    free(c);
}

void initMatrixLoopInterchange()
{
    int i, j;
    double **a, **b, **c;
    double *a_block, *b_block, *c_block;

    // Matrices a, b (to be multiplied), and c (the result matrix)
    a = (double **)malloc(NRA * sizeof(double *));
    b = (double **)malloc(NCA * sizeof(double *));
    c = (double **)malloc(NRA * sizeof(double *));

    // Memory allocation for matrices
    a_block = (double *)malloc(NRA * NCA * sizeof(double));
    b_block = (double *)malloc(NCA * NCB * sizeof(double));
    c_block = (double *)malloc(NRA * NCB * sizeof(double));

    // Initializing pointers to a, b, and c
    for (i = 0; i < NRA; i++)
        a[i] = a_block + i * NCA;
    for (i = 0; i < NCA; i++)
        b[i] = b_block + i * NCB;
    for (i = 0; i < NRA; i++)
        c[i] = c_block + i * NCB;

    printf("Starting sequential matrix multiplication with loop interchange\n");
    printf("Initializing matrices...\n");

    // Initialize matrices
    for (i = 0; i < NRA; i++)
        for (j = 0; j < NCA; j++)
            a[i][j] = (double)(i + j);
    for (i = 0; i < NCA; i++)
        for (j = 0; j < NCB; j++)
            b[i][j] = (double)(i * j);
    for (i = 0; i < NRA; i++)
        for (j = 0; j < NCB; j++)
            c[i][j] = 0.0;

    // Perform matrix multiplication with loop interchange
    matrix_multiplyLoopInterchange(a, b, c);

    // Free allocated memory
    free(a_block);
    free(b_block);
    free(c_block);
    free(a);
    free(b);
    free(c);
}

// Sequential matrix multiply implementation
void matrix_multiply(double **a, double **b, double **c)
{
    int i, j, k;

    for (i = 0; i < NRA; i++)
    {
        for (j = 0; j < NCB; j++)
        {
            for (k = 0; k < NCA; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Matrix multiply implementation with loop interchange
void matrix_multiplyLoopInterchange(double **a, double **b, double **c)
{
    int i, j, k;

    for (i = 0; i < NRA; i++)
    {
        for (k = 0; k < NCA; k++)
        {
            for (j = 0; j < NCB; j++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}
