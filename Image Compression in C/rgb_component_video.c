/*
 *      rgb_component_video.c
 *      by Nick Camp and Darby Huye
 *      Assignment 4; 3/8/2018
 *
 *
 *      This file contains functions used for compression from rgb to
 *      component video. The file also contains functions for decompression
 *      from component video to rbg. Compression takes a ppm image and
 *      decompression results in a ppm image. Compression results in a 2D
 *      array of structs containing information about each 2 by 2 pixel block
 *      and decompression takes a 2D array in that form.
 *
 *
 */

#include <stdio.h>
#include <rgb_component_video.h>
#include "assert.h"
#include <stdlib.h>
#include <math.h>

const double r_y = 0.299;
const double g_y = 0.587;
const double b_y = 0.114;
const double r_pb = -0.168736;
const double g_pb = -0.331264;
const double b_pb = 0.5;
const double r_pr = 0.5;
const double g_pr = -0.418688;
const double b_pr = -0.081312;

const double denom = 255;

/* FUNCTIONS FOR IMAGE COMPRESSION -> RGB TO COMPONENT VIDEO */

/* Compresses a ppm image to component video, returning a UArray2 of
    pixel_info_structs */
UArray2_T compress_to_cv(Pnm_ppm image)
{
        UArray2_T structs;
        if((image -> width) % 2 != 0)
            image -> width = (image -> width) - 1;
        if((image -> height) % 2 != 0)
            image -> height = (image -> height) - 1;
        structs = UArray2_new((image-> width)/2, (image->height)/2,
                                                    sizeof(pixel_info));
        populate_struct_array(image, structs);
        return structs;
}

/* creates a UArray2 of pixel info structs out of an input ppm image */
void populate_struct_array(Pnm_ppm image, UArray2_T structs)
{
        int row, col, ua2_row = 0, ua2_col= 0;
        for(row = 0; row < (int)(image -> height); row += 2) {
                ua2_col = 0;

                for(col = 0; col < (int)image -> width; col += 2){
                        pixel_info newStruct;
                        NEW(newStruct);
                        newStruct -> y1 = calc_y(image, col, row);
                        newStruct -> y2 = calc_y(image, col, row + 1);
                        newStruct -> y3 = calc_y(image, col + 1, row);
                        newStruct -> y4 = calc_y(image, col + 1, row + 1);
                        newStruct -> pb = average_p(calc_pb(image, col, row),
                                            calc_pb(image, col, row + 1),
                                            calc_pb(image, col + 1, row),
                                            calc_pb(image, col + 1, row + 1));
                        newStruct -> pr = average_p(calc_pr(image, col, row),
                                            calc_pr(image, col, row + 1),
                                            calc_pr(image, col + 1, row),
                                            calc_pr(image, col + 1, row + 1));
                        pixel_info * targetLocation = (UArray2_at(structs,
                                                           ua2_col, ua2_row));
                        *targetLocation = newStruct;
                        ua2_col++;
                }
                ua2_row++;
        }
}

/* calculates a y value out of a given pixel location */
float calc_y(Pnm_ppm image, int col, int row)
{
        Pnm_rgb rgb = (image -> methods -> at) (image -> pixels, col, row);
        double red = (rgb -> red)/denom;
        double green = (rgb -> green)/denom;
        double blue = (rgb -> blue)/denom;
        return (r_y * red + g_y * green + b_y * blue);
}

/* averages 4 given pb or pr values */
float average_p(double p1, double p2, double p3, double p4)
{
        return (p1 + p2 + p3 + p4)/4;
}

/* calculates the pb value of a given pixel location */
double calc_pb(Pnm_ppm image, int col, int row)
{
        Pnm_rgb rgb = (image -> methods -> at) (image -> pixels, col, row);
        double red = (rgb -> red)/denom;
        double green = (rgb -> green)/denom;
        double blue = (rgb -> blue)/denom;
        return (r_pb * red + g_pb * green + b_pb * blue);

}

/* calculates the pr value of a given pixel location */
double calc_pr(Pnm_ppm image, int col, int row)
{
        Pnm_rgb rgb = (image -> methods -> at) (image -> pixels, col, row);
        double red = (rgb -> red)/denom;
        double green = (rgb -> green)/denom;
        double blue = (rgb -> blue)/denom;
        return (r_pr * red + g_pr * green + b_pr * blue);
}


