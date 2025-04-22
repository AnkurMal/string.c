#include "string.h"

int main(void) {
    String str = String("HELLO");
    str.to_lower(&str);
    str.println(&str);

    str.free(&str);
    return 0;
}