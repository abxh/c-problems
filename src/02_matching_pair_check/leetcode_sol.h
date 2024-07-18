#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline char matching_symbol(char symb) {
    switch (symb) {
    case '[':
        return ']';
    case '{':
        return '}';
    case '(':
        return ')';
    };
    return ' ';
}

typedef struct {
    size_t count;
    size_t capacity;
    char values[];
} symb_stack_type;

static inline symb_stack_type* symb_stack_create(const size_t capacity) {
    symb_stack_type* stack_ptr = (symb_stack_type*)malloc(offsetof(symb_stack_type, values) + capacity * sizeof(char));

    stack_ptr->count = 0;
    stack_ptr->capacity = capacity;

    return stack_ptr;
}

static inline void symb_stack_destroy(symb_stack_type* stack_ptr) {
    free(stack_ptr);
}

static inline bool symb_stack_is_empty(const symb_stack_type* stack_ptr) {
    return stack_ptr->count == 0;
}

static inline void symb_stack_push(symb_stack_type* stack_ptr, const char value) {
    stack_ptr->values[stack_ptr->count++] = value;
}

static inline char symb_stack_pop(symb_stack_type* stack_ptr) {
    return stack_ptr->values[--stack_ptr->count];
}

bool isValid(char* s) {
    size_t n = strlen(s);
    symb_stack_type* stack_p = symb_stack_create((size_t)n);

    bool no_errors = true;
    int64_t i = 0;

    for (i = 0; i < n; i++) {
        if (!no_errors) {
            break;
        }

        switch (s[i]) {
        case '(':
        case '{':
        case '[':
            symb_stack_push(stack_p, s[i]);
            break;
        case ')':
        case '}':
        case ']':
            if (symb_stack_is_empty(stack_p)) {
                no_errors = false;
                break;
            }
            no_errors = s[i] == matching_symbol(symb_stack_pop(stack_p));
        }
    }

    return no_errors && symb_stack_is_empty(stack_p);
}
