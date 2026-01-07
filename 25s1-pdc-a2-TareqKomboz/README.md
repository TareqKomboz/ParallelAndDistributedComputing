This ReadMe describes how to compile and run the code for part 1 in the file mvmult.c
To compile it, execute the following command in your terminal:
mpicc -g -Wall -o mvmult mvmult.c

After you compiled the program, you can execute it using the following command in your terminal:
mpiexec -n <num_processors> ./mvmult

where you have to replace <num_processors> with the actual number of processors.