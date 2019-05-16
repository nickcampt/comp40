/* Ethan Sorkin and Nick Camp
* COMP-40
* HW2, iii
* bit2.h: interface for our Bit2_T representation
*/

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED
#define T Bit2_T
typedef struct T *T;


T Bit2_new(int height, int width);

void Bit2_free(T *ray);

int Bit2_height(T ray);

int Bit2_width(T ray);

int Bit2_count (T ray);

int Bit2_get(T ray, int col, int row);

int Bit2_put(T ray, int col, int row, int val);

void Bit2_map_row_major(T ray, void apply (int col, int row,
                            T bitvec, int element, void *unused), void *cl);

void Bit2_map_col_major(T ray, void apply (int col, int row,
                            T bitvec, int element, void *unused), void *cl);

#undef Bit2_T
#endif
