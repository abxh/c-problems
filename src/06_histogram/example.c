#include <stdlib.h>
#include <stdio.h>

#include "histogram.h"

int main(void) {
    const int64_t is[] = {0, 1, 3, 2, 1, 0, 0, 1};

    size_t k = 3;
    uint64_t* bins = malloc(k * sizeof(uint64_t));

    histogram_par_mutex(k, bins, sizeof(is) / sizeof(*is), is);

    printf("[");
    for (size_t i = 0; i < k; i++) {
        printf("%zu", bins[i]);
        if (i != k - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    free(bins);
}
