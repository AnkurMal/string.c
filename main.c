#include "string.h"
#include <stdio.h>

int main(void) {
    String str = String("Hello World");
    Vec spl = string_split(&str, " ");
    foreach(i, spl) {
        string_println(&i);
    }

    vec_free(&spl);
    string_free(&str);
    return 0;
}