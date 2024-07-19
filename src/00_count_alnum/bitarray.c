/*  bitarray.c
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

// Sources used:
// - https://stackoverflow.com/questions/47981/how-to-set-clear-and-toggle-a-single-bit
// - https://stackoverflow.com/questions/10134805/bitwise-rotate-left-function

#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitarray.h"

static inline size_t byte_index(size_t index) {
    return index >> 3;
}

static inline size_t bit_index(size_t index) {
    return ~index & 7;
}

static inline size_t round_up_to_next_mul8(size_t x) {
    return (x + (8 - 1)) / 8;
}

static inline size_t num_of_bits(size_t num_of_bytes) {
    return 8 * num_of_bytes;
}

bitarray_type* bitarray_create_w_bytes(size_t num_of_bytes) {
    if (num_of_bytes == 0 || num_of_bytes > SIZE_MAX / sizeof(uint8_t)) {
        return NULL;
    }
    bitarray_type* bitarray_ptr = malloc(offsetof(bitarray_type, bytes) + num_of_bytes);
    if (!bitarray_ptr) {
        return NULL;
    }
    memset(&bitarray_ptr->bytes[0], false, num_of_bytes);
    bitarray_ptr->num_of_bytes = num_of_bytes;

    return bitarray_ptr;
}

bitarray_type* bitarray_create_w_min_bits(size_t num_of_bits) {
    return bitarray_create_w_bytes(round_up_to_next_mul8(num_of_bits));
}

void bitarray_destroy(bitarray_type* bitarray_ptr) {
    assert(bitarray_ptr != NULL);

    free(bitarray_ptr);
}

void bitarray_print(const bitarray_type* bitarray_ptr) {
    assert(bitarray_ptr != NULL);

    size_t i;
    for (i = 0; i < 8 * bitarray_ptr->num_of_bytes; i++) {
        if (i % 8 == 0 && i % 32 != 0) {
            putchar(' ');
        }
        printf("%d", (bitarray_ptr->bytes[byte_index(i)] >> bit_index(i)) & 1);
        if ((i + 1) % 32 == 0) {
            putchar('\n');
        }
    }
    if ((i - 1) % 32 != 0) {
        putchar('\n');
    }
}

bool bitarray_at(const bitarray_type* bitarray_ptr, size_t index) {
    assert(bitarray_ptr != NULL);
    assert(index < num_of_bits(bitarray_ptr->num_of_bytes));

    size_t n = byte_index(index);
    size_t m = bit_index(index);

    return (bitarray_ptr->bytes[n] >> m) & 1;
}

void bitarray_set_true_at(bitarray_type* bitarray_ptr, size_t index) {
    assert(bitarray_ptr != NULL);
    assert(index < num_of_bits(bitarray_ptr->num_of_bytes));

    size_t n = byte_index(index);
    size_t m = bit_index(index);

    bitarray_ptr->bytes[n] |= 1 << m;
}

void bitarray_set_false_at(bitarray_type* bitarray_ptr, size_t index) {
    assert(bitarray_ptr != NULL);
    assert(index < num_of_bits(bitarray_ptr->num_of_bytes));

    size_t n = byte_index(index);
    size_t m = bit_index(index);

    bitarray_ptr->bytes[n] &= ~(1 << m);
}

void bitarray_set_at(bitarray_type* bitarray_ptr, size_t index, bool value) {
    assert(bitarray_ptr != NULL);
    assert(index < num_of_bits(bitarray_ptr->num_of_bytes));

    size_t n = byte_index(index);
    size_t m = bit_index(index);

    bitarray_ptr->bytes[n] &= ~(1 << m);
    bitarray_ptr->bytes[n] |= (value << m);
}

void bitarray_toggle_at(bitarray_type* bitarray_ptr, size_t index) {
    assert(bitarray_ptr != NULL);
    assert(index < num_of_bits(bitarray_ptr->num_of_bytes));

    size_t n = byte_index(index);
    size_t m = bit_index(index);

    bitarray_ptr->bytes[n] ^= 1 << m;
}
