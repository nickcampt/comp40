/* Ethan Sorkin and Nick Camp
* Comp40
* HW2, iii
* uarray2.c: implementation for our UArray2_T representation
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "list.h"
#include "seq.h"
#include "str.h"
#include "uarray.h"
#include "uarray2.h"
#define T UArray2_T

struct T {
        int width;
        int height;
        int size;
        UArray_T data;
};

int UArray2_index(T ray, int row, int col);

/* returns a pointer to the element stored at the given row/column index. It is
a checked runtime error for the provided index to be out of scope */
void  *UArray2_at(T ray, int col, int row)
{
        assert(ray != NULL);
        assert(row < ray->height && row >= 0);  /* ensuring row is in scope */
        assert(col < ray->width && col >= 0);   /*ensuring col is in scope */

        int index = UArray2_index(ray, row, col);

        return UArray_at(ray->data, index); 
}

/* Returns a new 2D array of the specified width and height, where each element
can hold 'size' bytes. It is a checked runtime error for size, height, or
width to be negative */
T UArray2_new(int width, int height, int size)
{
        /* ensuring provided values are valid */
        assert(width >= 0 && height >= 0 && size >= 0);

        UArray2_T board = malloc(sizeof(*board));

        board->width = width;
        board->height = height;
        board->size = size;
        board->data = UArray_new((width * height), size);
        /* using a Hanson UArray as a member of our UArray2_T */

        return board;
}

/* deallocates and clears the given array. It is a checked runtime error for
the provided array to be NULL. */
void UArray2_free(T *ray)
{
        assert(ray != NULL && *ray != NULL);
        UArray_free(&(*ray)->data);
        free(*ray);
}

/* Calls  apply function on elements of the 2D array, row by row */
void UArray2_map_row_major(T ray, void apply (int col, int row,
     T uarray, void *element, void *unused), void *cl)
{
        assert(ray != NULL);

        for (int i = 0; i < UArray2_height(ray); i++) /* row by row */
        {
                for (int j = 0; j < UArray2_width(ray); j++) /* col by col */
                {
                        apply(j, i, ray, UArray2_at(ray, j, i), cl);
                }
        }
}

/* Calls apply function on elements of the 2D array, in order by column */
void UArray2_map_col_major(T ray, void apply (int col, int row,
     T uarray, void *element, void *unused), void *cl)
{

        assert(ray != NULL);

        for (int i = 0; i < UArray2_width(ray); i++) /* column by column */
        {
                for (int j = 0; j < UArray2_height(ray); j++) /* row by row */
                {
                        apply(i, j, ray, UArray2_at(ray, i, j), cl);
                }
        }
}


/* Because we will be using a single array to represent our matrix, we will
need to access each element using its index equivalent to a specified row and
column. This function will return the index corresponding to a row/column
coordinate in our 2D array. This is a helper function, not something a client
would use. */
int UArray2_index(T ray, int row, int col)
{
        /* no need to call assert on ray, row, or col, because we already did
        that in UArray2_at, and that function is the only time UArray_index
        is ever called */

        int width = UArray2_width(ray);
        int index = (width)*(row) + col;
        return index;
}

/* returns the height of the given 2D array. It is a checked runtime error for
the provided array to be NULL. */
int UArray2_height(T ray)
{
        assert(ray != NULL);
        return ray->height;
}

/* returns the width of the 2D array. It is a checked runtime error for the
provided array to be NULL. */
int UArray2_width(T ray)
{
        assert(ray != NULL);
        return ray->width;
}

/* Returns the size of the elements in the provided array. It is a checked
runtime error for the provided array to be NULL. */
int UArray2_size(T ray)
{
        assert(ray != NULL);
        return ray->size;
}
