/************************************************************************************************
Copyright (c) 2024, José Luid Krüger <jlkruger94@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** @file stdc.c
 ** @brief Biblioteca de funciones útiles de C
 **/

/* === Headers files inclusions =============================================================== */

#include "stdc.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

char* stdc_split(char* str, const char* delim) {
    static char* next = NULL;
    if (str != NULL) {
        next = str;
    }
    if (next == NULL) {
        return NULL;
    }

    char* current = next;
    char* dptr = (char*)delim;

    while (*next != '\0') {
        char* temp = next;
        char* tempd = dptr;

        while (*temp == *tempd && *tempd) {
            temp++;
            tempd++;
        }

        if (*tempd == '\0') {
            *next = '\0';
            next = temp;
            return current;
        }
        next++;
    }

    next = NULL;
    return current;
}

int stdc_strncmp(const char* str1, const char* str2, size_t max_len) {
    size_t i = 0;

    while (i < max_len && str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return (str1[i] > str2[i]) ? 1 : -1;
        }
        i++;
    }

    if (i == max_len) {
        return 0;
    }

    if (str1[i] == '\0' && str2[i] == '\0') {
        return 0;
    }
    return (str1[i] == '\0') ? -1 : 1;
}

size_t stdc_strlen(const char *str) {
    size_t length = 0;

    if (str == NULL) {
        return 0;
    }

    while (*str != '\0') {
        length++;
        str++;
    }

    return length;
}
/* === End of documentation ==================================================================== */