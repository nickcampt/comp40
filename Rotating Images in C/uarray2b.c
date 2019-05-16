/*
 * Nick Camp and Ethan Sorkin
 * uarray2b.c
 * Homework 3 - Locality
 *
 * This is an implementation of our UArray2b representation, inlcuding all
 * function and struct definitions.
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
#include "uarray2b.h"

#define T UArray2b_T

struct T {
        int height_used; /* height of cells with data in them */
        int width_used;  /* width of cells with data in them */
        int ray_height;  /* height allocated by array */
        int ray_width;   /* width allocated by array */
        int blocksize;   /* dimension of a single block */
        int data_size;   /* size of data being stored */
        UArray_T cells;  /* 1D array holding all cell values */
};

int UArray2b_index(T ray2b, int column, int row);

/* creates a new UArray2b with given width, height, size, and blocksize */
T UArray2b_new (int width, int height, int size, int blocksize)
{
        /* ensuring provided values are valid */
        assert(width >= 0 && height >= 0 && size >= 0 && blocksize > 0);

        UArray2b_T grid = (malloc(sizeof(*grid)));

        grid->height_used = height;
        grid->width_used = width;
        grid->data_size = size;
        grid->blocksize = blocksize;

        int array_height = ((int)ceil(((double)height / blocksize)) * 
                                                                    blocksize);
        int array_width = ((int)ceil(((double)width / blocksize)) * blocksize);

        grid->ray_height = array_height;
        grid->ray_width = array_width;

        grid->cells = UArray_new((array_width * array_height), size);

        return grid;
}


/* creates new blocked 2d array: blocksize as large as possible provided
* block occupies at most 64KB (if possible) */
T UArray2b_new_64K_block(int width, int height, int size)
{
        /* ensuring provided values are valid */
        assert(width >= 0 && height >= 0 && size >= 0);

        int blocksize;
        if (size > 64000)
                blocksize = 1;
        else   {
                blocksize = floor(sqrt(64000 / size));
                /* (64000 / size) is the number of total # of cells that can 
                fit in the 64K grid. Since 64000 isnt a power of 2 we use a 
                floor function to make sure that the blocksize is an int */
        }

        UArray2b_T grid = UArray2b_new(width, height, size, blocksize);

        return grid;
}

/* deallocates memory allocated for the given UArray2b */
void UArray2b_free (T *ray2b)
{
        assert(ray2b != NULL && *ray2b != NULL);
        UArray_free(&(*ray2b)->cells);
        free(*ray2b);
}

/* returns width of the given UArray2b */
int UArray2b_width (T ray2b)
{
        assert(ray2b != NULL);
        return ray2b->width_used;
}

/* returns height of the given UArray2b */
int UArray2b_height (T ray2b)
{
        assert(ray2b != NULL);
        return ray2b->height_used;
}

/* returns data size of the given UArray2b */
int UArray2b_size (T ray2b)
{
        assert(ray2b != NULL);
        return ray2b->data_size;
}

/* returns blocksize of the given UArray2b */
int UArray2b_blocksize(T ray2b)
{
        assert(ray2b != NULL);
        return ray2b->blocksize;
}


/* return a pointer to the cell in the given column and row.
* index out of range is a checked run-time error */
void *UArray2b_at(T ray2b, int column, int row)
{
        assert(ray2b != NULL);
        /* ensuring row and column are in scope */
        assert(row < ray2b->height_used && row >= 0); 
        assert(column < ray2b->width_used && column >= 0); 

        int index = UArray2b_index(ray2b, column, row);
        return UArray_at(ray2b->cells, index);
}


/* visits every cell in one block before moving to another block */
void UArray2b_map(T ray2b, void apply(int column, int row, T array2b,
void *elem, void *cl), void *cl)
{
        assert(ray2b != NULL);

        int bs = ray2b->blocksize;
        
        /* shifts row iterator to the right */
        for (int i = 0; i <= ray2b->ray_height; i+= bs) 
        {
                /* shifts col iterator to the right */
                for (int j = 0; j <= ray2b->ray_width; j += bs) 
                {
                        /* iterates through rows */
                        for (int row = 0; row < bs; row++)
                        {
                                /* iterates throught columns */
                                for (int col = 0; col < bs; col++) 
                                {
                                        if(row + i < ray2b->height_used && 
                                                   col + j < ray2b->width_used)
                                        {
                                                apply(col + j, row + i, ray2b,
                                                  UArray2b_at(ray2b, col + j, 
                                                                 row + i), cl);
                                        }
                                }
                        }
                        /* box iteration complete */
                }
                /* boxes of the same row iteration complete */
        }
}

/* returns value of the index in our single UArray that correspond to the
* given row / column pair */
int UArray2b_index(T ray2b, int column, int row)
{
        /* no need to call assert on ray, row, or col, because we already did
        that in UArray2_at, and that function is the only time UArray_index
        is ever called */

        assert(ray2b != NULL);
        int blocksize = ray2b->blocksize;
        int block_row = row / blocksize;
        int block_col = column / blocksize;
        int blocks_per_row = (ray2b->ray_width / blocksize);

        int block_num = (block_row)*blocks_per_row + block_col;  

        /* math is fun :) */
        int index =  ((blocksize) * (row % blocksize)) + (column % blocksize) 
                                      + ((blocksize * blocksize) * block_num);
        return index;
}

/*
* it is a checked run-time error to pass a NULL T
* to any function in this interface
*/
