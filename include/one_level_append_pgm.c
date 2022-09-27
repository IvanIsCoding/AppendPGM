#include "one_level_append_pgm.h"
#include <math.h>

void oneLevelPGMInit(one_level_pgm *pgm, size_t size, size_t maxError) {

    /* Basic details */
    pgm = malloc(sizeof (one_level_pgm));
    pgm->kv_pairs = malloc(sizeof(key_value_pair) * size);
    pgm->size = size;
    pgm->maxError = maxError;

    /* Allocate memory for PGM level */
    size_t div_factor = 2*maxError;
    if(div_factor < 1) {
        div_factor = 1;
    }

    size_t level_size = (size / div_factor) + 1;
    cvector_reserve(pgm->level, level_size); 

}

void oneLevelPGMAdd(one_level_pgm *pgm, key_t key, val_t val) {
    if (pgm->count > 2) {

        double y_val = (double)pgm->count;

        double upper_prediction = (pgm->upper_a)*((double)key - (double)(pgm->latest_pair.x)) + (double)(pgm->latest_pair.y);

        double lower_prediction = (pgm->lower_a)*((double)key - (double)(pgm->latest_pair.x)) + (double)(pgm->latest_pair.y);

        /* Line 7 from Algorithm 1: Swing Filter  */
        if (
            (y_val - upper_prediction) > pgm->maxError 
            || (y_val - lower_prediction) < -pgm->maxError
        ) {
            // TODO: new record
        }
        else { /* Line 13 from Algorithm 1: Swing Filter  */
            // Line 15 from Algorithm 1: Swing Filter
            /* TODO */

            // Line 17 from Algorithm 1: Swing Filter

        }

    }
    else if(pgm->count == 1) {
        double dy = (double)1;
        double dx = ((double)key) - ((double)pgm->latest_pair.x);

        line_segment seg;
        seg.a = dy/dx;
        seg.b = ((double)pgm->latest_pair.y) - ((double)pgm->latest_pair.x)*(seg.a); // TODO

        cvector_push_back(pgm->level, seg);

        /* Calculate coefficient of upper segment */
        pgm->upper_a = ((double)(1 + pgm->maxError))/dx;

        /* Calculate coefficient of lower segment */
        pgm->upper_a = ((double)(1 - pgm->maxError))/dx;

        /* Add key-value pair */
        pgm->kv_pairs[pgm->count].x = key;
        pgm->kv_pairs[pgm->count].y = val;
        pgm->count += 1;
    }
    else {
        /* Add key-value pair */
        pgm->smallest_key = key;
        pgm->kv_pairs[pgm->count].x = key;
        pgm->kv_pairs[pgm->count].y = val;
        pgm->latest_pair.x = key;
        pgm->latest_pair.y = pgm->count;

        /* Increment for next item */
        pgm->count += 1;
    }
}

void oneLevelPGMBuild(one_level_pgm *pgm, key_t* keys, val_t* values, size_t size, size_t maxError) {

    for (size_t i = 0; i < size; i++) {
        oneLevelPGMAdd(pgm, keys[i], values[i]);
    }

}

bool oneLevelPGMSearch(one_level_pgm *pgm, key_t key, val_t* val) {
    
    if(key < pgm->smallest_key) {
        return false;
    }

    return false;

}

void oneLevelPGMFree(one_level_pgm *pgm) {
    free(pgm->kv_pairs);
    cvector_free(pgm->level);
    free(pgm);
}