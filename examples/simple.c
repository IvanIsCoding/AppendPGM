/*
Copyright 2022 Ivan Carvalho

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/
#include <stdio.h>
#include <stdlib.h>
#include "append_pgm/append_pgm.h"

int main() {
    // Generate data
    size_t n = 100000;
    size_t epsilon = 1;
    append_pgm* pgm = appendPGMInit(n, epsilon);

    size_t V = 31000;
    for(uint32_t i = 1; i <= V; i++){
        appendPGMAdd(pgm, i*i);
    }

    uint32_t val = 0;
    uint32_t count = 0;
    for(uint32_t i = 1; i <= V; i++){
        pgm_approx_pos approx_pos = appendPGMApproxSearch(pgm, i*i);

        if (approx_pos.lo <= i - 1 && i - 1 <= approx_pos.hi) {
            count += 1;
        }

        if (approx_pos.lo > approx_pos.hi) {
            printf("Error at %d\n", i);
            break;
        }
    }

    printf("Count is equal to %d, expected %d\n", count, V);
    printf("Size of PGM in bytes: %d\n", appendPGMSizeBytes(pgm));

    return 0;
}