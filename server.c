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

/** @file server.c
 ** @brief Definición de la función principal del programa
 **/

/* === Headers files inclusions =============================================================== */

#include "server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "stdc.h"
/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */
static int analyze_client_req(char* req, char* res, size_t max_length);
/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

static int analyze_client_req(char* req, char* res, size_t max_length) {
    /* Validación inicial */
    if (req == NULL || res == NULL || max_length == 0) return FATAL_ERROR;

    /* Eliminar salto de línea si existe */
    int req_length = stdc_strlen(req);
    if (req_length > 0 && req[req_length - 1] == '\n') {
        req[req_length - 1] = '\0';
    }

    /* Obtener elementos del comando */
    char *cmd = stdc_split(req, " ");
    char *key = stdc_split(NULL, " ");
    char *value = stdc_split(NULL, " ");

    /* Validar elementos */
    if (cmd == NULL || key == NULL) {
        snprintf(res, max_length, "server: Error, comando invalido\n");
        return FATAL_ERROR;
    }

    /* Buffer para operaciones con archivos */
    char filepath[BUFFER_LENGTH_S];
    snprintf(filepath, sizeof(filepath), "%s.txt", key);

    /**********  SET *********************/
    if (stdc_strncmp(cmd, "SET", MAX_CMD_LENGTH) == SUCCESS) {
        /* Validar value */
        if (value == NULL) {
            printf("server: Error, no existe un valor a Setear\n");
            return FATAL_ERROR;
        }

        /* Crear/abrir archivo */
        int fd = open(filepath, O_CREAT | O_WRONLY | O_TRUNC, FILE_PERMISSION);
        if (fd == GENERIC_ERROR) {
            perror("Error al abrir el archivo");
            return FATAL_ERROR;
        }

        /* Escribir valor en el archivo */
        if (write(fd, value, stdc_strlen(value)) == GENERIC_ERROR) {
            perror("Error al escribir en el archivo");
            close(fd);
            return FATAL_ERROR;
        }

        close(fd);
        snprintf(res, max_length, "OK\n");
        return SUCCESS;
    }
    /**********  GET *********************/
    else if (stdc_strncmp(cmd, "GET", MAX_CMD_LENGTH) == SUCCESS) {
        /* Leer archivo */
        int fd = open(filepath, O_RDONLY);
        if (fd == GENERIC_ERROR) {
            snprintf(res, max_length, "NOTFOUND\n");
            return GENERIC_ERROR;
        }

        char value[BUFFER_LENGTH_S];
        ssize_t bytes_read = read(fd, value, sizeof(value) - 1);
        close(fd);

        if (bytes_read == GENERIC_ERROR) {
            printf("server: Error al leer el archivo\n");
            return FATAL_ERROR;
        }

        value[bytes_read] = '\0';
        snprintf(res, max_length, "OK\n%s\n", value);
        return SUCCESS;
    }
    /**********  DEL *********************/
    else if (stdc_strncmp(cmd, "DEL", MAX_CMD_LENGTH) == SUCCESS) {

        snprintf(res, max_length, "OK\n");
        /* Eliminar archivo */
        if (unlink(filepath) == GENERIC_ERROR) {
            return GENERIC_ERROR;
        }
        return SUCCESS;
    } else {
        printf("server: Error, comando no encontrado\n");
        return FATAL_ERROR;
    }
}

/* === Public function implementation ========================================================== */

int main(void) {
    /* Creamos socket */
    int s = socket(AF_INET, SOCK_STREAM, 0);

    /* Cargamos datos de IP:PORT del server */
    struct sockaddr_in serveraddr;
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &(serveraddr.sin_addr)) <= 0) {
        fprintf(stderr, "ERROR invalid server IP\n");
        exit(EXIT_FAILURE);
    }

    /* Abrimos puerto con bind() */
    if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == GENERIC_ERROR) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* Seteamos socket en modo Listening */
    if (listen(s, 1) == GENERIC_ERROR) { /* backlog=10 */
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        /* Ejecutamos accept() para recibir conexiones entrantes */
        socklen_t addr_len = sizeof(struct sockaddr_in);
        struct sockaddr_in clientaddr;
        int newfd;
        printf("server: esperando una conexion...\n");
        if ((newfd = accept(s, (struct sockaddr *)&clientaddr, &addr_len)) == GENERIC_ERROR) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char ipClient[IP_CLIENT_LENGTH];
        inet_ntop(AF_INET, &(clientaddr.sin_addr), ipClient, sizeof(ipClient));
        printf("server: conexion desde:  %s\n", ipClient);

        /* Leemos mensaje de cliente */
        char buffer[BUFFER_LENGTH_S] = { 0 };
        char response [BUFFER_LENGTH_S] = { 0 };

        int n = read(newfd, buffer, sizeof(buffer));
        if (n == GENERIC_ERROR) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        buffer[n] = 0x00;
        int result = analyze_client_req(buffer,response,sizeof(buffer));
        if (result != SUCCESS) {
            if (result == FATAL_ERROR) exit(EXIT_FAILURE);
            perror("server: Algo salió mal");
        }

        /* Enviamos mensaje a cliente */
        n = write(newfd, response, stdc_strlen(response));
        if (n == GENERIC_ERROR) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        printf("server: envie %d bytes\n", n);

        /* Esperamos a que el cliente cierre */
        printf("server: esperando a que el cliente cierre la conexion...\n");
        n = read(newfd, buffer, sizeof(buffer));
        if (n == GENERIC_ERROR) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (n != 0) {
            /* No deberia ocurrir ya que si el cliente cerro la conexion, read devuelve 0 */
            buffer[n] = 0x00;
            printf("server: recibi %d bytes:%s\n", n, buffer);
        }

        /* Cerramos conexion con cliente */
        close(newfd);
    }

    return EXIT_SUCCESS;
}

/* === End of documentation ==================================================================== */