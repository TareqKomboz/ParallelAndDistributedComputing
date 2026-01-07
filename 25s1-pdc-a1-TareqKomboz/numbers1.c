/*
* Name:           Tareq Abu El Komboz
* StudentID:      A1976365
* Description:    This is Assignement 1 Code Submission Program. The objective is to find numbers out of order in a ring topology, and print information about them.
*/

// Import libraries
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Main executing code
int main(int argc, char *argv[]) {
    // Initialize variables for each process
    int rank, size;                                     // Rank of the process and total number of processes
    int number;                                         // The number held by this process
    int left_number;                                    // Number received from the left

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *all_numbers = NULL;                            // Used to read input
    all_numbers = (int *)malloc(size * sizeof(int));    // Allocate memory for the input numbers

    // Check if we have the right number of arguments (program name + one number per process)
    if (argc != size + 1) {
        MPI_Finalize(); // Need to tell MPI to quit before exiting
        return 1; // Indicate error
    }

    // Read numbers from command line arguments into the array
    for (int i = 0; i < size; i++) {
        all_numbers[i] = atoi(argv[i + 1]); // Convert text argument to integer
    }

    /*
     * Determine neighbour relationships between tasks in the ring
     * Left neighbour: (rank - 1 + size) % size (wrap-around using modulo)
     * Right neighbour: (rank + 1) % size (wrap-around)
     */
    int left = (rank - 1 + size) % size;    // Calculation of left neigbor rank
    int right = (rank + 1) % size;          // Calculation of right neigbor rank

    // Get relevant numbers
    number = all_numbers[rank];
    left_number = all_numbers[left];

    /*
    * Each process sends its number to its right neighbor and then receives a number from its left neighbor.
    */
    MPI_Send(
        &number,           // Pointer to the message buffer
        1,                 // Number of elements in the message
        MPI_INT,           // Datatype of the message
        right,             // Destination process rank
        0,                 // Message tag
        MPI_COMM_WORLD     // Communicator
    );
    // Receive a number from the left neighbor.
    MPI_Recv(
        &left_number,      // Pointer to the buffer where the received data will be stored
        1,                 // Number of elements expected
        MPI_INT,           // Datatype of the message
        left,              // Source process rank
        0,                 // Message tag
        MPI_COMM_WORLD,    // Communicator
        MPI_STATUS_IGNORE  // Status object (ignored)
    );

    /*
     * For all processes except process 0, check if the number is out of order.
     * A process is out of order if its number is less than the number it received
     * from its left neighbour.
     */
    if (rank != 0) { // All but rank 0 check number to their left, because 0 has no left neighbor
        if (number < left_number) { // Left check if in order, right not needed
            printf("Process %d has at least one number out of order.\n", rank); // If check failed
        }
    }

    // Indicate that the process has finished executing.
    printf("Process %d has now finished.\n", rank);

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}