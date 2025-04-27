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
#include <errno.h>

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

#define parse_macro_for_long(type, type_str, func_name)                                                   \
    __assert(str->len>0, "Cannot parse an empty string.");                                                \
    char *cstr = string_to_cstring(str);                                                                  \
    char *endptr = NULL;                                                                                  \
    errno = 0;                                                                                            \
    type res = func_name(cstr, &endptr, 10);                                                              \
    __assert(errno!=ERANGE, "Parsed value out of range for "type_str".");                                 \
    __assert(*endptr=='\0', "Cannot parse an invalid digit.");                                            \
    free(cstr);                                                                                           \
    return res;                                                                           
                              
#define parse_macro_for_double(type, type_str, func_name)                                                 \
    __assert(str->len>0, "Cannot parse an empty string.");                                                \
    char *cstr = string_to_cstring(str);                                                                  \
    char *endptr = NULL;                                                                                  \
    errno = 0;                                                                                            \
    type res = func_name(cstr, &endptr);                                                                  \
    __assert(errno!=ERANGE, "Parsed value out of range for "type_str".");                                 \
    __assert(*endptr=='\0', "Cannot parse an invalid digit.");                                            \
    free(cstr);                                                                                           \
    return res;                                             

int64_t _report_error_internal(String *str, ...) {
    (void)str;
    __assert(false, "An invalid data type was passed.");
}

void _report_error_internal_parse(String *str) {
    (void)str;
    __assert(false, "An invalid data type was passed.");
}

void _append_str_internal_char(String *str, const char *append)
{
    size_t len = strlen(append);

    if(str->capacity - str->len < len) {
        str->capacity = len + str->len;
        char *new  = realloc(str->arr, str->capacity);
        __assert(new!=NULL, "Not enough memory to append string.");
        str->arr = new;
    }
    
    memcpy(&str->arr[str->len], append, len);
    str->len += len;
}

void _append_str_internal_string(String *str, const String *append) {
    if(str->capacity - str->len < append->len) {
        str->capacity = append->len + str->len;
        char *new  = realloc(str->arr, str->capacity);
        __assert(new!=NULL, "Not enough memory to append string.");
        str->arr = new;
    }
    
    memcpy(&str->arr[str->len], append->arr, append->len);
    str->len += append->len;
}

void _insert_str_internal_char(String *str, const char *insert, int64_t index) {
    __assert(index>=0, "Inserting at index %lld is not allowed.", index);
    __assert((size_t)index<=str->len, "Inserting at index %lld, but the length is %zd.", index, str->len);

    size_t ins_len = strlen(insert);
    if (ins_len == 0) return; 

    if (str->len + ins_len > str->capacity) {
        size_t new_cap = (str->len + ins_len) * 2;
        str->arr = realloc(str->arr, new_cap);
        __assert(str->arr != NULL, "Not enough memory to insert string.");
        str->capacity = new_cap;
    }

    memmove(&str->arr[index + ins_len], &str->arr[index], str->len - index);
    memcpy(&str->arr[index], insert, ins_len);

    str->len += ins_len;
}

void _insert_str_internal_string(String *str, const String *insert, int64_t index) {
    __assert(index>=0, "Inserting at index %lld is not allowed.", index);
    __assert((size_t)index<=str->len, "Inserting at index %lld, but the length is %zd.", index, str->len);

    if (insert->len == 0) return; 

    if (str->len + insert->len > str->capacity) {
        size_t new_cap = (str->len + insert->len) * 2;
        str->arr = realloc(str->arr, new_cap);
        __assert(str->arr != NULL, "Not enough memory to insert string.");
        str->capacity = new_cap;
    }

    memmove(&str->arr[index + insert->len], &str->arr[index], str->len - index);
    memcpy(&str->arr[index], insert, insert->len);

    str->len += insert->len;
}

long _parse_str_internal_long(String *str) {
    parse_macro_for_long(long, "long", strtol);
}

long long _parse_str_internal_long_long(String *str) {
    parse_macro_for_long(long long, "long long", strtoll);
}

unsigned long _parse_str_internal_unsigned_long(String *str) {
    parse_macro_for_long(unsigned long, "unsigned long", strtoul);
}

unsigned long long _parse_str_internal_unsigned_long_long(String *str) {
    parse_macro_for_long(unsigned long long, "unsigned long long", strtoull);
}

double _parse_str_internal_double(String *str) {
    parse_macro_for_double(double, "double", strtod);
}

long double _parse_str_internal_long_double(String *str) {
    parse_macro_for_double(long double, "long double", strtold);
}

float _parse_str_internal_float(String *str) {
    parse_macro_for_double(float, "float", strtof);
}

