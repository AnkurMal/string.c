#include "string.h"
#include <stdio.h>

int main(void) {
    String str = String("Ankur");
    string_append(&str, " Mallick");
    string_println(&str);

    return 0;
}