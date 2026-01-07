/*
 * Assignment 2, Part 2
 * Author: Tareq Abu El Komboz
 * StudentID: A1976365
 *
 * This program computes the sum of numbers 0 to 999 in an array once sequentially and once in parallel.
 * The goal is to get familiar with OpenMP, pragmas and parallelizing loops correctly.
 *
 * Compile:  mpicc -fopenmp -g -Wall -o arraysum arraysum.c
 * Run:      ./arraysum 4
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <omp.h>

 int ARRAY_SIZE = 1000;

 int main(int argc, char *argv[]) {
    // 0. Determine thread count (default = 4)
    int num_threads = 4;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
        if (num_threads <= 0) {
            fprintf(stderr, "Invalid thread count. Falling back to 4.\n");
            num_threads = 4;
        }
    }

    // 1. Initialise the array such that the ith element contains i.
    int *a = (int *) malloc(ARRAY_SIZE * sizeof(int)); // Allocate the array dynamically so that its address is shared

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        a[i] = i;
    }

    // 2. Sequential for loop to sum the elements of the array, and print the sum.
    int seq_sum = 0;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        seq_sum += a[i];
    }

    // Print the serial result
    printf("Sequential sum is %d\n", seq_sum);

    // 3. Parallel for loop, to initialise the array.
    omp_set_num_threads(num_threads);
    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        a[i] = i;  // Every thread works on  a chunk
    }

    // 4. Parallel directive to establish a team of threads to carry out the summation and Reduction clause to compute the global sum.
    int par_sum = 0;
    #pragma omp parallel for reduction(+:par_sum)
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        par_sum += a[i];
    }

    // Print the parallel result
    printf("Parallel sum is %d\n", par_sum);

    return 0;
 }