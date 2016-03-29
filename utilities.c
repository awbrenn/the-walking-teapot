#include <stdio.h>
#include <stdlib.h>

void dieWithError(char* message) {
   fprintf(stderr, "ERROR: %s\n", message);
   exit(-1);
}
