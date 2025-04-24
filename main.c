#include "string.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    String str = String(",a,a,");
    foreach(i, string_split(&str, ",")) {
        string_println(i);
    }

    return 0;
}