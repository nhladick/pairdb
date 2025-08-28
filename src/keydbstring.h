#ifndef KEYDBSTRING_H
#define KEYDBSTRING_H

#include <sys/types.h>

// maximum buffer sizes
//#define TBL_NAME_MAX 32
//#define KEY_MAX 100
//#define VAL_MAX 100

enum {
    TBL_NAME_MAX = 32,
    KEY_MAX = 100,
    VAL_MAX = 100
};

/*
 * Public domain string copy function
 * recommended for use over functions
 * like strcpy and strncpy. Not included
 * in any official libraries.
 *
 * Copies src string to dst, truncating
 * if necessary but always including
 * the terminating NUL character.
 *
 * Returns length of source string or -1
 * if source string is truncated.
 *
 * https://man7.org/linux/man-pages/man7/string_copying.7.html
 *
 */

ssize_t strtcpy(char *restrict dst, const char *restrict src, size_t dsize);

#endif // KEYDBSTRING_H
