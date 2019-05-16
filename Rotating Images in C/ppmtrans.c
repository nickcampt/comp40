/*
 * Nick Camp and Ethan Sorkin
 * ppmtrans.c
 * Homework 3 - Locality
 *
 * ppmtrans.c: this takes in command line arguments and performs an image
 * transofrmation, then printing the result to standard output,
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <string.h>
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

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

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

Pnm_ppm rotate_image(Pnm_ppm image, A2Methods_mapfun *map, int rotation, 
                                        char *time_file_name, char *filename);
                                        
Pnm_ppm flip_image(Pnm_ppm image, A2Methods_mapfun *map, char *flip, 
                                        char *time_file_name, char *filename);

void rotate90(int col, int row, A2Methods_UArray2 ray2, void *elem, void *cl);

void rotate180(int col, int row, A2Methods_UArray2 ray2, void *elem, void *cl);

void rotate270(int col, int row, A2Methods_UArray2 ray2, void *elem, void *cl);

void flip_h(int col, int row, A2Methods_UArray2 original, void *elem, 
                                                             void *destination);

void flip_v(int col, int row, A2Methods_UArray2 original, void *elem, 
                                                             void *destination);


int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL;
        int   rotation       = 0;
        int   i;
        FILE *file;
        char *filename;
        char *flip = NULL;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                    "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                } else if (strcmp(argv[i], "-flip") == 0) {
                        if(!(i + 1 < argc)) {     /* no flip value */
                                usage(argv[0]);
                        }
                        flip = argv[++i];
                        if (!(strcmp(flip, "vertical") == 0 || 
                                         (strcmp(flip, "horizontal") == 0))) {
                                fprintf(stderr, 
                                     "Flip must be vertical or horizontal\n");
                                usage(argv[0]);
                        }
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                argv[i]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }

        if (i == argc - 1)  {      /* last argument is a filename */   
                filename = argv[i];
                file = fopen(filename, "rb");
                assert (file != NULL);
        }
        else  {      /* user will enter pixel values through stdin */   
                file = stdin;
                assert (file != NULL);
        }

        /* converts file to valid Pnm_ppm */
        Pnm_ppm image = Pnm_ppmread(file, methods); 
   
        if (rotation != 0)  {    /* some type of rotation will occur */
                Pnm_ppm output_image = rotate_image(image, map, rotation, 
                                                    time_file_name, filename);
                Pnm_ppmwrite(stdout, output_image);
                Pnm_ppmfree(&output_image);            
        }
        
        else if (flip != NULL)  {
                Pnm_ppm output_image = flip_image(image, map, flip, 
                                                    time_file_name, filename);
                Pnm_ppmwrite(stdout, output_image);
                Pnm_ppmfree(&output_image);
        }

        else  {  /* for 0 degree rotation, we just print the original image */
                if (time_file_name != NULL) {   /* if the user enters -time */
                        CPUTime_T timer = CPUTime_New();
                        FILE *timing_file = fopen(time_file_name, "a");
                        double time_elapsed = 0;
                        
                        CPUTime_Start(timer); /* starting timer */
                        time_elapsed = CPUTime_Stop(timer);/* stopping timer */
                        
                        int pixel_count = image->width * image->height;
                        fprintf(timing_file, "Image: %s\n", filename);
                        fprintf(timing_file, "Number of pixels = %d\n", 
                                                                 pixel_count);
                        fprintf(timing_file, "Rotation: %d degrees\n", 
                                                                    rotation);
                        fprintf(timing_file, "Total time: %f nanoseconds\n", 
                                                                time_elapsed);
                        fprintf(timing_file, "Time per pixel = %f nanoseconds",
                                                (time_elapsed / pixel_count));
                        fprintf(timing_file, "\n\n");
                        
                        fclose(timing_file);
                        CPUTime_Free(&timer);
                }
                
                Pnm_ppmwrite(stdout, image);
        }

        Pnm_ppmfree(&image);
        fclose (file);        
        return EXIT_SUCCESS;
}

/* this function performs a rotation on an image (either 90, 180, or 270), and
returns the rotated image */
Pnm_ppm rotate_image(Pnm_ppm image, A2Methods_mapfun *map, int rotation, 
                                        char *time_file_name, char *image_file)
{
        CPUTime_T timer = CPUTime_New();
        double time_elapsed = 0;

        Pnm_ppm final_image = malloc(sizeof(*final_image));
        final_image->denominator = image->denominator;
        final_image->methods = image->methods;
        

        if (rotation == 90)     {
                final_image->width = image->height;
                final_image->height = image->width;
                final_image->pixels = image->methods->new(final_image->width, 
                    final_image->height, image->methods->size(image->pixels));
                              
                CPUTime_Start(timer); /* starting timer */
                map(image->pixels, rotate90, final_image);
                time_elapsed = CPUTime_Stop(timer); /* stopping timer */
        }
    
        else if (rotation == 180)       {
                final_image->width = image->width;
                final_image->height = image->height;
                final_image->pixels = image->methods->new(final_image->width, 
                    final_image->height, image->methods->size(image->pixels));

                CPUTime_Start(timer); /* starting timer */
                map(image->pixels, rotate180, final_image);
                time_elapsed = CPUTime_Stop(timer); /* stopping timer */
        }

        else if (rotation == 270)       {
                final_image->width = image->height;
                final_image->height = image->width;
                final_image->pixels = image->methods->new(final_image->width, 
                    final_image->height, image->methods->size(image->pixels));

                CPUTime_Start(timer); /* starting timer */
                map(image->pixels, rotate270, final_image);
                time_elapsed = CPUTime_Stop(timer); /* stopping timer */
        }

        if (time_file_name != NULL)    {    /* if the user enters -time */
                FILE *timing_file = fopen(time_file_name, "a");
                int pixel_count = final_image->width * final_image->height;
                fprintf(timing_file, "Image: %s\n", image_file);
                fprintf(timing_file, "Number of pixels = %d\n", pixel_count);
                fprintf(timing_file, "Rotation: %d degrees\n", rotation);
                fprintf(timing_file, "Total time: %f nanoseconds\n", 
                                                                time_elapsed);
                fprintf(timing_file, "Time per pixel = %f nanoseconds\n\n", 
                                                (time_elapsed / pixel_count));
                fclose(timing_file);
        }
        
        CPUTime_Free(&timer);
        return final_image;
}

