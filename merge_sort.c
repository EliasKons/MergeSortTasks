#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "timer.h"
#include "my_rand.h"

int is_sorted(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void selection_sort(int* v, int start, int end) {
    int temp;
    for(int i = start; i < end; i++) {
        for(int j = i + 1; j <= end; j++) {
            if(v[i] > v[j]) {
                temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }
    }
}

void merge(int* src, int *dst, int start, int mid, int end) {
    int i = start, j = mid + 1, k = start;
    while(i <= mid && j <= end) {
        if (src[i] <= src[j]) {
            dst[k] = src[i++];
        } 
        else {
            dst[k] = src[j++];
        }
        k++;
    }

    while(i <= mid) {
        dst[k] = src[i++];
        k++;
    }
    while(j <= end) {
        dst[k] = src[j++];
        k++;
    }
}


void merge_sort_rec_parallel(int *arr, int *temp, int start, int end) {
    int mid = (end + start) / 2;

    if(end - start < 32) {
        selection_sort(arr, start, end);
        return;
    }

    #pragma omp task default(none) shared(arr, temp, start, mid) if(end - start >= 3000)
    merge_sort_rec_parallel(arr, temp, start, mid);
    #pragma omp task default(none) shared(arr, temp, mid, end) if(end - start >= 3000)
    merge_sort_rec_parallel(arr, temp, mid + 1, end);
    
    #pragma omp taskwait
    merge(arr, temp, start, mid, end);


    memcpy(arr + start, temp + start, (end - start + 1) * sizeof(int));
}



void merge_sort_parallel(int *arr, int n, int thread_count) {
    int *temp = malloc(n * sizeof(int));

    #pragma omp parallel num_threads(thread_count)
    #pragma omp single
    merge_sort_rec_parallel(arr, temp, 0, n - 1);

    free(temp);
}


void merge_sort_rec_single(int *arr, int *temp, int start, int end) {
    int mid = (end + start) / 2;

    if(end - start < 32) {
        selection_sort(arr, start, end);
        return;
    }

    merge_sort_rec_single(arr, temp, start, mid);
    merge_sort_rec_single(arr, temp, mid + 1, end);

    merge(arr, temp, start, mid, end);


    memcpy(arr + start, temp + start, (end - start + 1) * sizeof(int));
}



void merge_sort_single(int *arr, int n) {
    int *temp = malloc(n * sizeof(int));

    merge_sort_rec_single(arr, temp, 0, n - 1);

    free(temp);
}

