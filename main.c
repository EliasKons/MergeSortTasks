#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "timer.h"
#include "my_rand.h"
#include "merge_sort.h"

int main(int argc, char *argv[]) {
    char *ptr, *output;
    double start, finish;
    FILE *fp;
    int thread_count, *arr;
    long long int n;
    unsigned int seed = 1;

    if(argc != 4) {
        fprintf(stderr, "main [n] [thread count] [output file]\n");
        exit(EXIT_FAILURE);
    }

    n = strtoll(argv[1], &ptr, 10);
    thread_count = atoi(argv[2]);
    output = argv[3];

    if(n <= 0) {
        fprintf(stderr, "N must be non-zero positive!\n");
        exit(EXIT_FAILURE);
    } else if (thread_count <= 0) {
        fprintf(stderr, "Number of threads must be a non-zero positive integer!\n");
        exit(EXIT_FAILURE);
    }
    

    arr = malloc(n * sizeof(n));

    for(int i = 0; i < n; i++) {
        arr[i] = my_rand(&seed);
        seed++;
    }

    if(thread_count == 1) {
        GET_TIME(start);
        merge_sort_single(arr, n);
        GET_TIME(finish);
    }
    else {
        // omp_set_num_threads(thread_count);
        GET_TIME(start);
        merge_sort_parallel(arr, n, thread_count);
        GET_TIME(finish);
    }


    if(is_sorted(arr, n) == 0) {
        fprintf(stderr, "Merge sort failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Array sorted\n");

    free(arr);

    fp = fopen(output, "a");

    if(fp == NULL) {
        perror(output);
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%lld, %d, %e\n", n, thread_count, finish - start);

    fclose(fp);

    return 0;

}

