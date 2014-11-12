#include <stdio.h>
#include <stdlib.h>

#include <limits.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "misc.h"

int mkd (const char *basedir, mode_t mode, int level, int size) {
    int i, w = 0;
    char filename [LINE_MAX];

    if (level <= 0) return 0;
    if (size <= 0) return 0;

    unsigned int x = size -1;
    while (x) {
        w ++;
        x >>= 4;
    }

    for (i = 0; i < size; i ++) {
        snprintf (filename, sizeof (filename), "%s/%0*x", basedir, w, i);

        if (mkdir (filename, mode) == -1) {
            WARNING ("mkd, mkdir \"%s\" failed, %m\n", filename);
            return -1;
        }

        if (level > 1 && mkd (filename, mode, level -1, size) == -1) return -1;
    }

    return 0;
}
