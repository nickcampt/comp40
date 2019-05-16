#include "segment.h"
#include <stdio.h>
#include <stdlib.h>

void map_multiple_segments();
uint32_t registers[3] = {3, 4 ,5, 6, 7, 8, 9, 10};


int main()
{
        printf("Testing unmap_segment.\n");
        printf("Expected output is: 3456789\n");
        printf("Programs output is: ");
        map_multiple_segments();
        unmap_multiple_segments();
        print_removed_segments();
        
        
        exit(EXIT_SUCCESS);
}

//Maps 10 segments
void map_multiple_segments()
{
        for (int i=0; i<10; ++i)
        {
                map_segment(2, 4, registers[]);
                printf(register[4]);                                        
        }
}


//Unmaps all the segments.
//Indices of unmapped segments are stored in the removed_segments sequence.
void unmap_multiple_segments()
{
        for(int i=3; i<10; ++i)
                unmap_segment(i);
}