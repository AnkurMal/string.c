#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// credit: <https://stackoverflow.com/a/400970>
#define foreach(item, var) \
    for(size_t keep = 1, count = 0; keep && count != (var).len; keep = !keep, count++) \
        for(typeof((var).arr[0]) *(item) = &(var).arr[count]; keep; keep = !keep)

#define String(str) string_new(str)

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

String      string_new(const char *str);
String      string_new_with_len(const char *str, size_t len);
void        string_print(String *str);
void        string_println(String *str);
void        string_print_debug(String *str);
void        string_println_debug(String *str);
char        string_at(String *str, int64_t index);
void        string_push(String *str, char character);
StringSplit string_split(String *str, const char *delimeter); 
void        string_free(String *str);
bool        string_equals(String *str1, String *str2);
void        string_map(String *str, void (*func)(String *str));
void        string_change(String *str, int64_t index, char chr);
void        string_to_upper(String *str);
void        string_to_lower(String *str);
String      string_input();
bool        string_is_empty(String *str);
void        string_trim_right(String *str);
void        string_trim_left(String *str);
void        string_trim(String *str);
String      string_read_file(const char *filename);
StringSplit string_lines(String *str);
String      string_clone(String *str);

StringSplit split_new();
void        split_free(StringSplit *split);
void        split_push(StringSplit *split, String data);
String      split_pop(StringSplit *split);
String      split_at(StringSplit *split, int64_t index);
void        split_print(StringSplit *split);
void        split_println(StringSplit *split);
void        split_print_debug(StringSplit *split);
void        split_println_debug(StringSplit *split);

char        *format(const char *fmt, ...);