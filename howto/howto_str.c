#include "ds.h"
#include <stdio.h>

int main(void) {
    String s = strNew("elves,dwarves");
    s = strCat(s, ",dragons");
    char *cs = ",elesser";
    s = strCatFmt(s, "%s", cs);

    int count;
    String *tokens = strnSplit(s, strLen(s), ",", 1, &count);
    for (int i = 0; i < count; i++) {
        printf("Token: %s\n", tokens[i]);
    }
    strFreeSplitRes(tokens, count);

    String sliced = strSlice(s, 0, 5);
    printf("Sliced: %s\n", sliced);

    String copied = strCpy(strEmpty(), "goodbye");
    printf("Copied: %s\n", copied);

    strtoUpper(copied);
    printf("Uppercase: %s\n", copied);

    copied = strMapChars(copied, "OO", "AA", 2);
    printf("Mapped: %s\n", copied);

    strFree(s);
    strFree(sliced);
    strFree(copied);

    return 0;
}
