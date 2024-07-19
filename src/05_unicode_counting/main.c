#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fnvhash.h"

#define NAME ucharht
#define KEY_TYPE char*
#define VALUE_TYPE unsigned int
#define KEY_IS_EQUAL(a, b) (strcmp((a), (b)) == 0)
#define HASH_FUNCTION(key) (fnvhash_32_str(key))
#include "fhashtable.h"

#include "arena.h"

size_t utf8_num_of_bytes(const char* s) {
    // https://en.wikipedia.org/wiki/UTF-8#Encoding

    switch (*s & 0b11111000) {
    case 0b11000000:
        return 2;
    case 0b11100000:
        return 3;
    case 0b11110000:
        return 4;
    }
    return 1;
}

int int_max(const int a, const int b) {
    return (a >= b) ? a : b;
}

struct line_info_type {
    char* buf;
    size_t capacity;
    ssize_t len;
};

#define lim (4096 * 4)

int main(void) {
    struct line_info_type line = {.buf = NULL};

    unsigned char* arena_buf = malloc(lim);
    arena_type arena;
    arena_init(&arena, lim, arena_buf);

    ucharht_type* ht_ptr = ucharht_create(lim);

    while (line.buf == NULL || !feof(stdin)) {

        line.len = getline(&line.buf, &line.capacity, stdin);
        if (line.len == -1) {
            if (!feof(stdin)) {
                fprintf(stderr, "%s\n", strerror(errno));
            }
            break;
        }

        for (size_t i = 0, n = 0; i < (size_t)line.len; i += int_max(1, n)) {
            if (line.buf[i] == '\0') {
                break;
            } else if (line.buf[i] == ' ' || line.buf[i] == '\n' || line.buf[i] == '\t') {
                continue;
            }

            n = utf8_num_of_bytes(&line.buf[i]);

            char* str = arena_allocate(&arena, n + 1);
            if (!str) {
                fprintf(stderr, "line too long. sorry.\n");
                goto reset;
            }
            memcpy(str, &line.buf[i], n);
            str[n] = '\0';

            if (!ucharht_contains_key(ht_ptr, str)) {
                if (ucharht_is_full(ht_ptr)) {
                    fprintf(stderr, "line too long. sorry.\n");
                    goto reset;
                }
                ucharht_insert(ht_ptr, str, 1);
            } else {
                (*ucharht_get_value_mut(ht_ptr, str))++;
            }
        }

        {
            char* key;
            size_t value;
            size_t tmpi;
            fhashtable_for_each(ht_ptr, tmpi, key, value) {
                if (key[1] != '\0') {
                    printf("%s: %zu\n", key, value);
                }
            }
        }

    reset:
        arena_deallocate_all(&arena);
        ucharht_clear(ht_ptr);
    }

    free(line.buf);
    free(arena_buf);
    ucharht_destroy(ht_ptr);
}
