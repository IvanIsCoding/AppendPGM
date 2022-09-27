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
typedef uint32_t key_t;
typedef uint32_t val_t;
typedef uint32_t size_t;

typedef struct {
	key_t x;
	val_t y;
} key_value_pair;

typedef struct {
	key_t x;
	size_t y;
} point_pair;

// Stores line ax + b starting at x = POS
typedef struct {
	double a;
	double b;
	key_t pos;
} line_segment;

typedef struct {
	/* Underlying data */
	key_value_pair*		kv_pairs;				/* Array of entries */
	uint32_t maxError;						   /* Maximum error */	
	size_t count;							  /* Number of points in spline */
	size_t size;							 /* Maximum number of points */

	/* Implementation details */
	cvector_vector_type(line_segment) level; /* One and only level of PGM */

	key_t smallest_key;
	point_pair latest_pair;
	double upper_a, lower_a;

} one_level_pgm;


void oneLevelPGMInit(one_level_pgm *pgm, size_t size, size_t maxError);

void oneLevelPGMBuild(one_level_pgm *pgm, key_t* keys, val_t* values, size_t size, size_t maxError);

void oneLevelPGMAdd(one_level_pgm *pgm, key_t key, val_t val);

bool oneLevelPGMSearch(one_level_pgm *pgm, key_t key, val_t* val);

void oneLevelPGMFree(one_level_pgm *pgm);

#if defined(__cplusplus)
}
#endif

#endif