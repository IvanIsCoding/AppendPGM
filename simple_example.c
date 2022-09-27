#include <stdio.h>
#include <stdlib.h>
#include "one_level_append_pgm.h"

int main() {
    // Generate some random data
    size_t n = 100000;
    size_t epsilon = 2;
    one_level_pgm* pgm = oneLevelPGMInit(n, epsilon);

    for(uint32_t i = 10; i <= 10000; i++){
        if (i % 2 == 1) {
            oneLevelPGMAdd(pgm, 3*i, 3*i);  
        }
        else {
            uint32_t random_offset = (rand()) % (i - 1);
            if(random_offset == 0) random_offset++;
            oneLevelPGMAdd(pgm, 3*(i - 1) + 1, 3*i);
        }
    }

    uint32_t val = 0;
    bool isThere = oneLevelPGMSearch(pgm, 57, &val);

    if (isThere) {
        printf("Found: %d\n", val);
    }
    else {
        printf("Not found: %d\n", val);
    }

    return 0;
}