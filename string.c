#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define INIT_STRING_CAP 256
#define INIT_VEC_CAP    8

#define __assert(expression, format, ...) \
    do { \
        if (!(expression)) { \
            fprintf(stderr, "Error in file %s; line %d in function %s:\n", __FILE__, __LINE__, __func__); \
            fprintf(stderr, format "\n", ##__VA_ARGS__); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

String string_new(const char *str) {
    String string;
    
    string.len = strlen(str);
    string.capacity = INIT_STRING_CAP;

    string.print = print;
    string.print_debug = print_debug;
    string.at = at;
    string.push = push;
    string.split = split;
    string.free = string_free;
    string.equals = equals;
    string.map = map;
    string.change = change;
    string.to_upper = to_upper;
    string.to_lower = to_lower;

    while (string.capacity < string.len) string.capacity *= 2;
    string.arr = (char *)malloc(string.capacity);
    __assert(string.arr!=NULL, "%s", "Not enough memory to create new string.");

    memcpy(string.arr, str, string.len);

    return string;
}

void print(String *str) {
    for(size_t i=0; i < str->len; i++) {
        printf("%c", str->arr[i]);
    }
}

void print_debug(String *str) {
    printf("String { len: %zu, capacity: %zu, arr: \"", str->len, str->capacity);
    str->print(str);
    printf("\" }");
}

char at(String *str, int64_t index) {
    __assert(index>=0, "Accessing at index %lld is not allowed.", index);
    __assert((size_t)index<str->len, "Accessing at index %lld, but the length is %zd.", index, str->len);
    return str->arr[index];
}

void push(String *str, char character) {
    if(str->len==str->capacity) {
        str->capacity *= 2;
        char *new  = realloc(str->arr, str->capacity);
        __assert(new!=NULL, "Not enough memory to insert %c.", character);
        str->arr = new;
    }
    str->arr[str->len++] = character;
}

Vec split(String *str, const char *delimeter) {
    Vec vec = vec_new();
    size_t del_len = strlen(delimeter);
    size_t current = 0;

    char *string = malloc(str->len + 1);
    __assert(string!=NULL, "%s", "Not enough memory.");
    memcpy(string, str->arr, str->len);
    string[str->len] = '\0';

    char *str_ptr = strstr(string, delimeter);
    while(str_ptr) {
        size_t index = str_ptr - &string[current];

        char *slice = malloc(index + 1);
        __assert(slice!=NULL, "%s", "Not enough memory.");
        memcpy(slice, &string[current], index);
        slice[index] = '\0';
        
        String *new_slice = malloc(sizeof(String));
        __assert(new_slice!=NULL, "%s", "Not enough memory.");
        *new_slice = string_new(slice);

        vec.push(&vec, new_slice);
        free(slice);

        current += index+del_len;
        str_ptr = strstr(&string[current], delimeter);
    }
    if (current < str->len) {
        String *new_slice = malloc(sizeof(String));
        __assert(new_slice!=NULL, "%s", "Not enough memory.");
        *new_slice = string_new(&string[current]);

        vec.push(&vec, new_slice);
    }

    free(string);
    return vec;
}

void string_free(String *str) {
    free(str->arr);
    str->len = 0;
    str->capacity = INIT_STRING_CAP;
    str->arr = NULL;
}

bool equals(String *str1, String *str2) {
    if (str1->len != str2->len) return false;
    return memcmp(str1->arr, str2->arr, str1->len) == 0;
}

void map(String *str, void (*func)(String *str)) {
    func(str);
}

void change(String *str, int64_t index, char chr) {
    __assert(index>=0, "Accessing at index %lld is not allowed.", index);
    __assert((size_t)index<str->len, "Accessing at index %lld, but the length is %zd.", index, str->len);
    str->arr[index] = chr;
}

void to_upper(String *str) {
    for(size_t i=0; i<str->len; i++) {
        str->arr[i] = (char)toupper(str->arr[i]);
    }
}

void to_lower(String *str) {
    for(size_t i=0; i<str->len; i++) {
        str->arr[i] = (char)tolower(str->arr[i]);
    }
}

Vec vec_new() {
    String **arr = malloc(sizeof(String*)*INIT_VEC_CAP);
    __assert(arr!=NULL, "%s", "Not enough memory to create new vector.");
    Vec vec = {0, INIT_VEC_CAP, arr};

    vec.free = vec_free;
    vec.push = vec_push;
    vec.pop = vec_pop;
    vec.at = vec_at;
    vec.print = vec_print;
    vec.print_debug = vec_print_debug;

    return vec;
}

void vec_push(Vec *vec, String *data) {
    if(vec->len==vec->capacity) {
        vec->capacity *= 2;
        String **new = realloc(vec->arr, sizeof(String*)*vec->capacity);
        __assert(new!=NULL, "%s", "Not enough memory to insert string.");
        vec->arr = new;
    }
    vec->arr[vec->len++] = data;
}

String *vec_pop(Vec *vec) {
    __assert(vec->len>0, "%s", "Cannot pop, vector underflow.");
    return vec->arr[--vec->len];
}

void vec_print(Vec *vec) {
    printf("[");
    for(size_t i=0; i<vec->len; i++) {
        if(i!=vec->len-1) {
            printf("\"");
            vec->arr[i]->print(vec->arr[i]);
            printf("\", ");
        }
        else {
            printf("\"");
            vec->arr[i]->print(vec->arr[i]);
            printf("\"");
        }
    }
    printf("]");
}

void vec_print_debug(Vec *vec) {
    printf("Vec { len: %zu, capacity: %zu, arr: ", vec->len, vec->capacity);
    vec->print(vec);
    printf(" }");
}

String *vec_at(Vec *vec, int64_t index) {
    __assert(index>=0, "Accessing at index %lld is not allowed.", index);
    __assert((size_t)index<vec->len, "Accessing at index %lld, but the length is %zd.", index, vec->len);
    return vec->arr[index];
}

void vec_free(Vec *vec) {
    for(size_t i=0; i<vec->len; i++) {
        free(vec->arr[i]->arr);
        free(vec->arr[i]);
    }
    vec->len = 0;
    vec->capacity = INIT_VEC_CAP;
    free(vec->arr);
    vec->arr = NULL;
}

const char *format(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args) + 1;
    va_end(args);

    char *str = malloc(len);
    if (!str) return NULL;

    va_start(args, fmt);
    vsnprintf(str, len, fmt, args);
    va_end(args);

    return str;  
}