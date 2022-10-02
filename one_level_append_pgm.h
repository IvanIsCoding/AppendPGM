/*
Copyright 2022 Ivan Carvalho

Use of this source code is governed by an MIT-style
license that can be found in the LICENSE file or at
https://opensource.org/licenses/MIT.
*/
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef ONE_LEVEL_APPEND_PGM_H
#define ONE_LEVEL_APPEND_PGM_H

#define CVECTOR_LOGARITHMIC_GROWTH /* for C Vector */

#include <stdint.h>
#include <stdbool.h>
#include "cvector.h"

/* Define type for keys and location ids. */
typedef uint32_t pgm_key_t;
//typedef uint32_t size_t;

typedef struct {
	pgm_key_t x;
	size_t y;
} point_pair;

// Stores line ax + b starting at x = POS
typedef struct {
	double a;
	double b;
	pgm_key_t pos;
} line_segment;

typedef struct {
	size_t lo;
	size_t hi;
} pgm_approx_pos;

typedef struct {
	/* Underlying data */
	uint32_t maxError;						   /* Maximum error              */	
	size_t count;							  /* Number of points in spline */
	size_t size;							 /* Maximum number of points   */

	/* Implementation details */
	cvector_vector_type(line_segment) level; /* One and only level of PGM */

	pgm_key_t smallest_key;
	pgm_key_t largest_key;
	point_pair latest_pair;
	double upper_a, lower_a;

} one_level_pgm;


one_level_pgm* oneLevelPGMInit(size_t size, size_t maxError);

void oneLevelPGMBuild(one_level_pgm *pgm, pgm_key_t* keys, size_t size, size_t maxError);

void oneLevelPGMAdd(one_level_pgm *pgm, pgm_key_t key);

pgm_approx_pos oneLevelPGMApproxSearch(one_level_pgm *pgm, pgm_key_t key);

void oneLevelPGMFree(one_level_pgm *pgm);

#if defined(__cplusplus)
}
#endif

#endif