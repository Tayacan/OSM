#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_SIZE 128
#define MAX_THREADS 64

int size;

typedef struct
{
    double *row_a;
    double *matrix_b;
    double *row_result;
} task_t;

void rowmult(task_t *task)
{
    int i, j;

    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            task->row_result[i] += task->row_a[j] * task->matrix_b[i + size * j];
        }
    }
}

void* solve_tasks(void *arg)
{
    stack_t *stack = (stack_t*) arg;
    task_t *task;
    while((task = (task_t*)stack_pop(stack)))
    {
        rowmult(task);
        free(task);
    }

    /* We don't use the return value. */
    return NULL;
}

void init_rand_matrix(double *matrix, int size)
{
    int i, j;
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            matrix[i * size + j] = (double) rand();
        }
    }
}

/* Output a matrix of dimensions size x size. */
void output_square_matrix(double *m, int size)
{
    int i, j;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            printf("%3.2f  ", m[i * size + j]);
        }
        putchar('\n');
    }
}

int main(int argc, char** argv)
{
    double *a, *b, *r;
    char *num_end;
    int i, nthreads;
    task_t *task;
    pthread_t *threads;
    struct timespec t1, t2;
    double elapsed;

    stack_t *tasks = malloc(sizeof(stack_t));
    stack_init(tasks);

    srand(time(NULL));

    if(argc > 1)
    {
        size = strtol(argv[1], &num_end, 10);
        if(num_end[0] != '\0')
        {
            fprintf(stderr, "argument not a number: %s\n", argv[1]);
            exit(-1);
        }
        if(size < 0)
        {
            fprintf(stderr, "negative size: %s\n", argv[1]);
            exit(-2);
        }
    }
    else
    {
        size = DEFAULT_SIZE;
    }

    if(argc > 2)
    {
        nthreads = strtol(argv[2], &num_end, 10);
        if(num_end[0] != '\0')
        {
            fprintf(stderr, "argument not a number: %s\n", argv[2]);
            exit(-3);
        }
        if(size < 1)
        {
            fprintf(stderr, "number of threads less than 1: %s\n", argv[2]);
            exit(-4);
        }
    }
    else
    {
        nthreads = MAX_THREADS;
    }
    printf("Multiplying random square matrices of size %d x %d\n",
         size, size);

    // allocate and initialize matrices
    a = (double*) malloc(sizeof(double) * size * size);
    b = (double*) malloc(sizeof(double) * size * size);
    r = (double*) calloc(sizeof(double), size * size);
    init_rand_matrix(a, size);
    init_rand_matrix(b, size);

    // put things on stack
    for(i = 0; i < size; i++)
    {
        task = malloc(sizeof(task_t));
        task->row_a = &a[size * i];
        task->matrix_b = b;
        task->row_result = &r[size * i];
        stack_push(tasks, task);
    }

    threads = (pthread_t*) malloc(nthreads * sizeof(pthread_t));

    clock_gettime(CLOCK_MONOTONIC, &t1);
    // start threads
    for(i = 0; i < nthreads; i++)
    {
        pthread_create(&threads[i], NULL, solve_tasks, tasks);
    }

    // join threads
    for(i = 0; i < nthreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2);

    if (size < 20) {
        printf("\nResult:\n");
        output_square_matrix(r, size);
    }
    elapsed = t2.tv_sec - t1.tv_sec;
    elapsed += (t2.tv_nsec - t1.tv_nsec) / 1000000000.0;
    printf("Time spent: %F\n", elapsed);
    //printf("stuff %ld\n", _POSIX_TIMERS);

    // Free ALL THE THINGS!
    free(threads);
    free(a);
    free(b);
    free(r);
    free(tasks);

    return 0;
}
