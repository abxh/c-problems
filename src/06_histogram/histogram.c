#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void histogram_seq(const size_t k, size_t* bins, const size_t n, const int* is) {
    for (size_t i = 0; i < k; i++) {
        bins[i] = 0;
    }
    for (size_t i = 0; i < n; i++) {
        const int index = is[i];
        if (0 <= index && index < k) {
            const size_t x_old = bins[index];
            const size_t x_new = x_old + 1;
            bins[index] = x_new;
        }
    }
}

int main(void) {
    const int is[] = {0, 1, 3, 2, 1, 0, 0, 1};

    size_t k = 3;
    size_t* bins = malloc(3 * sizeof(size_t));

    histogram_seq(k, bins, sizeof(is) / sizeof(*is), is);

    printf("[");
    for (size_t i = 0; i < k; i++) {
        printf("%zu", bins[i]);
        if (i != k - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
