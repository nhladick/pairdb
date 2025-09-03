#include <string.h>
#include <stdio.h>

#include "stringutil.h"

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
 * This version is modified slightly - updates
 * to error numbers were removed, slight formatting
 * changes were made.
 *
 */

ssize_t strtcpy(char *restrict dst, const char *restrict src, size_t dsize)
{
    bool trunc;
    size_t dlen;
    size_t slen;
    if (dsize == 0) {
        return -1;
    }
    slen = strnlen(src, dsize);
    trunc = (slen == dsize);
    dlen = slen - trunc;
    stpcpy(mempcpy(dst, src, dlen), "");

    return trunc ? -1 : slen;
}

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

void getrandstr(char *dst, size_t len)
{
    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    size_t cslen = strlen(charset);

    unsigned int ruint;
    FILE *f = fopen("/dev/urandom", "r");

    for (size_t i = 0; i < len - 1; i++) {
        fread(&ruint, sizeof(ruint), 1, f);
        dst[i] = charset[ruint % cslen];
    }
    dst[len - 1] = '\0';
    fclose(f);
}








