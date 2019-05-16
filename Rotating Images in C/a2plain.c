/*
 * Nick Camp and Ethan Sorkin
 * a2plain.c
 * Homework 3 - Locality
 *
 * a2plain.c: this is a methods suite which calls functions from our UArray2
 * implementation.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "list.h"
#include "seq.h"
#include "str.h"
#include "math.h"
#include "uarray.h"
#include "uarray2.h"
#include <a2plain.h>

typedef A2Methods_UArray2 A2;   // private abbreviation


/* creates a new UArray2 with given width, height, size, and blocksize */
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}

/* does nothing because UArray2 does not have blocks */
static A2Methods_UArray2 new_with_blocksize(int width, int height,
int size, int blocksize)
{
        (void) blocksize;
        return UArray2_new(width, height, size);
}

/* deallocates memory allocated for the given UArray2 */
static void a2free(A2 * array2p)
{
        UArray2_free((UArray2_T*) array2p);
}

/* returns width of the given UArray2 */
static int width(A2 array2)
{
        return UArray2_width(array2);
}

/* returns width of the given UArray2 */
static int height(A2 array2)
{
        return UArray2_height(array2);
}

/* returns data size of the given UArray2 */
static int size(A2 array2)
{
        return UArray2_size(array2);
}

/* returns blocksize of the given UArray2 */
static int blocksize(A2 array2)
{
        (void)array2;
        return 1;   //in UArray2.c, an individual cell behaves like a block
}

/* return a pointer to the cell in the given column and row.
* index out of range is a checked run-time error */
static A2Methods_Object *at(A2 array2, int i, int j)
{
        return UArray2_at(array2, i, j);
}


typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);

/* calls given apply function in row-major order */
static void map_row_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
        UArray2_map_row_major(array2, (applyfun*) apply, cl);
}

/* calls given apply function in column-major order */
static void map_col_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
        UArray2_map_col_major(array2, (applyfun *) apply, cl);
}

struct small_closure {
        A2Methods_smallapplyfun *apply;
        void *cl;
};

/* apply function when less parameters provided */
static void apply_small(int i, int j, UArray2_T array2, void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)array2;
        cl->apply(elem, cl->cl);
}

/* calls given apply function in row-major order */
static void small_map_row_major(A2 a2, A2Methods_smallapplyfun apply,
                                  void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}

/* calls given apply function in column-major order */
static void small_map_col_major(A2 a2, A2Methods_smallapplyfun apply,
                                  void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}


static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        map_row_major,
        map_col_major,
        NULL,                   // map_block_major,
        map_row_major,          // map_default 
        small_map_row_major,
        small_map_col_major, 
        NULL,                   // small_map_block_major,
        small_map_row_major,    // small_map_default
};


A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;

