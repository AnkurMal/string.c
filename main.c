#include "string.h"
#include <stdio.h>

int main(void) {
    String str = String("9");
    printf("%ld", string_parse(&str, long));

    return 0;
}