#if __has_include("string.h")
    #include "string.h"
#else 
    #error "Error: string.h not found"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef _WIN32
    #define NEWLINE "\r\n"
#else
    #define NEWLINE "\n"
#endif

#define INIT_STRING_CAP 256
#define INIT_SPLIT_CAP  8

#define __assert(expression, format, ...)                                                                 \
    do {                                                                                                  \
        if (!(expression)) {                                                                              \
            fprintf(stderr, "Error in file %s; line %d in function %s:\n", __FILE__, __LINE__, __func__); \
            fprintf(stderr, format"\n" __VA_OPT__(,) __VA_ARGS__);                                        \
            exit(EXIT_FAILURE);                                                                           \
        }                                                                                                 \
    } while(0)

void _append_str_internal_char(String *str, const char *append) {
    size_t len = strlen(append);

    if(str->capacity - str->len < len) {
        str->capacity = len + str->len;
        char *new  = realloc(str->arr, str->capacity);
        __assert(new!=NULL, "Not enough memory append string.");
        str->arr = new;
    }
    
    memcpy(&str->arr[str->len], append, len);
    str->len += len;
}

void _append_str_internal_string(String *str, const String *append) {
    if(str->capacity - str->len < append->len) {
        str->capacity = append->len + str->len;
        char *new  = realloc(str->arr, str->capacity);
        __assert(new!=NULL, "Not enough memory append string.");
        str->arr = new;
    }
    
    memcpy(&str->arr[str->len], append->arr, append->len);
    str->len += append->len;
}

void _report_append_error_internal(String *str, ...) {
    (void)str;
    __assert(false, "Cannot append an invalid data type.");
}

String string_new(const char *str)
{
    String string;
    
    string.len = strlen(str);
    string.capacity = INIT_STRING_CAP;

    while (string.capacity < string.len) string.capacity *= 2;
    string.arr = (char *)malloc(string.capacity);
    __assert(string.arr!=NULL, "Not enough memory to create new string.");

    memcpy(string.arr, str, string.len);

    return string;
}

String string_with_len(const char *str, size_t len) {
    String string;
    
    string.len = len;
    string.capacity = INIT_STRING_CAP;

    while (string.capacity < string.len) string.capacity *= 2;
    string.arr = (char *)malloc(string.capacity);
    __assert(string.arr!=NULL, "Not enough memory to create new string.");

    memcpy(string.arr, str, string.len);

    return string;
}

void string_print(String *str) {
    for(size_t i=0; i < str->len; i++) {
        printf("%c", str->arr[i]);
    }
}

void string_println(String *str) {
    string_print(str);
    printf("\n");
}

void string_print_debug(String *str) {
    printf("String { len: %zu, capacity: %zu, arr: \"", str->len, str->capacity);
    string_print(str);
    printf("\" }");
}

void string_println_debug(String *str) {
    string_print_debug(str);
    printf("\n");
}

char string_at(String *str, int64_t index) {
    __assert(index>=0, "Accessing at index %lld is not allowed.", index);
    __assert((size_t)index<str->len, "Accessing at index %lld, but the length is %zd.", index, str->len);
    return str->arr[index];
}

void string_push(String *str, char character) {
    if(str->len==str->capacity) {
        str->capacity *= 2;
        char *new  = realloc(str->arr, str->capacity);
        __assert(new!=NULL, "Not enough memory to insert %c.", character);
        str->arr = new;
    }
    str->arr[str->len++] = character;
}

StringSplit string_split(String *str, const char *delimeter) {
    StringSplit spl = split_new();
    size_t del_len = strlen(delimeter);
    size_t current = 0, track = 0;

    while (current <= str->len - del_len) {
        if(memcmp(&str->arr[current], delimeter, del_len) == 0) {
            split_push(&spl, string_with_len(&str->arr[track], current - track));
            current += del_len;
            track = current;
        }
        else
            current++;
    }
    if (track <= str->len) {
        split_push(&spl, string_with_len(&str->arr[track], str->len - track));
    }
    
    return spl;
}

void string_free(String *str) {
    free(str->arr);
    str->len = 0;
    str->capacity = INIT_STRING_CAP;
    str->arr = NULL;
}

