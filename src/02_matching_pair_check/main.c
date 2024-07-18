#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { DEFAULT_SYMBOL_ENUM, LBRACKET, LCURLY, LPARAN, RBRACKET, RCURLY, RPARAN } SYMBOL_ENUM;

static SYMBOL_ENUM encode_symbol(char c) {
    switch (c) {
    case '[':
        return LBRACKET;
    case '{':
        return LCURLY;
    case '(':
        return LPARAN;
    case ']':
        return RBRACKET;
    case '}':
        return RCURLY;
    case ')':
        return RPARAN;
    default:
        break;
    }
    return DEFAULT_SYMBOL_ENUM;
}

static char decode_symbol(SYMBOL_ENUM symb) {
    switch (symb) {
    case LBRACKET:
        return '[';
    case LCURLY:
        return '{';
    case LPARAN:
        return '(';
    case RBRACKET:
        return ']';
    case RCURLY:
        return '}';
    case RPARAN:
        return ')';
    case DEFAULT_SYMBOL_ENUM:
        return ' ';
    }
    return ' ';
}

static SYMBOL_ENUM matching_symbol(SYMBOL_ENUM symb) {
    switch (symb) {
    case LBRACKET:
        return RBRACKET;
    case LCURLY:
        return RCURLY;
    case LPARAN:
        return RPARAN;
    case RBRACKET:
        return LBRACKET;
    case RCURLY:
        return LCURLY;
    case RPARAN:
        return LPARAN;
    case DEFAULT_SYMBOL_ENUM:
        return DEFAULT_SYMBOL_ENUM;
    }
    return DEFAULT_SYMBOL_ENUM;
}

#define NAME symb_stack
#define VALUE_TYPE SYMBOL_ENUM
#include "fstack.h" // symb_stack_*, stack_for_each

int main(void) {
    puts("Input line:");

    char* str = NULL;
    size_t str_buffersize = 0;
    ssize_t n = getline(&str, &str_buffersize, stdin);
    if (n < 0) {
        return 1;
    }

    symb_stack_type* stack_p = symb_stack_create((size_t)n);
    if (!stack_p) {
        free(str);
        return 1;
    }

    bool no_errors = true;
    ssize_t i = 0;

    for (i = 0; i < n; i++) {
        if (!no_errors) {
            break;
        }

        switch (str[i]) {
        case '(':
        case '{':
        case '[':
            symb_stack_push(stack_p, encode_symbol(str[i]));
            break;
        case ')':
        case '}':
        case ']':
            if (symb_stack_is_empty(stack_p)) {
                no_errors = false;
                break;
            }
            no_errors = encode_symbol(str[i]) == matching_symbol(symb_stack_pop(stack_p));
        }
    }

    if (!no_errors) {
        if (i <= 8) {
            printf("%*c error at\n", (int)i, '^');
        } else {
            printf("%*s ^\n", (int)i - 2, "error at");
        }
    } else if (!symb_stack_is_empty(stack_p)) {
        printf("\nUnclosed %c.\n", decode_symbol(symb_stack_pop(stack_p)));
    } else {
        printf("\nno errors : )\n");
    }

    symb_stack_destroy(stack_p);
    free(str);

    return 0;
}
