/*
Copyright 2022 Ivan Carvalho

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/
#include "append_pgm.h"
#include <math.h>

append_pgm* appendPGMInit(size_t size, size_t maxError) {
    /* Basic details */
    append_pgm* pgm = malloc(sizeof (append_pgm));
    pgm->size = size;
    pgm->maxError = maxError;
    pgm->count = 0;

    pgm->first_level = oneLevelPGMInit(size, maxError);

    return pgm;
}