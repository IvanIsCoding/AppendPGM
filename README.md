# AppendPGM [![License](https://img.shields.io/github/license/IvanIsCoding/AppendPGM.svg)](https://opensource.org/licenses/MIT)

A pure C implementation of the Piecewise Geometric Model index (PGM) with support for append operations.

This library is a work-in-progress, and its API might change.

## Usage

This library is header-only. To compile, include the headers:

```
gcc examples/simple.c -I./include -o simple
./simple
```

The above command compiles the `examples/simple.c` and executes it:

```c
#include <stdio.h>
#include <stdlib.h>
#include "append_pgm/append_pgm.h"

int main() {
    // Generate data
    size_t n_bottom = 100000;
    size_t n_second = 100; 
    size_t epsilon = 1;
    append_pgm* pgm = appendPGMInit(n_bottom, n_second, epsilon);

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

    return 0;
}
```

## Citation

If you use PGM in your research, please cite the original work of Ferragina and Vinciguerra.

```tex
@article{Ferragina:2020pgm,
  Author = {Paolo Ferragina and Giorgio Vinciguerra},
  Title = {The {PGM-index}: a fully-dynamic compressed learned index with provable worst-case bounds},
  Year = {2020},
  Volume = {13},
  Number = {8},
  Pages = {1162--1175},
  Doi = {10.14778/3389133.3389135},
  Url = {https://pgm.di.unipi.it},
  Issn = {2150-8097},
  Journal = {{PVLDB}}}
```