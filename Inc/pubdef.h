#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <iso646.h>


#define USE_GRAPHVIZ 1


#define BUFSIZE 1024
#define TYPESIGNAL(X) _Generic((X),\
    int: "%d", float : "%f", double: "%f", char: "%c", char*: "%s", default: "%d")
#define PRINT_VAR(var) do {                                                     \
        char __print_buf[BUFSIZE], __format_buf[BUFSIZE] = "val of " #var ": ";   \
        sprintf(__print_buf, strcat(__format_buf, TYPESIGNAL(var)), var);         \
        puts(__print_buf);                                                        \
} while(0)



#define __VA_ARGS_N(_9, _8, _7, _6, _5, _4, _3, _2, _1, _0, N, ...) N
#define VA_ARGS_N(...) __VA_ARGS_N(0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define __range_1(var, n) (size_t var = 0; var < n; var++)
#define __range_2(var, a, b) (size_t var = a; var < b; var++)
#define __range_3(var, start, stop, step) (int var = start; var < stop; var += step)
#define __range_n_cat(a, b) a ## b
#define __range_n(n) __range_n_cat(__range_, n)
#define __range(...) __range_n(VA_ARGS_N(__VA_ARGS__))
#define range(var, ...)  __range(__VA_ARGS__)(var, __VA_ARGS__)

#define ever (;;)


#define error(msg)              \
    do {                        \
        fputs(msg, stderr);     \
        exit(1);                \
    } while (0)


#define check_null(expr)                                            \
    do {                                                            \
        if ((expr) == NULL) {                                       \
            fprintf(stderr, "Find NULL expr in file \"%s\" line %d",\
                __FILE__, __LINE__);                                \
            exit(1);                                                \
        }                                                           \
    } while (0)

#define printp(ptr) printf("%p", ptr)

#define endl putchar('\n')


#define new(Type) Type##_New
#define delete(Type) Type##_Delete

#define MemAllEqual (0)

typedef void* Any;

