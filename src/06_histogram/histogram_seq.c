#include "histogram.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void histogram_seq(const size_t k, uint64_t* bins, const size_t n, const int64_t* is) {
    for (size_t i = 0; i < k; i++) {
        bins[i] = 0;
    }
    for (size_t i = 0; i < n; i++) {
        const int64_t index = is[i];
        if (0 <= index && index < (int64_t)MIN(k, INT64_MAX)) {
            const uint64_t x_old = bins[index];
            const uint64_t x_new = x_old + 1;
            bins[index] = x_new;
        }
    }
}
