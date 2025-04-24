#include "string.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    char *fmt = format("Heyy %d %s", 2, "6");
    String str = String(fmt);
    String clone = string_clone(&str);

    string_println_debug(&str);
    string_println_debug(&clone);

    string_free(&str);
    free(fmt);
    return 0;
}