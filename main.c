#include "string.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    String str = String("Ankur");
    String str1 = String(" Mallick");
    string_append(&str, &str1);
    string_println_debug(&str);

    return 0;
}