bool string_equals(String *str1, String *str2) {
    if (str1->len != str2->len) return false;
    return memcmp(str1->arr, str2->arr, str1->len) == 0;
}

void string_map(String *str, void (*func)(String *str)) {
    func(str);
}

void string_change(String *str, int64_t index, char chr) {
    __assert(index>=0, "Accessing at index %lld is not allowed.", index);
    __assert((size_t)index<str->len, "Accessing at index %lld, but the length is %zd.", index, str->len);
    str->arr[index] = chr;
}

void string_to_upper(String *str) {
    for(size_t i=0; i<str->len; i++) {
        str->arr[i] = (char)toupper(str->arr[i]);
    }
}

void string_to_lower(String *str) {
    for(size_t i=0; i<str->len; i++) {
        str->arr[i] = (char)tolower(str->arr[i]);
    }
}

String string_input() {
    String buff = String("");
    char ch;

    while ((ch = (char)getchar()) != '\n' && ch != EOF) {
        string_push(&buff, ch);
    }
    return buff;
}

bool string_is_empty(String *str) {
    return str->len==0;
}

void string_trim_right(String *str) {
    if(str->len != 0)
        while (str->len>0 && isspace(str->arr[str->len-1]))
            str->len--;
}

void string_trim_left(String *str) {
    if(str->len != 0) {
        size_t start = 0;
        while (start<str->len && isspace(str->arr[start]))
            start++;
        
        if(start>0) {
            str->len -= start;
            memmove(str->arr, &str->arr[start], str->len);
        }
    }
}

void string_trim(String *str) {
    string_trim_left(str);
    string_trim_right(str);
}

String string_read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    __assert(file!=NULL, "Could not open %s", filename);

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    String buff;
    buff.len = size;
    buff.capacity = size;
    buff.arr = (char *)malloc(buff.capacity);
    __assert(buff.arr!=NULL, "Not enough memory to create new string.");

    fread(buff.arr, 1, size, file);

    fclose(file);
    return buff;
}

StringSplit string_lines(String *str) {
    return string_split(str, NEWLINE);
}

String string_clone(String *str) {
    String clone;
    clone.capacity = str->capacity;
    clone.len = str->len;
    clone.arr = malloc(str->len);

    memcpy(clone.arr, str->arr, str->len);
    return clone;
}

StringSplit split_new() {
    String *arr = malloc(sizeof(String)*INIT_SPLIT_CAP);
    __assert(arr!=NULL, "Not enough memory to create new split.");
    StringSplit split = {0, INIT_SPLIT_CAP, arr};

    return split;
}

void split_push(StringSplit *split, String data) {
    if(split->len==split->capacity) {
        split->capacity *= 2;
        String *new = realloc(split->arr, sizeof(String)*split->capacity);
        __assert(new!=NULL, "Not enough memory to insert string.");
        split->arr = new;
    }
    split->arr[split->len++] = data;
}

String split_pop(StringSplit *split) {
    __assert(split->len>0, "Cannot pop, split underflow.");
    return split->arr[--split->len];
}

void split_print(StringSplit *split) {
    printf("[");
    for(size_t i=0; i<split->len; i++) {
        if(i!=split->len-1) {
            printf("\"");
            string_print(&split->arr[i]);
            printf("\", ");
        }
        else {
            printf("\"");
            string_print(&split->arr[i]);
            printf("\"");
        }
    }
    printf("]");
}

void split_println(StringSplit *split) {
    split_print(split);
    printf("\n");
}

void split_print_debug(StringSplit *split) {
    printf("StringSplit { len: %zu, capacity: %zu, arr: ", split->len, split->capacity);
    split_print(split);
    printf(" }");
}

void split_println_debug(StringSplit *split) {
    split_print_debug(split);
    printf("\n");
}

String split_at(StringSplit *split, int64_t index) {
    __assert(index>=0, "Accessing at index %lld is not allowed.", index);
    __assert((size_t)index<split->len, "Accessing at index %lld, but the length is %zd.", index, split->len);
    return split->arr[index];
}

void split_free(StringSplit *split) {
    for(size_t i=0; i<split->len; i++) {
        free(split->arr[i].arr);
    }
    split->len = 0;
    split->capacity = INIT_SPLIT_CAP;
    free(split->arr);
    split->arr = NULL;
}

char *format(const char *fmt, ...) {
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