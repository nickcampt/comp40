#include <pnm.h>
#include "rgb_component_video.h"
#include <stdlib.h>
#include "a2methods.h"
#include "a2plain.h"
#include "assert.h"
#include "dct.h"
#include <math.h>
#include "mem.h"



#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

void print_words(UArray_T ray);
void print_pix(Pnm_ppm image);

int main(int argc, char * argv[]) {


    (void) argc;

    A2Methods_T methods = uarray2_methods_plain;

 
    A2Methods_mapfun *map = methods->map_default; 
    assert(map);
    SET_METHODS(uarray2_methods_plain, map_row_major, "row-major");

    FILE * fp = fopen(argv[1], "rb");
    Pnm_ppm image = Pnm_ppmread(fp, methods);
    int width = image -> width;
    int height = image -> height;
    if((image -> width) % 2 != 0)
        image -> width = width -1;
    if((image -> height) % 2 != 0)
        image -> height = height - 1;
    print_pix(image);
    UArray2_T structs = compress_to_cv(image);

    UArray_T compressed = compressdct(structs);
    //print_words(compressed);
    fprintf(stderr, "printing the first word\n");
     //word = *(uint64_t *)UArray_at(ray, 0);



    //now to decompress:
    fprintf(stderr, "DECOMPRESSING NOW \n\n");

    UArray2_T partiallydec = decompressdct(compressed, width/2, height/2);
    fprintf(stderr, "after partial\n");
    fprintf(stderr, "y4: %f\n", (*(pixel_info *)UArray2_at(partiallydec, 0, 0)) -> y4);
    Pnm_ppm decompressed = decompress_structs(partiallydec, methods);
    fprintf(stderr, "decompressed: \n");
    print_pix(decompressed);
    Pnm_ppmwrite(stdout, decompressed);


/*
    int b,c,d;
    unsigned a, pb,pr;
    a = 311;
    b = 14;
    c = 12;
    d = 3;
    pb = 5;
    pr = 10; 
    fprintf(stderr, "y1: %f\n", calc_Y1(a,b,c,d));
    fprintf(stderr, "y2: %f\n", calc_Y2(a,b,c,d));
    fprintf(stderr, "y3: %f\n", calc_Y3(a,b,c,d));
    fprintf(stderr, "y4: %f\n", calc_Y4(a,b,c,d));
    fprintf(stderr, "pb: %f\n", dequantize_pbpr(pb));
    fprintf(stderr, "pr: %f\n", dequantize_pbpr(pr));


    UArray2_T test = UArray2_new(1, 1, sizeof(pixel_info));
    pixel_info thisstruct;
    NEW(thisstruct);
    thisstruct -> y1 = calc_Y1(a,b,c,d);
    thisstruct -> y2 = calc_Y2(a,b,c,d);
    thisstruct -> y3 = calc_Y3(a,b,c,d);
    thisstruct -> y4 = calc_Y4(a,b,c,d);
    thisstruct -> pb = dequantize_pbpr(pb);
    thisstruct -> pr = dequantize_pbpr(pr);

    *(pixel_info*)UArray2_at(test, 0, 0) = thisstruct;


    UArray_T compressed = compressdct(test);
    print(compressed);
    
    if(compressed == NULL) assert(0);
    fprintf(stderr, "num: %u\n", *(unsigned*)UArray_at(compressed, 0));*/
    //fprintf(stderr, "ceil of -0.29999 %f\n", ceil((0.299999)* 50));
/*
    int num = quantize_bcd(-.1224);
    (void) num;
    fprintf(stderr, "pb: %d\n", quantize_pbpr(.1222));
    fprintf(stderr, "bcd: %d\n", quantize_bcd(.1222));
    fprintf(stderr, "ceil: %f\n", ceil(0.0000001 * 50));
    fprintf(stderr, "floor: %f\n", floor(-0.0000001 * 50));*/
   // (void) image;
   // UArray2_T partiallycompressed = compress_to_cv(image); 

}

void print_words(UArray_T ray)
{
    uint64_t un;
    int64_t sig;
    uint64_t word;
    for(int i = 0; i < UArray_length(ray); i++)
    {
        word = *(uint64_t *)UArray_at(ray, i);
        fprintf(stderr, "word: %u\n", (unsigned)word);
        un = Bitpack_getu(word, 9, 23);
        fprintf(stderr, "a: %u\n", (unsigned)un);

        sig = Bitpack_gets(word, 5, 18);
        fprintf(stderr, "b: %d\n", (int)sig);
        sig = Bitpack_gets(word, 5, 13);
        fprintf(stderr, "c: %d\n", (int)sig);
        sig = Bitpack_gets(word, 5, 8);
        fprintf(stderr, "d: %d\n", (int)sig);

        un = Bitpack_getu(word, 4, 4);
        fprintf(stderr, "pb: %u\n", (unsigned)un);
        un = Bitpack_getu(word, 4, 0);
        fprintf(stderr, "pr: %u\n", (unsigned)un);
    }
}
void print_pix(Pnm_ppm image)
{
    for(int row = 0; row < UArray2_height(image->pixels); row++) {
            for(int col = 0; col < UArray2_width(image->pixels); col++) {
                Pnm_rgb rgb = (image -> methods -> at) (image -> pixels, col, row);
                
                unsigned red = (rgb)->red;
                unsigned blue = (rgb)->blue;
                unsigned green = (rgb)->green;
                fprintf(stderr, "(%d, %d): r:%u, g:%u, b:%u\n", row, col, red, green, blue);
                
            }
    }
}
