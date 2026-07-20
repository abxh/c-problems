#include "histogram.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void histogram_seq(const size_t num_bins, uint64_t* bins, const size_t num_indices, const int64_t* indices) {
    for (size_t i = 0; i < num_bins; i++) {
        bins[i] = 0;
    }
    for (size_t i = 0; i < num_indices; i++) {
        const int64_t bin_index = indices[i];
        if (0 <= bin_index && bin_index < (int64_t)MIN(num_bins, INT64_MAX)) {
            const uint64_t x_old = bins[bin_index];
            const uint64_t x_new = x_old + 1;
            bins[bin_index] = x_new;
        }
    }
}
