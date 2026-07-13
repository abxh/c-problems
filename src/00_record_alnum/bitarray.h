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
