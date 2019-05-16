/* Ethan Sorkin and Nick Camp
* Comp40
* HW2, iii
* unblackedges.c: this program takes in a valid pbm image, with pixel values
* of either 0 or 1, and removes all 'black edge pixels' as defined in the spec.
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "list.h"
#include "seq.h"
#include "str.h"
#include "pnmrdr.h"
#include "bit2.h"

#define BLACK 1
#define WHITE 0


void read_in_bitmap(Pnmrdr_T pbm, Pnmrdr_mapdata pbmData, Bit2_T map);

void check_black(int row, int col, Bit2_T map);

void check_edges(int rowMax, int colMax, Bit2_T map); 

void pbmwrite(Bit2_T map);

int main (int argc, char *argv[])
{
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

        Pnmrdr_T pbm;
        assert (pbm = Pnmrdr_new(file)); /* ensuring file is a PNM */
        Pnmrdr_mapdata pbmData = Pnmrdr_data(pbm);
        assert(pbmData.type == 1); /* ensuring file is a PBM */

        Bit2_T map = Bit2_new(pbmData.width, pbmData.height);

        read_in_bitmap(pbm, pbmData, map);
        check_edges(Bit2_height(map), Bit2_width(map), map);

        pbmwrite(map);


        Pnmrdr_free(&pbm);
        fclose (file);
        Bit2_free(&map);
        return 0;
}

/* This will read in the provided FILE to our Bit2_T, containing a PBM with 
pixel values of either 1 or 0 */
void read_in_bitmap(Pnmrdr_T pbm, Pnmrdr_mapdata pbmData, Bit2_T map)
{
        int val;
        for (int row = 0; row < (int)pbmData.height; row++)
        {
                for (int col = 0; col < (int)pbmData.width; col++)
                {
                        val = Pnmrdr_get(pbm);
                        Bit2_put(map, col, row, val);
                }
        }

}
/* goes to every black edge starting point and traces their black edgedness */
void check_edges(int rowMax, int colMax, Bit2_T map)
{
        for (int row = 0; row < rowMax; row++) 
        {
                check_black(row, 0, map); /* checking top row */ 
                check_black(row, colMax - 1, map); /* checking bottom row */
        }
        for (int col = 0; col < colMax; col++)
        {
                check_black(0, col, map); /* checking leftmost column */
                check_black(rowMax - 1, col, map); /* checking right column */
        }
}

/* checks if a pixel is black, and if so it 1. changes to white then 2. goes to
its neighbor pixels and performs the same checks / switches */
void check_black(int row, int col, Bit2_T map)
{
        if (Bit2_get(map, col, row) == BLACK) /*convert black pixel to white */
                Bit2_put(map, col, row, WHITE); 
        else 
                return;

       
        if (row != 0)  /* check up */
                check_black(row - 1, col, map);
        
        if (col != Bit2_width(map) - 1) /* check right */
                check_black(row, col + 1, map);

        if (row != Bit2_height(map) - 1) /* check down */
                check_black(row + 1, col, map);

        if (col != 0) /* check left */
                check_black(row, col - 1, map);
        
}

/* converts bit vector to a valid PBM image format */
void pbmwrite(Bit2_T map)
{
        printf("P1\n%d %d\n", Bit2_width(map), Bit2_height(map));
        for (int i = 0; i < Bit2_height(map); i++)
        {
                for (int j = 0;  j < Bit2_width(map); j++)
                {
                        printf("%d", Bit2_get(map, j, i));
                }
                printf("\n");
        }   
}
