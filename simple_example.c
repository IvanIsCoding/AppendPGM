#include <stdio.h>
#include <stdlib.h>
#include "one_level_append_pgm.h"

int main() {
    // Generate some random data
    size_t n = 100000;
    size_t epsilon = 3;
    one_level_pgm* pgm = oneLevelPGMInit(n, epsilon);

    size_t V = 10000;
    for(uint32_t i = 1; i <= V; i++){
        oneLevelPGMAdd(pgm, i*i, i);
    }

    uint32_t val = 0;
    uint32_t count = 0;
    for(uint32_t i = 1; i <= V; i++){
        if(oneLevelPGMSearch(pgm, i*i, &val)){
            count += 1;
        } else {
            printf("Error at %d\n", i);
            break;
        }
    }

    printf("Count is equal to %d, expected %d\n", count, V);
    printf("Number of lines: %d\n", cvector_size(pgm->level));

    return 0;
}