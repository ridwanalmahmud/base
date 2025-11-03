#include "ds.h"
#include <stdio.h>

int main(void) {
    String s = strNew("elves,dwarves");
    s = strCat(s, ",dragons");
    char *cs = ",elesser";
    s = strCatFmt(s, "%s", cs);
    String token = strTok(s, ",");
    while (token) {
        printf("Token: %s\n", token);
        token = strTok(NULL, ",");
    }
    s = strSlice(s, 0, 5);
    printf("%s\n", s);
    s = strCpy(s, "goodbye");
    printf("%s\n", s);
    strtoUpper(s);
    printf("%s\n", s);
    s = strMapChars(s, "OO", "AA", 2);
    printf("%s\n", s);
    strFree(s);

    return 0;
}
