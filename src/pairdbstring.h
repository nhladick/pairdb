#ifndef PAIRDBSTRING_H
#define PAIRDBSTRING_H

#include <sys/types.h>

// maximum buffer size constants
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

/*
 * Generates a random nul-terminated string
 * of length len - 1 using characters listed
 * below. Writes string and nul char to dst.
 *
 * Characters:
 *      - ascii digits 0 through 9
 *      - letters a through z
 *
 * Note: works only on Linux or BSD-like
 * systems - uses /dev/urandom
 */

void getrandstr(char *dst, size_t len);

#endif // PAIRDBSTRING_H
