#include "string.h"
#include <stdio.h>

int main(void) {
    String str = String("  Hello  ");
    string_trim(&str);
    string_print(&str);
    printf("hey");

    string_free(&str);
    return 0;
}