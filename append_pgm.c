/*
Copyright 2022 Ivan Carvalho

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/
#include "append_pgm.h"
#include <math.h>
#include <stdio.h>

append_pgm* appendPGMInit(size_t size, size_t maxError) {
    /* Basic details */
    append_pgm* pgm = malloc(sizeof (append_pgm));
    pgm->size = size;
    pgm->maxError = maxError;
    pgm->count = 0;
    cvector_reserve(pgm->levels, 1);

    /* First level */
    pgm->num_levels = 1;
    size_t div_factor = 2*pgm->maxError;
    if(div_factor < 1) {
        div_factor = 1;
    }

    size_t level_size = (pgm->size / div_factor) + 1;
    one_level_pgm* new_level = oneLevelPGMInit(level_size, pgm->maxError);
    cvector_push_back(pgm->levels, new_level);

    return pgm;
}

void appendPGMBuild(append_pgm *pgm, pgm_key_t* keys, size_t size, size_t maxError) {
    for (size_t i = 0; i < size; i++) {
        appendPGMAdd(pgm, keys[i]);
    }
}

void appendPGMAdd(append_pgm *pgm, pgm_key_t key) {

    pgm->count += 1;

    for(size_t current = 0; current < pgm->num_levels; current++){
        size_t previous_level_size = cvector_size(pgm->levels[current]->level);

        if (current == 0) {
            oneLevelPGMAdd(pgm->levels[current], key);
        }
        else {
            pgm_key_t last_key = pgm->levels[current - 1]->latest_pair.x;
            oneLevelPGMAdd(pgm->levels[current], last_key);
        }

        size_t new_level_size = cvector_size(pgm->levels[current]->level);
        if (new_level_size == previous_level_size) { // no new point, leave upper levels intact
            return;
        }
    }

    if (cvector_size(pgm->levels[pgm->num_levels - 1]->level) > 1) {
        size_t div_factor = 2*pgm->maxError;
        if(div_factor < 1) {
            div_factor = 1;
        }

        size_t level_size = (pgm->levels[pgm->num_levels - 1]->size / div_factor) + 1;
        one_level_pgm* new_level = oneLevelPGMInit(level_size, pgm->maxError);
        cvector_push_back(pgm->levels, new_level);
        pgm->num_levels += 1;

        oneLevelPGMAdd(pgm->levels[pgm->num_levels - 1], pgm->levels[pgm->num_levels - 2]->level[0].pos);
        oneLevelPGMAdd(pgm->levels[pgm->num_levels - 1], pgm->levels[pgm->num_levels - 2]->level[1].pos);
    }

}

pgm_approx_pos appendPGMApproxSearch(append_pgm *pgm, pgm_key_t key) {
    pgm_approx_pos answer;

    if(key < pgm->levels[0]->smallest_key) {
        answer.lo = 1;
        answer.hi = 0;
        return answer;
    }

    one_level_pgm* level_pgm = pgm->levels[pgm->num_levels - 1];
    size_t maxError = level_pgm->maxError;
    size_t model_index = 0;

    for (size_t current = pgm->num_levels - 1; current >= 0;) {
        size_t level_size = level_pgm->count;
        double pred = level_pgm->level[model_index].a * ((double)key) + level_pgm->level[model_index].b;
        double lo_pred = pred - maxError;
        lo_pred = (lo_pred < 0) ? (0) : (lo_pred);

        double hi_pred = pred + (double)maxError + 1;

        answer.lo = (size_t)lo_pred;
        answer.hi = (size_t)hi_pred;
        answer.lo = (answer.lo <= (level_size - 1)) ? (answer.lo) : (level_size - 1);
        answer.hi = (answer.hi <= (level_size - 1)) ? (answer.hi) : (level_size - 1);

        if (current >= 1) {
            level_pgm = pgm->levels[current - 1];
            for (size_t i = answer.hi; i >= answer.lo;) {
                if (key >= level_pgm->level[i].pos) {
                    model_index = i;
                    break;
                }
                if (i > 0) {
                    i -= 1;
                }
            }
            current -= 1;
        }
        else {
            break;
        }

    }

    return answer;
}

void appendPGMFree(append_pgm *pgm) {
    for(size_t i = 0; i < pgm->num_levels; i++){
        oneLevelPGMFree(pgm->levels[i]);
    }

    cvector_free(pgm->levels);
    free(pgm);
}