/*
Copyright 2022 Ivan Carvalho

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/
#include <stdio.h>
#include <stdlib.h>
#include "one_level_append_pgm.h"

int main() {
    // Generate some random data
    size_t n = 100000;
    size_t epsilon = 1;
    one_level_pgm* pgm = oneLevelPGMInit(n, epsilon);

    size_t V = 10000;
    for(uint32_t i = 1; i <= V; i++){
        oneLevelPGMAdd(pgm, i*i);
    }

    uint32_t val = 0;
    uint32_t count = 0;
    for(uint32_t i = 1; i <= V; i++){
        pgm_approx_pos approx_pos = oneLevelPGMApproxSearch(pgm, i*i);

        if (approx_pos.lo <= i - 1 && i - 1 <= approx_pos.hi) {
            count += 1;
        }

        if (approx_pos.lo > approx_pos.hi) {
            printf("Error at %d\n", i);
            break;
        }
    }

    printf("Count is equal to %d, expected %d\n", count, V);
    printf("Number of lines: %d\n", cvector_size(pgm->level));

    return 0;
}