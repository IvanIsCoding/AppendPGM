/*
Copyright 2022 Ivan Carvalho

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/
#include "one_level_append_pgm.h"
#include <math.h>

one_level_pgm* oneLevelPGMInit(size_t size, size_t maxError) {
    /* Basic details */
    one_level_pgm* pgm = malloc(sizeof (one_level_pgm));
    pgm->kv_pairs = malloc(sizeof(key_value_pair) * size);
    pgm->size = size;
    pgm->maxError = maxError;
    pgm->count = 0;

    /* Allocate memory for PGM level */
    size_t div_factor = 2*maxError;
    if(div_factor < 1) {
        div_factor = 1;
    }

    size_t level_size = (size / div_factor) + 1;
    cvector_reserve(pgm->level, level_size);

    return pgm;
}

void oneLevelPGMAdd(one_level_pgm *pgm, pgm_key_t key, pgm_val_t val) {
    if (pgm->count >= 2) {

        double y_val = (double)pgm->count;

        double upper_prediction = (pgm->upper_a)*((double)key - (double)(pgm->latest_pair.x)) + (double)(pgm->latest_pair.y);

        double lower_prediction = (pgm->lower_a)*((double)key - (double)(pgm->latest_pair.x)) + (double)(pgm->latest_pair.y);

        /* Line 7 from Algorithm 1: Swing Filter  */
        if (
            (y_val - upper_prediction) > (double)pgm->maxError 
            || (y_val - lower_prediction) < -(double)pgm->maxError
        ) { // Add record
            pgm->latest_pair.x = pgm->kv_pairs[pgm->count - 1].x;
            pgm->latest_pair.y = pgm->count - 1;

            double dy = (double)1;
            double dx = ((double)key) - ((double)pgm->latest_pair.x);

            line_segment seg;
            seg.a = dy/dx;
            seg.b = ((double)pgm->latest_pair.y) - ((double)pgm->latest_pair.x)*(seg.a);
            seg.pos = pgm->latest_pair.x;

            cvector_push_back(pgm->level, seg);

            /* Calculate coefficient of upper segment */
            pgm->upper_a = ((double)(1.0 + (double)pgm->maxError))/dx;

            /* Calculate coefficient of lower segment */
            pgm->lower_a = ((double)(1.0 - (double)pgm->maxError))/dx;
        }
        else { /* Line 13 from Algorithm 1: Swing Filter  */
            // Line 15 from Algorithm 1: Swing Filter
            if ((y_val - lower_prediction) > (double)pgm->maxError) {
                double dy = (double)(pgm->count - pgm->latest_pair.y);
                double dx = ((double)key) - ((double)pgm->latest_pair.x);
                pgm->lower_a = ((double)(dy - (double)pgm->maxError))/dx;
            }

            // Line 17 from Algorithm 1: Swing Filter
            if ((y_val - upper_prediction) < -(double)pgm->maxError) {
                double dy = (double)(pgm->count - pgm->latest_pair.y);
                double dx = ((double)key) - ((double)pgm->latest_pair.x);
                pgm->upper_a = ((double)(dy + (double)pgm->maxError))/dx;
            }

            // This diverges from Swinger algorithm
            // as we don't calculate the A with Mean-Square error, just the average
            size_t level_last = cvector_size(pgm->level) - 1;
            pgm->level[level_last].a = (pgm->upper_a + pgm->lower_a)/2.0;
            pgm->level[level_last].b = ((double)pgm->latest_pair.y) - ((double)pgm->latest_pair.x)*((pgm->upper_a + pgm->lower_a)/2.0);
        }

    }
    else if(pgm->count == 1) { // Add first record
        double dy = (double)1.0;
        double dx = ((double)key) - ((double)pgm->latest_pair.x);

        line_segment seg;
        seg.a = dy/dx;
        seg.b = ((double)pgm->latest_pair.y) - ((double)pgm->latest_pair.x)*(seg.a);
        seg.pos = pgm->latest_pair.x;

        cvector_push_back(pgm->level, seg);

        /* Calculate coefficient of upper segment */
        pgm->upper_a = ((double)(1.0 + (double)pgm->maxError))/dx;

        /* Calculate coefficient of lower segment */
        pgm->lower_a = ((double)(1.0 - (double)pgm->maxError))/dx;
    }
    else {
        /* Level Setup */
        pgm->smallest_key = key;
        pgm->latest_pair.x = key;
        pgm->latest_pair.y = pgm->count;
    }

    /* Add key-value pair */
    pgm->kv_pairs[pgm->count].x = key;
    pgm->kv_pairs[pgm->count].y = val;
    /* Increment for next item */
    pgm->count += 1;
}

void oneLevelPGMBuild(one_level_pgm *pgm, pgm_key_t* keys, pgm_val_t* values, size_t size, size_t maxError) {
    for (size_t i = 0; i < size; i++) {
        oneLevelPGMAdd(pgm, keys[i], values[i]);
    }
}

bool oneLevelPGMSearch(one_level_pgm *pgm, pgm_key_t key, pgm_val_t* val) {
    if(key < pgm->smallest_key) {
        return false;
    }

    size_t window = cvector_size(pgm->level);
    size_t offset = 0;
    while (window > 1) {
      size_t half = window >> 1;
      offset += (pgm->level[offset + half].pos < key) ? half : 0;
      window -= half;
    }

    double pred = pgm->level[offset].a * ((double)key) + pgm->level[offset].b;
    double lo_pred = pred - pgm->maxError;
    lo_pred = (lo_pred < 0) ? (0) : (lo_pred);

    double hi_pred = pred + (double)pgm->maxError + 1;

    size_t lo = (size_t)lo_pred;
    size_t hi = (size_t)hi_pred;
    hi = (hi <= (pgm->count - 1)) ? (hi) : (pgm->count - 1);

    for(size_t i = lo; i <= hi; i++){
        if(pgm->kv_pairs[i].x == key){
            *val = pgm->kv_pairs[i].y;
            return true;
        }
    }

    return false;
}

void oneLevelPGMFree(one_level_pgm *pgm) {
    free(pgm->kv_pairs);
    cvector_free(pgm->level);
    free(pgm);
}