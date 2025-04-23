#include "string.h"
#include <stdio.h>

int main(void) {
    printf("Enter a string: ");
    String str = string_input();

    printf("The string entered: ");
    string_println(&str);

    string_free(&str);
    return 0;
}