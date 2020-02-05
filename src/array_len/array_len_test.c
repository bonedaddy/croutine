#include <stdio.h>
#include "../../include/array_len/array_len.h"  // get it from local dir

int main() {
    int numbers[] = {0, 1, 2, 3, 4, 5};
    printf("%zd\n", array_len(numbers));
    char *words[] = {"hello world", "this is a test", "and here is some test datachan"};
    printf("%zd\n", array_len(words));
    printf("%zd\n", array_size(words));
}