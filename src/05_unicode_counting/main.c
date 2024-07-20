#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fnvhash.h"

#define NAME ucharht
#define KEY_TYPE uint64_t
#define VALUE_TYPE unsigned int
#define KEY_IS_EQUAL(a, b) ((a) == (b))
#define HASH_FUNCTION(key) (fnvhash_32((uint8_t*)&key, sizeof(uint64_t)))
#include "fhashtable.h"

#define NAME ucharque
#define VALUE_TYPE char*
#include "fqueue.h"

#include "arena.h"

size_t utf8_num_of_bytes(const unsigned char* s) {
    // https://en.wikipedia.org/wiki/UTF-8#Encoding

    if ((*s >> 7) == 0b0) {
        return 1;
    } else if ((*s >> 5) == 0b110) {
        return 2;
    } else if ((*s >> 4) == 0b1110) {
        return 3;
    } else if ((*s >> 3) == 0b11110) {
        return 4;
    }
    assert(false);
}

uint32_t utf8_get_differing_bits(const unsigned char* s) {
    if (s[0] >> 7 == 0) {
        return (uint32_t)s[0];
    }
    uint32_t retval;
    int n;
    if ((*s >> 5) == 0b110) {
        retval = (s[0] & 0b00011111) << 6;
        n = 2;
    } else if ((*s >> 4) == 0b1110) {
        retval = (s[0] & 0b00001111) << 12;
        n = 3;
    } else if ((*s >> 3) == 0b11110) {
        retval = (s[0] & 0b00000111) << 18;
        n = 4;
    } else {
        assert(false);
    }
    for (int i = 1; i < n; i++) {
        assert((s[i] >> 6) == 0b10);

        retval |= (s[i] & 0b00111111) << ((n - 1 - i) * 6);
    }
    return retval;
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
    ucharque_type* que_ptr = ucharque_create(lim);

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

            n = utf8_num_of_bytes((unsigned char*)&line.buf[i]);

            char* str = arena_allocate(&arena, n + 1);
            if (!str) {
                fprintf(stderr, "line too long. sorry.\n");
                goto reset;
            }
            memcpy(str, &line.buf[i], n);
            str[n] = '\0';

            uint32_t differing_bits_utf8 = utf8_get_differing_bits((unsigned char*)&line.buf[i]);

            if (!ucharht_contains_key(ht_ptr, differing_bits_utf8)) {
                if (ucharht_is_full(ht_ptr)) {
                    fprintf(stderr, "line too long. sorry.\n");
                    goto reset;
                }
                ucharht_insert(ht_ptr, differing_bits_utf8, 1);
                ucharque_enqueue(que_ptr, str);
            } else {
                (*ucharht_get_value_mut(ht_ptr, differing_bits_utf8))++;
            }
        }

        {
            char* key;
            size_t tmpi;
            fqueue_for_each(que_ptr, tmpi, key) {
                uint32_t ht_key = utf8_get_differing_bits((unsigned char*)key);
                printf("%s (U+%X): %d\n", key, ht_key,  ucharht_get_value(ht_ptr, ht_key, -1));
            }
        }

    reset:
        arena_deallocate_all(&arena);
        ucharht_clear(ht_ptr);
        ucharque_clear(que_ptr);
    }

    free(line.buf);
    free(arena_buf);
    ucharht_destroy(ht_ptr);
    ucharque_destroy(que_ptr);
}
