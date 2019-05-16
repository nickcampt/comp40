#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "assert.h"
#include "seq.h"
#include "pnmrdr.h"
#include "string.h"
#include "uarray2.h"

#define DIMEN 9
#define DATA_SIZE 4


void read_in_sudoku(Pnmrdr_T pgm, Pnmrdr_mapdata pgmData, UArray2_T board);

bool check_rows(UArray2_T board);

bool check_columns(UArray2_T board);

bool check_boxes(UArray2_T board);

int main(int argc, char *argv[])
{
        /* creates an empty 2D array */
        UArray2_T board = UArray2_new(DIMEN, DIMEN, DATA_SIZE);

        assert (argc <= 2); /* checking number of command line args */
        FILE *file;

        /* getting pnm file from user */
        if (argc == 1)  
        {
                file = stdin; /* file must be read through stdin */
                assert (file != NULL);
        }
        else if (argc == 2) /* filename is provided at command line */
        {
                char *filename = argv[1];
                file = fopen(filename, "r");
                assert (file != NULL);
        }

        Pnmrdr_T pgm;
        assert (pgm = Pnmrdr_new(file)); /* ensuring file is a PNM */
        Pnmrdr_mapdata pgmData = Pnmrdr_data(pgm);
        assert(pgmData.type == 2); /* ensuring file is a PGM */
        assert(pgmData.width == DIMEN && pgmData.height == DIMEN);

        read_in_sudoku(pgm, pgmData, board);

        int exit_code = 0;
        if (check_rows(board) == false || check_columns(board) == false || 
                check_boxes(board) == false) 
                exit_code = 1;

        Pnmrdr_free(&pgm);
        fclose (file);
        UArray2_free(&board);
        exit(exit_code);
}

/* This will read in the provided FILE containing a solved sudoku puzzle 
and enter its pixel values in our array */
void read_in_sudoku(Pnmrdr_T pgm, Pnmrdr_mapdata pgmData, UArray2_T board)
{
        char *data_ptr;

        /*iterates through each row */
        for (int i = 0; i < (int)pgmData.height; i++) 
        {       
                /*iterates through each col */
                for (int j = 0; j < (int)pgmData.width; j++) 
                {
                        data_ptr = UArray2_at(board, j, i);
                        *data_ptr = Pnmrdr_get(pgm);
                }
        }

}

/* Checks every row to ensure there are no duplicates */
bool check_rows(UArray2_T board)
{
        bool check_duplicates[DIMEN];
        int *element;

        /* iterates through all rows and checks that they have no duplicates */
        for (int i = 0; i < DIMEN; i++) /* iterates through rows */
        {
                /* sets all elements in array to false before each row*/
                for (int i = 0; i < DIMEN; i++)
                        check_duplicates[i] = false;

                for (int j = 0; j < DIMEN; j++) /* iterates through columns */
                {
                        element = UArray2_at(board, j, i);
                        if (check_duplicates[*element - 1] == false)
                                check_duplicates[*element - 1] = true;
                        else 
                                return false;  /* duplicate detected */
                }
        }
        return true; /* no duplicates detected */

}

/* Checks every column to ensure there are no duplicates */
bool check_columns(UArray2_T board)
{
        bool check_duplicates[DIMEN];
        int *element;

        /* iterates through all rows and checks that they have no duplicates */
        for (int i = 0; i < DIMEN; i++) /* iterates through columns */
        {
                /* sets all elements in array to false before each col */
                for (int i = 0; i < DIMEN; i++)
                        check_duplicates[i] = false;

                for (int j = 0; j < DIMEN; j++) /* iterates through rows */
                {
                        element = UArray2_at(board, i, j);
                        if (check_duplicates[*element - 1] == false)
                                check_duplicates[*element - 1] = true;
                        else 
                                return false;  /* duplicate detected */
                }
        }
        return true;  /* no duplicates detected */
}

/* Checks every box to ensure there are no duplicates */
bool check_boxes(UArray2_T board)
{
        bool check_duplicates[DIMEN];
        int *element;

        for (int k = 0; k <= 6; k += 3) /* shifts row iterator to the right */
        {
                for (int n = 0; n <= 6; n += 3) /* shifts col iterator down */
                {
                        /* sets all elements in array to false */
                        for (int i = 0; i < DIMEN; i++)
                                check_duplicates[i] = false;

                        /* iterates through all rows and checks that they have
                        no duplicates */
                        for (int i = 0 + k; i < 3; i++) /* iterates through 
                                                                        rows */
                        {
                                for (int j = 0 + n; j < 3; j++) /* iterates 
                                                             through columns */
                                {
                                        element = UArray2_at(board, j, i);
                                        /* element has not been detected */
                                        if (check_duplicates[*element - 1]
                                                                     == false)
                                                check_duplicates[*element - 1]
                                                                        = true;
                                        /* element has already been detected */
                                        else 
                                                return false;  
                                                /* duplicate detected */
                                }
                        }
                }
        }
        return true;  /* no duplicates detected */
}











