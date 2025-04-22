#include "string.h"

int main(void) {
    String str = String("HELLO");
    str.to_lower(&str);
    str.println(&str);

    return 0;
}