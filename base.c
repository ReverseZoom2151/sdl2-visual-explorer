#include "base.h"
#include <stdio.h>
#include <stdlib.h>

void succeed(const char *message) { printf("%s\n", message); }

void fail(const char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

#ifdef baseTest

int main() {
    succeed("Base module OK");
    return 0;
}

#endif
