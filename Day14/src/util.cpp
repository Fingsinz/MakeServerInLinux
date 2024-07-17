#include "util.h"
#include <iostream>

void errorif(bool condition, const char *errmsg) {
    if (condition) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}