#pragma once

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

void histogram_seq(const size_t k, uint64_t* bins, const size_t n, const int64_t* is);

void histogram_par_mutex(const size_t k, uint64_t* bins, const size_t n, const int64_t* is);

#ifdef __cplusplus
}
#endif
