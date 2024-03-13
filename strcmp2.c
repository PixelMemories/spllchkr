#include <stdio.h>

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 == '\'') {
            s1++;
            continue;
        }
        if (*s2 == '\'') {
            s2++;
            continue;
        }
        if (*s1 != *s2) {
            return (*s1 - *s2);
        }
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

