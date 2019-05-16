/*
 *      rgb_component_video.h
 *      by Nick Camp and Darby Huye
 *      Assignment 4; 3/8/2018
 *
 *      Function declarations for rbg_component_video.c
 *
 *
 */

#ifndef RGB_COMPONENT_VIDEO
#define RGB_COMPONENT_VIDEO

#include <stdio.h>
#include <pnm.h>
#include "uarray2.h"
#include "mem.h"


typedef struct pixel_info_struct *pixel_info;

struct pixel_info_struct {
    float y1, y2, y3, y4, pb, pr;
};

/*              COMPRESSION             */

/* Compresses a ppm image to component video, returning a UArray2 of
    pixel_info_structs */
UArray2_T compress_to_cv(Pnm_ppm image);

/* creates a UArray2 of pixel info structs out of an input ppm image */
void populate_struct_array(Pnm_ppm image, UArray2_T structs);

/* calculates a y value out of a given pixel location */
float calc_y(Pnm_ppm image, int col, int row);

/* averages 4 given pb or pr values */
float average_p(double p1, double p2, double p3, double p4);

/* calculates the pb value of a given pixel location */
double calc_pb(Pnm_ppm image, int col, int row);

/* calculates the pr value of a given pixel location */
double calc_pr(Pnm_ppm image, int col, int row);

/*              DECOMPRESSION           */

/* creates a ppm out of given pixel_info_structs */
Pnm_ppm decompress_structs(UArray2_T ray, A2Methods_T methods);

/* creates a pnm_rgb out of a given y, pb, and pr value*/
Pnm_rgb calc_inverse_y(float y, float pb, float pr);

/* calls max 225 and min0 on a given pixel value */
double checkPixelValues(double num);

/* ensures that a pixel value is never above 225 */
double max225(double num);

/* ensures that a pixel value is never below 0 */
double min0(double num);

#endif
