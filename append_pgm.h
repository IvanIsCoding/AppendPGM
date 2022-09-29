/*
Copyright 2022 Ivan Carvalho

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef APPEND_PGM_H
#define APPEND_PGM_H

#define CVECTOR_LOGARITHMIC_GROWTH /* for C Vector */

#include <stdint.h>
#include <stdbool.h>
#include "cvector.h"
#include "one_level_append_pgm.h"

typedef struct {
	int test_var;
} pgm_level;

typedef struct {
	/* Underlying data */
	uint32_t maxError;						   /* Maximum error              */	
	size_t count;							  /* Number of points in spline */
	size_t size;							 /* Maximum number of points   */

	/* Implementation details */
	cvector_vector_type(pgm_level) levels; /* One and only level of PGM */

	one_level_pgm* first_level;

} append_pgm;


append_pgm* appendPGMInit(size_t size, size_t maxError);

void appendPGMBuild(append_pgm *pgm, pgm_key_t* keys, pgm_val_t* values, size_t size, size_t maxError);

void appendPGMAdd(append_pgm *pgm, pgm_key_t key, pgm_val_t val);

bool appendPGMSearch(append_pgm *pgm, pgm_key_t key, pgm_val_t* val);

void appendPGMFree(append_pgm *pgm);

#if defined(__cplusplus)
}
#endif

#endif