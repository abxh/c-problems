/*  rotate_bits.c
 *
 *  Copyright (C) 2023 abxh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *  USA
 */

// sources:
// https://stackoverflow.com/questions/10134805/bitwise-rotate-left-function
// https://en.wikipedia.org/wiki/Circular_shift

#include <assert.h>
#include <limits.h>
#include <stdint.h>

#include "rotate_bits.h"

static_assert(CHAR_BIT == 8, "a char is 8 bits");

#define rotl(type, value, count)                               \
    if ((count) == 0 || (count) == ((int)sizeof(value)) * 8) { \
        return (value);                                        \
    }                                                          \
    return (type)(((value) << (count)) | ((value) >> (((int)sizeof(value)) * 8 - (count))));

#define rotr(type, value, count)                               \
    if ((count) == 0 || (count) == ((int)sizeof(value)) * 8) { \
        return (value);                                        \
    }                                                          \
    return (type)(((value) << (((int)sizeof(value)) * 8 - (count))) | (value) >> (count));

uint8_t rotate_bits_8(uint8_t value, int shift) {
    if (shift >= 0) {
        shift = shift % ((int)sizeof(value));
        rotr(uint8_t, value, shift)
    } else {
        shift = (-shift) % ((int)sizeof(value));
        rotl(uint8_t, value, shift)
    }
}

uint16_t rotate_bits_16(uint16_t value, int shift) {
    if (shift >= 0) {
        shift = shift % ((int)sizeof(value));
        rotr(uint16_t, value, shift)
    } else {
        shift = (-shift) % ((int)sizeof(value));
        rotl(uint16_t, value, shift)
    }
}

uint32_t rotate_bits_32(uint32_t value, int shift) {
    if (shift >= 0) {
        shift = shift % ((int)sizeof(value));
        rotr(uint32_t, value, shift)
    } else {
        shift = (-shift) % ((int)sizeof(value));
        rotl(uint32_t, value, shift)
    }
}

uint64_t rotate_bits_64(uint64_t value, int shift) {
    if (shift >= 0) {
        shift = shift % ((int)sizeof(value));
        rotr(uint64_t, value, shift)
    } else {
        shift = (-shift) % ((int)sizeof(value));
        rotl(uint64_t, value, shift)
    }
}
