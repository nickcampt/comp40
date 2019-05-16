/* Ethan Sorkin and Nick Camp
* COMP-40
* HW2, iii
* uarray2.h: interface for our UArray2_T representation
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "list.h"
#include "seq.h"
#include "str.h"
#include "uarray.h"
#ifndef UARRAY2_H_INCLUDED
#define UARRAY2_H_INCLUDED
#define T UArray2_T


typedef struct T *T;

T UArray2_new(int width, int height, int size);

void UArray2_free(T *ray);

int UArray2_height(T ray);

int UArray2_width(T ray);

void  *UArray2_at(T ray, int row, int col);

void UArray2_map_row_major(T ray, void apply (int col, int row,
     T uarray, void *element, void *unused), void *cl);

void UArray2_map_col_major(T ray, void apply (int col, int row,
     T uarray, void *element, void *unused), void *cl);

int UArray2_size(T ray);



#undef T
#endif
