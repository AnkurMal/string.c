#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define String(str) string_new(str);

typedef struct String {
    size_t len;
    size_t capacity;
    char   *arr;

    void        (*print)(struct String *self);
    void        (*print_debug)(struct String *self);
    char        (*at)(struct String *self, int64_t index);
    void        (*push)(struct String *self, char character);
    struct Vec  (*split)(struct String *self, const char *delimeter); 
    void        (*free)(struct String *self);
    bool        (*equals)(struct String *self, struct String *cmp);
    void        (*map)(struct String *self, void (*func)(struct String *self));
    void        (*change)(struct String *self, int64_t index, char chr);
    void        (*to_upper)(struct String *self);
    void        (*to_lower)(struct String *self);
} String;

typedef struct Vec {
    size_t len;
    size_t capacity;
    String **arr;

    void    (*free)(struct Vec *self);
    void    (*push)(struct Vec *self, String *data);
    String* (*pop)(struct Vec *self);
    String* (*at)(struct Vec *self, int64_t index);
    void    (*print)(struct Vec *self);
    void    (*print_debug)(struct Vec *self);
} Vec;

String string_new(const char *str);
void   print(String *str);
void   print_debug(String *str);
char   at(String *str, int64_t index);
void   push(String *str, char character);
Vec    split(String *str, const char *delimeter); 
void   string_free(String *str);
bool   equals(String *str1, String *str2);
void   map(String *str, void (*func)(String *str));
void   change(String *str, int64_t index, char chr);
void   to_upper(String *str);
void   to_lower(String *str);

Vec    vec_new();
void   vec_free(Vec *vec);
void   vec_push(Vec *vec, String *data);
String *vec_pop(Vec *vec);
String *vec_at(Vec *vec, int64_t index);
void   vec_print(Vec *vec);
void   vec_print_debug(Vec *vec);

const char *format(const char *fmt, ...);