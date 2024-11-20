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

#ifndef STDC_H
#define STDC_H

/** @file STDC.h
 ** @brief Definición de la función principal del programa
 **/

/* === Headers files inclusions ================================================================ */
#include <stdio.h>
#include <stdlib.h>
/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =============================================================== */

/* === Public data type declarations =========================================================== */

/* === Public variable declarations ============================================================ */

/* === Public function declarations ============================================================ */

/*
* @name stdc_split
* @brief Divide una cadena en tokens delimitados por una subcadena específica.
* @params
*   - str: Puntero a la cadena de entrada que se desea dividir. Debe pasarse la cadena completa en la primera llamada.
*          En llamadas subsiguientes, pase NULL para continuar con la cadena original.
*   - delim: Subcadena delimitadora que separa los tokens.
* @retval
*   - Un puntero al inicio del siguiente token en la cadena.
*   - NULL si no hay más tokens disponibles.
*/
char* stdc_split(char* str, const char* delim);

/*
* @name stdc_strncmp
* @brief Compara dos cadenas de texto hasta un máximo de caracteres.
* @params
*   - str1: Puntero a la primera cadena a comparar.
*   - str2: Puntero a la segunda cadena a comparar.
*   - max_len: Número máximo de caracteres a comparar.
* @retval
*   - 0 si las cadenas son iguales hasta `max_len` o el final de ambas.
*   - 1 si `str1` es mayor que `str2` en el primer carácter diferente.
*   - -1 si `str1` es menor que `str2` en el primer carácter diferente.
*/
int stdc_strncmp(const char* str1, const char* str2, size_t max_len);

/*
* @name stdc_strlen
* @brief Calcula la longitud de una cadena de caracteres.
* @params
*   - str: Puntero a la cadena de entrada.
* @retval
*   - La longitud de la cadena, excluyendo el carácter nulo final.
*   - 0 si el puntero de entrada es NULL.
*/
size_t stdc_strlen(const char *str);
/* === End of documentation ==================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* STDC_H */