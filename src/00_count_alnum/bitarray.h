/*  bitarray.h
 *
 *  Copyright (C) 2023 abfish
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

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct bitarray_type {
    size_t num_of_bytes;
    uint8_t bytes[];
} bitarray_type;

bitarray_type* bitarray_create_w_bytes(size_t num_of_bytes);

bitarray_type* bitarray_create_w_min_bits(size_t num_of_bits);

void bitarray_destroy(bitarray_type* bitarray_p);

void bitarray_print(const bitarray_type* bitarray_p);

bool bitarray_at(const bitarray_type* bitarray_p, size_t index);

void bitarray_toggle_at(bitarray_type* bitarray_p, size_t index);

void bitarray_set_true_at(bitarray_type* bitarray_p, size_t index);

void bitarray_set_false_at(bitarray_type* bitarray_p, size_t index);

void bitarray_set_at(bitarray_type* bitarray_p, size_t index, bool bit_value);

void bitarray_rotate(bitarray_type* bitarray_p, int shift);
