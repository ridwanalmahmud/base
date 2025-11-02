#include "ds.h"
#include <stdio.h>

int main(void) {
    String str = strNew("Hello, world");
    printf("%s\n", str);
    strFree(str);

    return 0;
}
