#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h> // time, timespec (linux), nanosleep (linux)

#ifdef _WIN32
#include <windows.h> // Sleep
#endif

#include "rotate_bits.h" // rotate_bits_8

#define VT_ESC "\033"
#define VT_MOVUP VT_ESC "[A"
#define VT_CLEARLINE VT_ESC "[2K"

#define VT_MOVTOFRONT "\r"

static void init_star_pattern(uint64_t star_pattern[8]) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8 * sizeof(*star_pattern); j++) {
            uint64_t s = (rand() & 1) && (rand() & 1) && (rand() & 1) && (rand() & 1);
            star_pattern[i] |= s << j;
        }
    }
}

int main(void) {
    uint64_t star_pattern[8] = {0};
    init_star_pattern(star_pattern);

    int16_t rotation_dir = 0;

    while (true) {
        rotation_dir += 128; // we overflow deliberately
        for (size_t i = 0; i < 8; i++) {
            star_pattern[i] = rotate_bits_64(star_pattern[i], (rotation_dir >= 0) - (rotation_dir <= 0));
        }
        for (size_t i = 0; i < 8; i++) {
            for (size_t j = 0; j < 8 * sizeof(*star_pattern); j++) {
                printf("%c", ((star_pattern[i] >> j) & 1) ? '*' : ' ');
            }
            putchar('\n');
        }

#ifdef linux
        nanosleep((const struct timespec[]){{0, 1 / 25. * 1e+9}}, NULL);
#endif
#ifdef _WIN32
        Sleep(1000 / 25);
#endif

        for (int i = 0; i < 8; i++) {
            printf(VT_CLEARLINE);
            printf(VT_MOVUP);
        }
        printf(VT_MOVTOFRONT);
    }
}
