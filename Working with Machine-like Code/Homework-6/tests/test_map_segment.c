#include "segment.h"
#include <stdio.h>
#include <stdlib.h>

void map_multiple_segments();
uint32_t registers[3] = {3, 4 ,5, 6, 7, 8, 9, 10};

int main()
{
        printf("Testing map_segment for correct segment creation.\n");
        printf("Expected output is: 123456789\n");
        printf("Programs output is: ");
        test_segment_creation();
        
        exit(EXIT_SUCCESS);
}


//Tests the storage of segment index in register B. 
//The expected values are 1,2,3,4,5,6,7,8,9,10
//Also tests if the segments are being created in order.
void map_multiple_segments()
{
        for (int i=0; i<10; ++i)
        {
                map_segment(2, 4, registers[]);
                printf(register[4]);                                        
        }
}
