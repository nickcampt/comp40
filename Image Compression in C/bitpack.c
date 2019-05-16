/*
 *      bitpack.c
 *      by Nick Camp and Darby Huye
 *      Assignment 4; 3/8/18
 *
 *
 *      The file Bitpack.c has used for our bitword creation and usage.
 *      We are able to check if a bit will fit into a word in a given width
 *      at a given LSB. We are also able to create a new word out of a given
 *      old word and a given new value.
 *
 */

#include <stdio.h>
#include "bitpack.h"
#include <assert.h>
#include "except.h"
#include <math.h>
#include <limits.h>

const int wordlength = 64;

Except_T Bitpack_Overflow = { "Overflow packing bits" };


/* checks if a given unsigned word can hold a given width value */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width > 0); //check this
        if(n > pow(2, width) - 1) return false;
        return true;
}

/* checks if a given signed word can hold a given width value */
bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width > 0);
        int shiftsize = wordlength - width;
        int64_t temp = n;
        temp = temp << shiftsize;
        temp = temp >> shiftsize;
        if(temp == n) return true;
        return false;

}

/* gets the value of a given width at a given LSB - FOR UNSIGNED*/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert((int)(width + lsb) <= wordlength);
        uint64_t mask = pow(2, width) - 1;
        uint64_t shiftedword = word >> lsb;
        return (shiftedword & mask);
}

/* gets the value of a given width at a given LSB - FOR SIGNED*/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert((int)(width + lsb) <= wordlength);

        uint64_t mask = (pow(2, width) - 1);
        mask = mask << lsb;
        uint64_t temp = word & mask;
        temp = temp >> lsb;
        int64_t sign = temp >> (width - 1);
        if(sign == 1) {
             int64_t sign_mask = UINT_MAX;
             return (sign_mask | temp);
        }
        return temp;

}

/* creates a new UNSIGNED bitword out of an old word and a new value, as well
as a target LSB and width for the new value */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                                                                uint64_t value)
{
        assert((int)width <= wordlength && (int)width >= 0);
        assert((int)(width + lsb) <= wordlength);
        if(!Bitpack_fitsu(value, width))
                RAISE(Bitpack_Overflow);

        uint64_t mask = pow(2, width) - 1;

        mask = mask << lsb;
        mask = ~mask;
        uint64_t temp = word & mask;
        value = value << lsb;
        return (temp + value);

}

/* creates a new SIGNED bitword out of an old word and a new value, as well
as a target LSB and width for the new value */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                                                                int64_t value)
{
        assert((int)width <= wordlength && (int)width >= 0);
        assert((int)(width + lsb) <= wordlength);
        if(!Bitpack_fitss(value, width))
                RAISE(Bitpack_Overflow);


        uint64_t mask = pow(2, width) - 1;
        uint64_t tempvalue = value;
        tempvalue = tempvalue << (wordlength - width) >>
                                                (wordlength - lsb - width);
        mask = mask << lsb;
        uint64_t tempword = word & ~mask;
        return tempvalue + tempword;
}