/* FUNCTIONS FOR DECOMPRESSION -> COMPONENT VIDEO TO RGB */

/* creates a ppm out of given pixel_info_structs */
Pnm_ppm decompress_structs(UArray2_T pixelInfoRay, A2Methods_T methods)
{
        int row, col, ray_row = 0, ray_col = 0;
        Pnm_ppm decompressed;
        NEW(decompressed);
        decompressed -> pixels =
                (methods->new)(UArray2_width(pixelInfoRay)* 2,
                             UArray2_height(pixelInfoRay) * 2,
                                         sizeof(struct Pnm_rgb));

        for(row = 0; row < UArray2_height(pixelInfoRay); row++) {
                ray_col = 0;

                for(col = 0; col < UArray2_width(pixelInfoRay); col++) {
                        /* compressedInfo gives us the y's, pb, and pr */
                        pixel_info * compressedInfo = UArray2_at(pixelInfoRay,
                                                                    col, row);

                        Pnm_rgb rgb1, rgb2, rgb3, rgb4;

                        rgb1 = calc_inverse_y((*compressedInfo) -> y1,
                                                (*compressedInfo) -> pb,
                                                (*compressedInfo) -> pr);

                        rgb2 = calc_inverse_y((*compressedInfo) -> y2,
                                                (*compressedInfo) -> pb,
                                                (*compressedInfo) -> pr);

                        rgb3 = calc_inverse_y((*compressedInfo) -> y3,
                                                (*compressedInfo) -> pb,
                                                (*compressedInfo) -> pr);

                        rgb4 = calc_inverse_y((*compressedInfo) -> y4,
                                                (*compressedInfo) -> pb,
                                                (*compressedInfo) -> pr);
                        free(*compressedInfo);

                        Pnm_rgb pixelLocation =
                                        methods -> at(decompressed -> pixels,
                                                    ray_col, ray_row);
                        *pixelLocation = *rgb1; /* top left of 2x2 block */

                        pixelLocation = methods -> at(decompressed -> pixels,
                                                    ray_col, ray_row + 1);
                        *pixelLocation = *rgb2; /* bottom left of 2x2 block */

                        pixelLocation = methods -> at(decompressed -> pixels,
                                                    ray_col + 1, ray_row);
                        *pixelLocation = *rgb3; /* top right of 2x2 block */

                        pixelLocation = methods -> at(decompressed -> pixels,
                                                    ray_col + 1, ray_row + 1);
                        *pixelLocation = *rgb4; /* bottom right of 2x3 block*/

                        free(rgb1);
                        free(rgb2);
                        free(rgb3);
                        free(rgb4);

                        ray_col += 2;

                }
                ray_row += 2;
        }
        decompressed -> width = UArray2_width(pixelInfoRay) * 2;
        decompressed -> height = UArray2_height(pixelInfoRay) * 2;
        decompressed -> methods = methods;
        decompressed -> denominator = denom;
        return decompressed;

}

/* creates a pnm_rgb out of a given y, pb, and pr value*/
Pnm_rgb calc_inverse_y(float y, float pb, float pr)
{
    Pnm_rgb rgb;
    NEW(rgb);
    double r, g, b;
    r = ((1.0 * y) + (1.402 * pr)) * denom;
    r = checkPixelValues(r);
    g = ((1.0 * y) - ((0.344136 * pb)) - (0.714136 * pr)) * denom;
    g = checkPixelValues(g);
    b = ((1.0 * y) + (1.772 * pb)) * denom;
    b = checkPixelValues(b);


    rgb -> red = r;
    rgb -> green = g;
    rgb -> blue = b;

    return rgb;
}

/* calls max 225 and min0 on a given pixel value */
double checkPixelValues(double num)
{
    num = max225(num);
    num = min0(num);
    return num;
}

/* ensures that a pixel value is never above 225 */
double max225(double num)
{
    if(num >= 255)
        return 255;
    return num;
}

/* ensures that a pixel value is never below 0 */
double min0(double num)
{
    if(num <= 0)
        return 0;
    return num;
}
