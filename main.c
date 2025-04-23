#include "string.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    char *fmt = format("Heyy %d %s", 2, "6");
    String str = String(fmt);

    string_print_debug(&str);

    string_free(&str);
    free(fmt);
    return 0;
}