/* this function performs a flip on an image (vertical or horizontal), and
returns the rotated image */
Pnm_ppm flip_image(Pnm_ppm image, A2Methods_mapfun *map, char *flip, 
                                        char *time_file_name, char *image_file)
{
        CPUTime_T timer = CPUTime_New();
        double time_elapsed = 0;

        Pnm_ppm final_image = malloc(sizeof(*final_image));
        final_image->denominator = image->denominator;
        final_image->methods = image->methods;
        

        if (strcmp(flip, "horizontal") == 0)     {
                final_image->width = image->width;
                final_image->height = image->height;
                final_image->pixels = image->methods->new(final_image->width, 
                    final_image->height, image->methods->size(image->pixels));
                              
                CPUTime_Start(timer); /* starting timer */
                map(image->pixels, flip_h, final_image);
                time_elapsed = CPUTime_Stop(timer); /* stopping timer */
        }
    
        else if (strcmp(flip, "vertical") == 0)       {
                final_image->width = image->width;
                final_image->height = image->height;
                final_image->pixels = image->methods->new(final_image->width, 
                    final_image->height, image->methods->size(image->pixels));

                CPUTime_Start(timer); /* starting timer */
                map(image->pixels, flip_v, final_image);
                time_elapsed = CPUTime_Stop(timer); /* stopping timer */
        }


        if (time_file_name != NULL)    {    /* if the user enters -time */
                FILE *timing_file = fopen(time_file_name, "a");
                int pixel_count = final_image->width * final_image->height;
                fprintf(timing_file, "Image: %s\n", image_file);
                fprintf(timing_file, "Number of pixels = %d\n", pixel_count);
                fprintf(timing_file, "Flip: %s\n", flip);
                fprintf(timing_file, "Total time: %f nanoseconds\n", 
                                                                time_elapsed);
                fprintf(timing_file, "Time per pixel = %f nanoseconds\n\n", 
                                                (time_elapsed / pixel_count));
                fclose(timing_file);
        }
        
        CPUTime_Free(&timer);
        return final_image;
}

/* performs a 90 degree rotation on an image and returns the rotated image */
void rotate90(int col, int row, A2Methods_UArray2 original, void *elem, 
                                                             void *destination)
{
        Pnm_ppm destination_ptr = destination;
        int new_col = destination_ptr->methods->height(original) - row - 1;
        int new_row = col;

        /* getting pointer to address where we want to put data */
        Pnm_rgb target = destination_ptr->methods->at(destination_ptr->pixels, 
                                                            new_col, new_row);
        Pnm_rgb elem_ptr = elem;
        *target = *elem_ptr;

} 

/* performs a 180 degree rotation on an image and returns the rotated image */
void rotate180(int col, int row, A2Methods_UArray2 original, void *elem, 
                                                             void *destination)
{
        Pnm_ppm destination_ptr = destination;
        int new_col = destination_ptr->methods->width(original) - col - 1;
        int new_row = destination_ptr->methods->height(original) - row - 1;

         /* getting pointer to address where we want to put data */
        Pnm_rgb target = destination_ptr->methods->at(destination_ptr->pixels, 
                                                             new_col, new_row);
        Pnm_rgb elem_ptr = elem;
        *target = *elem_ptr;

} 

/* performs a 270 degree rotation on an image and returns the rotated image */
void rotate270(int col, int row, A2Methods_UArray2 original, void *elem, 
                                                             void *destination)
{
        Pnm_ppm destination_ptr = destination;
        int new_col = row;
        int new_row = destination_ptr->methods->width(original) - col - 1;

         /* getting pointer to address where we want to put data */
        Pnm_rgb target = destination_ptr->methods->at(destination_ptr->pixels, 
                                                             new_col, new_row);

        Pnm_rgb elem_ptr = elem;
        *target = *elem_ptr;

}

void flip_h(int col, int row, A2Methods_UArray2 original, void *elem, 
                                                             void *destination)
{
        Pnm_ppm destination_ptr = destination;
        int new_col = destination_ptr->methods->width(original) - col - 1;
        int new_row = row;

         /* getting pointer to address where we want to put data */
        Pnm_rgb target = destination_ptr->methods->at(destination_ptr->pixels, 
                                                             new_col, new_row);
        Pnm_rgb elem_ptr = elem;
        *target = *elem_ptr;

}
void flip_v(int col, int row, A2Methods_UArray2 original, void *elem, 
                                                             void *destination)
{
        Pnm_ppm destination_ptr = destination;
        int new_col = col;
        int new_row = destination_ptr->methods->height(original) - row - 1;

         /* getting pointer to address where we want to put data */
        Pnm_rgb target = destination_ptr->methods->at(destination_ptr->pixels, 
                                                             new_col, new_row);
        Pnm_rgb elem_ptr = elem;
        *target = *elem_ptr;
}



