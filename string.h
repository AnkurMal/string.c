#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// credit: <https://stackoverflow.com/a/400970>
#define foreach(item, var)                                                             \
    for(size_t keep = 1, count = 0; keep && count != (var).len; keep = !keep, count++) \
        for(typeof((var).arr[0]) *(item) = &(var).arr[count]; keep; keep = !keep)

#define String(str) string_new(str)

#define string_append(str_ptr, append_ptr) _Generic((append_ptr),                      \
    const char*:        _append_str_internal_char,                                     \
    char*:              _append_str_internal_char,                                     \
    const String*:      _append_str_internal_string,                                   \
    String*:            _append_str_internal_string,                                   \
    default:            _report_error_internal                                         \
)(str_ptr, append_ptr)         
        
#define string_insert(str_ptr, insert_ptr, index) _Generic((insert_ptr),               \
    const char*:        _insert_str_internal_char,                                     \
    char*:              _insert_str_internal_char,                                     \
    const String*:      _insert_str_internal_string,                                   \
    String*:            _insert_str_internal_string,                                   \
    default:            _report_error_internal                                         \
)(str_ptr, insert_ptr, index)                  
        
#define string_parse(str_ptr, type) _Generic((type)0,                                  \
    long:               _parse_str_internal_long,                                      \
    long long:          _parse_str_internal_long_long,                                 \
    unsigned long:      _parse_str_internal_unsigned_long,                             \
    unsigned long long: _parse_str_internal_unsigned_long_long,                        \
    float:              _parse_str_internal_float,                                     \
    double:             _parse_str_internal_double,                                    \
    long double:        _parse_str_internal_long_double,                               \
    default:            _report_error_internal                                         \
)(str_ptr)         
            
#define string_find(str_ptr, pattern) _Generic((pattern),                              \
    const char*:        _find_str_internal_char,                                       \
    char*:              _find_str_internal_char,                                       \
    const String*:      _find_str_internal_string,                                     \
    String*:            _find_str_internal_string,                                     \
    default:            _report_error_internal                                         \
)(str_ptr, pattern)                
            
#define string_remove(str_ptr, pattern) _Generic((pattern),                            \
    const char*:        _remove_str_internal_char,                                     \
    char*:              _remove_str_internal_char,                                     \
    const String*:      _remove_str_internal_string,                                   \
    String*:            _remove_str_internal_string,                                   \
    default:            _report_error_internal                                         \
)(str_ptr, pattern)                
            
#define string_equals(str1_ptr, str2_ptr) _Generic((str2_ptr),                         \
    const char*:        _equal_str_internal_char,                                      \
    char*:              _equal_str_internal_char,                                      \
    const String*:      _equal_str_internal_string,                                    \
    String*:            _equal_str_internal_string,                                    \
    default:            _report_error_internal                                         \
)(str1_ptr, str2_ptr)                  
            
#define split_remove_all(split_ptr, pattern) _Generic((pattern),                       \
    const char*:        _remove_spl_internal_char,                                     \
    char*:              _remove_spl_internal_char,                                     \
    const String*:      _remove_spl_internal_string,                                   \
    String*:            _remove_spl_internal_string,                                   \
    default:            _report_error_internal                                         \
)(split_ptr, pattern)          
        
#define split_contains(split_ptr, str_ptr) _Generic((str_ptr),                         \
    const char*:        _contains_spl_internal_char,                                   \
    char*:              _contains_spl_internal_char,                                   \
    const String*:      _contains_spl_internal_string,                                 \
    String*:            _contains_spl_internal_string,                                 \
    default:            _report_error_internal                                         \
)(split_ptr, str_ptr)

typedef struct String {
    size_t len;
    size_t capacity;
    char   *arr;
} String;

typedef struct StringSplit {
    size_t len;
    size_t capacity;
    String *arr;
} StringSplit;

int64_t            _report_error_internal(String *str, ...);
           
void               _append_str_internal_char(String *str, const char *append);
void               _append_str_internal_string(String *str, const String *append);
              
void               _insert_str_internal_char(String *str, const char *insert, int64_t index);
void               _insert_str_internal_string(String *str, const String *insert, int64_t index);

long               _parse_str_internal_long(String *str);
long long          _parse_str_internal_long_long(String *str);
unsigned long      _parse_str_internal_unsigned_long(String *str);
unsigned long long _parse_str_internal_unsigned_long_long(String *str);
float              _parse_str_internal_float(String *str);
double             _parse_str_internal_double(String *str);
long double        _parse_str_internal_long_double(String *str);
    
int64_t            _find_str_internal_char(String *str, const char *pat);
int64_t            _find_str_internal_string(String *str, const String *pat);
           
void               _remove_str_internal_char(String *str, const char *pat);
void               _remove_str_internal_string(String *str, const String *pat);
           
bool               _equal_str_internal_char(String *str1, const char *str2);
bool               _equal_str_internal_string(String *str1, const String *str2);
           
void               _remove_spl_internal_char(StringSplit *split, const char *pat);
void               _remove_spl_internal_string(StringSplit *split, const String *pat);
           
bool               _contains_spl_internal_char(StringSplit *split, const char *str);
bool               _contains_spl_internal_string(StringSplit *split, const String *str);

String             string_new(const char *str);
char               *string_to_cstring(String *str);
String             string_with_len(const char *str, size_t len);
void               string_print(String *str);
void               string_println(String *str);
void               string_print_debug(String *str);
void               string_println_debug(String *str);
char               string_at(String *str, int64_t index);
void               string_push(String *str, char character);
StringSplit        string_split(String *str, const char *delimeter); 
void               string_free(String *str);
void               string_map(String *str, void (*func)(String *str));
void               string_change(String *str, int64_t index, char chr);
void               string_to_upper(String *str);
void               string_to_lower(String *str);
String             string_input();
bool               string_is_empty(String *str);
void               string_trim_right(String *str);
void               string_trim_left(String *str);
void               string_trim(String *str);
String             string_read_file(const char *filename);
StringSplit        string_lines(String *str);
String             string_clone(String *str);
void               string_clear(String *str);
bool               string_contains(String *str, char ch);
int                string_compare(String *str1, String *str2);
void               string_reverse(String *str);

StringSplit        split_new();
void               split_free(StringSplit *split);
void               split_push(StringSplit *split, String data);
String             split_pop(StringSplit *split);
String             split_at(StringSplit *split, int64_t index);
void               split_print(StringSplit *split);
void               split_println(StringSplit *split);
void               split_print_debug(StringSplit *split);
void               split_println_debug(StringSplit *split);
       
char               *format(const char *fmt, ...);