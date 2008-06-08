#include <stdio.h>
#include <stdlib.h>

bool valid_string(char *str) {
    // TODO: Put in a trim function so when we test strlen, it'll be trimmed.
    return (str != NULL) && (strlen(str) > 0);
}

