
# README

## How to Compile and Run

All programs must be compiled with the `-DNO_OUTPUT` flag to suppress result output and show only timing information.

Use the following command structure for each version:

```bash
gcc -g -Wall -fopenmp -DNO_OUTPUT -o columnX columnX.c -lm
```

Replace `X` with 1, 2, 3, or 4 depending on the version.

To run each compiled program, use:

```bash
./columnX 1 400 1000 0.01 1000 g
./columnX 2 400 1000 0.01 1000 g
./columnX 4 400 1000 0.01 1000 g
```

- `1, 2, 4`: Number of threads
- The remaining parameters stay the same for all runs and columns.

## How to Collect Performance Data

1. Run each command **five times**.
2. Copy the terminal output manually into the designated folder.
3. Identify the **minimum runtime** from the five runs. This is the final result.

## System Requirements & Dependencies

No external libraries are required beyond the standard C libraries and OpenMP. Ensure your system supports OpenMP.

The following headers are used:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
```