int64_t _find_str_internal_char(String *str, const char *pat) {
    size_t len = strlen(pat);
    if(len==0) return 0;

    for(size_t i=0; i <= str->len-len; i++) {
        if(memcmp(&str->arr[i], pat, len) == 0) return i;
    }
    return -1;
}

int64_t _find_str_internal_string(String *str, const String *pat) {
    if(pat->len==0) return 0;
    for(size_t i=0; i <= str->len-pat->len; i++) {
        if(memcmp(&str->arr[i], pat->arr, pat->len) == 0) return i;
    }
    return -1;
}

void _remove_str_internal_char(String *str, const char *pat) {
    int64_t index = _find_str_internal_char(str, pat);
    size_t len = strlen(pat);

    if(index!=-1) {
        memmove(&str->arr[index], &str->arr[index+len], str->len - (index+len));
        str->len -= len;
    }
}

void _remove_str_internal_string(String *str, const String *pat) {
    int64_t index = _find_str_internal_string(str, pat);

    if(index!=-1) {
        memmove(&str->arr[index], &str->arr[index+pat->len], str->len - (index+pat->len));
        str->len -= pat->len;
    }
}

bool _equal_str_internal_char(String *str1, const char *str2) {
    size_t len = strlen(str2);
    if (str1->len != len) return false;
    return memcmp(str1->arr, str2, str1->len) == 0;
}

bool _equal_str_internal_string(String *str1, const String *str2) {
    if (str1->len != str2->len) return false;
    return memcmp(str1->arr, str2->arr, str1->len) == 0;
}

void _remove_spl_internal_char(StringSplit *split, const char *pat) {
    size_t current = 0;

    while (current < split->len) {
        if(_equal_str_internal_char(&split->arr[current], pat)) {
            for(size_t i=current; i<split->len-1; i++) {
                split->arr[i] = split->arr[i+1];
            }
            split->len--;
        }
        else
            current++;
    }
}

void _remove_spl_internal_string(StringSplit *split, const String *pat) {
    size_t current = 0;

    while (current < split->len) {
        if(_equal_str_internal_string(&split->arr[current], pat)) {
            for(size_t i=current; i<split->len-1; i++) {
                split->arr[i] = split->arr[i+1];
            }
            split->len--;
        }
        else
            current++;
    }
}

bool _contains_spl_internal_char(StringSplit *split, const char *str) {
    for(size_t i=0; i<split->len; i++)
        if(_equal_str_internal_char(&split->arr[i], str)) return true;
    return false;
}

bool _contains_spl_internal_string(StringSplit *split, const String *str) {
    for(size_t i=0; i<split->len; i++)
        if(_equal_str_internal_string(&split->arr[i], str)) return true;
    return false;
}

String string_new(const char *str) {
    String string;
    
    string.len = strlen(str);
    string.capacity = INIT_STRING_CAP;

    while (string.capacity < string.len) string.capacity *= 2;
    string.arr = (char *)malloc(string.capacity);
    __assert(string.arr!=NULL, "Not enough memory to create new string.");

    memcpy(string.arr, str, string.len);

    return string;
}

char *string_to_cstring(String *str) {
    char *ptr = malloc(str->len+1);
    __assert(ptr!=NULL, "Not enough memory to create new cstring.");
    memcpy(ptr, str->arr, str->len);
    ptr[str->len] = '\0';

    return ptr;
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
    
    if (del_len == 0) {
        split_push(&spl, string_with_len("", 0)); 
        for (size_t i = 0; i < str->len; i++) {
            split_push(&spl, string_with_len(&str->arr[i], 1));
        }
        split_push(&spl, string_with_len("", 0)); 
        return spl;
    }
    else {
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
        if (track <= str->len) 
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

    size_t read = fread(buff.arr, 1, size, file);
    __assert(read == size, "Failed to read the whole file.");

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
    clone.arr = malloc(str->capacity);

    memcpy(clone.arr, str->arr, str->len);
    return clone;
}

void string_clear(String *str) {
    str->len = 0;
}

bool string_contains(String *str, char ch) {
    return memchr(str->arr, ch, str->len) != NULL;
}

int string_compare(String *str1, String *str2) {
    size_t min_len = str1->len < str2->len ? str1->len : str2->len;
    int cmp = memcmp(str1->arr, str2->arr, min_len);

    if (cmp < 0) return -1;
    if (cmp > 0) return 1;

    if (str1->len < str2->len) return -1;
    if (str1->len > str2->len) return 1;

    return 0;
}

void string_reverse(String *str) {
    for(size_t i=0; i<str->len/2; i++) {
        char temp = str->arr[i];
        str->arr[i] = str->arr[str->len-i-1];
        str->arr[str->len-i-1] = temp;
    }
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