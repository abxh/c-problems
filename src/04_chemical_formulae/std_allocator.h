/*  std_allocator.h
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

/*
    wrapper for std functions to conform to allocator function types
*/

#pragma once

#include <stdlib.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static inline void* std_allocate(void* allocator_struct_p, size_t alignment, size_t size) {
    return malloc(size);
}

static inline void* std_reallocate(void* allocator_struct_p, void* ptr, size_t alignment, size_t old_size, size_t new_size) {
    return realloc(ptr, new_size);
}

static inline void std_deallocate(void* allocator_struct_p, void* ptr) {
    free(ptr);
}

#pragma GCC diagnostic pop
