/* Ethan Sorkin and Nick Camp
* Comp40
* HW2, iii
* bit.c: implementation of our Bit2_T representation
*/



#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "list.h"
#include "seq.h"
#include "str.h"
#include "bit.h"
#include "bit2.h"
#define T Bit2_T

struct T {
        int width;
        int height;
        Bit_T data;
};


int Bit2_index(T ray, int row, int col);

/* Returns a new 2D array of the specified width and height. It is a checked
runtime error for height or width to be negative */
T Bit2_new(int width, int height)
{
        /* ensuring provided values are valid */
        assert(width >= 0 && height >= 0);

        Bit2_T board = malloc(sizeof(*board));

        board->width = width;
        board->height = height;
        board->data = Bit_new((width * height));
        /* using a Hanson UArray as a member of our Bit2_T */

        return board;

}

/* deallocates and clears the given array. It is a checked runtime error for
the provided array to be NULL. */
void Bit2_free(T *ray)
{
        assert(ray != NULL && *ray != NULL);
        Bit_free(&(*ray)->data);
        free(*ray);

}

/* returns the height of the given 2D array. It is a checked runtime error for
the provided array to be NULL. */
int Bit2_height(T ray)
{
        assert(ray != NULL);
        return ray->height;
}

/* returns the width of the 2D array. It is a checked runtime error for the
provided array to be NULL. */
int Bit2_width(T ray)
{
        assert(ray != NULL);
        return ray->width;
}

/* returns the number of ones in ray. It is a checked runtime error for the
provided array to be NULL.*/
int Bit2_count (T ray)
{


(void)ray;
return -1;
}

/* Returns the value of the bit at the given coordinate (1 if it is in ray,
0 otherwise). It is a checked runtime error for the provided index to be out
of scope or for the Bit2_T to be NULL*/
int Bit2_get(T ray, int col, int row)
{
        assert(ray != NULL);
        assert(row < ray->height && row >= 0);  /* ensuring row is in scope */
        assert(col < ray->width && col >= 0);   /* ensuring col is in scope */

        int index = Bit2_index(ray, row, col);


        return Bit_get(ray->data, index);
}

/* Sets the element at the given index to val and returns the previous value
of the bit in that location. It is a checked runtime error for the provided
index to be out of scope, for the Bit2_T to be NULL, or for the bit to not 
equal 1 or 0 */
int Bit2_put(T ray, int col, int row, int val)
{
        assert(ray != NULL);
        assert(row < ray->height && row >= 0);   /* ensuring row is in scope */
        assert(col < ray->width && col >= 0);    /* ensuring col is in scope */
        assert(val == 0 || val == 1); /* ensuring bit value is either 1 or 0 */

        int index = Bit2_index(ray, row, col);
        int replaced_val = Bit_get(ray->data, index);
        Bit_put(ray->data, index, val);

        return replaced_val;
}

/* Because we will be using a single array to represent our matrix, we will
need to access each element using its index equivalent to a specified row and
column. This function will return the index corresponding to a row/column
coordinate in our 2D array. This is a helper function, not something a client
would use. */
int Bit2_index(T ray, int row, int col)
{
        /* no need to call assert on ray, row, or col, because we already did
        that in Bit2_at, and that function is the only time Bit2_index
        is ever called */

        int width = Bit2_width(ray);
        int index = (width)*(row) + col;
        return index;
}

/* Calls  apply function on elements of the 2D array, in order by row */
void Bit2_map_row_major(T ray, void apply (int col, int row,
                             T bitvec, int element, void *unused), void *cl)
{
        assert(ray != NULL);

        for (int i = 0; i < Bit2_height(ray); i++) /* row by row */
        {
                for (int j = 0; j < Bit2_width(ray); j++) /* col by col */
                {
                        apply(j, i, ray, Bit2_get(ray, j, i), cl);
                }
        }
}

/* Calls apply function on elements of the 2D array, in order by column */
void Bit2_map_col_major(T ray, void apply (int col, int row, T bitvec, 
                                       int element, void *unused), void *cl)
{
        assert(ray != NULL);

        for (int i = 0; i < Bit2_width(ray); i++) /* column by column */
        {
                for (int j = 0; j < Bit2_height(ray); j++) /* row by row */
                {
                        apply(i, j, ray, Bit2_get(ray, i, j), cl);
                }
        }
}

