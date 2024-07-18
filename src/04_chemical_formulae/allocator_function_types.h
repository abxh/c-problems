/*  allocator_function_types.h
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

#include <stdlib.h> // size_t

typedef void* (*allocate_f)(void*, size_t, size_t); // allocator_struct_p, alignment, size

typedef void* (*reallocate_f)(void*, void*, size_t, size_t, size_t); // allocator_struct_p, ptr, alignment, old_size, new_size

typedef void (*deallocate_f)(void*, void*); // allocator_struct_p, ptr
