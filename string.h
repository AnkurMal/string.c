#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// credit: <https://stackoverflow.com/a/400970>
#define foreach(item, var) \
    for(size_t keep = 1, count = 0; keep && count != var.len; keep = !keep, count++) \
        for(typeof(var.arr[0]) item = var.arr[count]; keep; keep = !keep)

#define String(str) string_new(str)

typedef struct String {
    size_t len;
    size_t capacity;
    char   *arr;
} String;

typedef struct Vec {
    size_t len;
    size_t capacity;
    String *arr;
} Vec;

String string_new(const char *str);
void   string_print(String *str);
void   string_println(String *str);
void   string_print_debug(String *str);
void   string_println_debug(String *str);
char   string_at(String *str, int64_t index);
void   string_push(String *str, char character);
Vec    string_split(String *str, const char *delimeter); 
void   string_free(String *str);
bool   string_equals(String *str1, String *str2);
void   string_map(String *str, void (*func)(String *str));
void   string_change(String *str, int64_t index, char chr);
void   string_to_upper(String *str);
void   string_to_lower(String *str);

Vec    vec_new();
void   vec_free(Vec *vec);
void   vec_push(Vec *vec, String data);
String vec_pop(Vec *vec);
String vec_at(Vec *vec, int64_t index);
void   vec_print(Vec *vec);
void   vec_println(Vec *vec);
void   vec_print_debug(Vec *vec);
void   vec_println_debug(Vec *vec);

char   *format(const char *fmt, ...);