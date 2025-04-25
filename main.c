#include "string.h"
#include <stdio.h>

int main(void) {
    String str = String("9");
    printf("%f", string_parse(&str, double));

    return 0;
}