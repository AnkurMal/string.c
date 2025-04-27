#include "string.h"
#include <stdio.h>

int main(void) {
    String str = String("1");
    string_reverse(&str);
    string_println(&str);

    return 0;
}