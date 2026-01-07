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
    int rank, size;                                         // Rank of the process and total number of processes
    int number;                                             // The number held by this process
    int left_number;                                        // Number received from the left neighbour

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *all_numbers = NULL;                                // Used by rank 0 to read input
    all_numbers = (int *)malloc(size * sizeof(int));        // Allocate memory for the input numbers on rank 0

    int *all_order_statuses = NULL;                         // Array to hold results on rank 0
    all_order_statuses = (int *)malloc(size * sizeof(int)); // Allocate memory for the order statuses on rank 0

    // rank 0 gets input dynamically from terminal
    // rank 0 checks if numbers and number of ranks are equal, else throw error message
    // rank 0 scatters to every other process
    if (rank == 0) {
        // Check if we have the right number of arguments (program name + one number per process)
        if (argc != size + 1) {
            MPI_Finalize(); // Need to tell MPI to quit before exiting
            return 1; // Indicate error
        }

        // Read numbers from command line arguments into the array
        for (int i = 0; i < size; i++) {
            all_numbers[i] = atoi(argv[i + 1]); // Convert text argument to integer
        }
    }

    // Send one number from rank 0's array to each process
    MPI_Scatter(
        all_numbers,    // Array on rank 0
        1,              // Send 1 integer
        MPI_INT,        // Type is integer
        &number,        // Where to receive the integer
        1,              // Receive 1 integer
        MPI_INT,        // Type is integer
        0,              // Rank 0 is the sender
        MPI_COMM_WORLD
    );

    /*
     * Determine neighbour relationships between tasks in the ring
     * Left neighbour: (rank - 1 + size) % size (wrap-around using modulo)
     * Right neighbour: (rank + 1) % size (wrap-around)
     */
    int left = (rank - 1 + size) % size;    // Calculation of left neigbor rank
    int right = (rank + 1) % size;          // Calculation of right neigbor rank

    /*
    * Each process sends its number to its right neighbor and then receives a number from its left neighbor.
    */
    // Send the number to the right neighbor.
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
     * Every process sends answer in form of flag to master rank 0 and only rank 0 prints it to console
     */
    int am_i_out_of_order = 0; // Default: assume in order

    if (rank != 0) { // Process 0 doesn't check itself
        if (number < left_number) {
            am_i_out_of_order = 1; // Set flag if out of order
        }
    }

    // Each process sends its 'am_i_out_of_order' flag to rank 0
    MPI_Gather(
        &am_i_out_of_order,      // Data each process sends (its flag)
        1,                       // Count of items each process sends
        MPI_INT,                 // Type of item sent
        all_order_statuses,      // Buffer where rank 0 receives data
        1,                       // Count of items rank 0 receives *from each process*
        MPI_INT,                 // Type of item received
        0,                       // Rank of the receiving process (root)
        MPI_COMM_WORLD
    );

    // Rank 0 Prints the Results
    if (rank == 0) {
        int out_of_order_count = 0; // Counter for out-of-order processes

        // Loop through the results gathered from *all* processes (0 to size-1)
        for (int i = 0; i < size; i++) {
            if (all_order_statuses[i] == 1) { // Check the flag received from process i
                printf("Process %d has number %d out of order.\n", i, all_numbers[i]);
                out_of_order_count++; // Increment the counter
                // Indicate that the process has finished executing.
                printf("Process %d has now finished.\n", rank);
            }
        }

        // Final summary line with the total count
        printf("The number of processes holding an out-of-order number is %d.\n", out_of_order_count);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}