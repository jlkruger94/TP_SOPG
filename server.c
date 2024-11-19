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
static int analize_cliente_req(char* req, char* res, size_t max_length);
/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */
static int analize_cliente_req(char* req, char* res, size_t max_length) {
    /* Valida los parametros de entrada */
    if (req == NULL || res == NULL) return FATAL_ERROR;

    /* Si el último caracter es un salto de linea lo elimino */
    int str_length_req = stdc_strlen(req) - 1;
    if (req[str_length_req] == '\n') req[str_length_req] = 0x00;

    /* Obtiene los elementos de la petición */
    char *cmd = stdc_split(req," ");
    char *key = stdc_split(NULL," ");
    char *value = stdc_split(NULL," ");

    /* Verifica si se encuentran los elementos necesarios para operar */
    if (cmd == NULL || key == NULL) {
        printf("server: Error en el comando recibido\n");
        return FATAL_ERROR;
    }

    char filepath[BUFFER_LENGTH_S];
    int f;
    /* Arma el path del archivo */
    snprintf(filepath,sizeof(filepath),"%s.txt",key);

    /**********  SET *********************/
    if (stdc_strncmp(cmd,"SET", MAX_CMD_LENGTH) == SUCCESS) {
        /* Arma la respuesta */
        snprintf(res,max_length,"%s","OK\n");

        /* Crea un archivo llamado key en el dir base */
        f = open(filepath,O_CREAT | O_WRONLY | O_TRUNC,FILE_PREMISSION);
        if (f == GENERIC_ERROR) {
            perror("Error al abrir el archivo");
            /* Fallo inadmisible */
            return FATAL_ERROR;
        }
        int n = write(f, value, stdc_strlen(value));
        if (n == GENERIC_ERROR) {
            /* Fallo inadmisible */
            perror("server: Error al escribir el archivo");
            return FATAL_ERROR;
        }
        close(f);

        return SUCCESS;
    }
    /**********  GET *********************/
    else if (stdc_strncmp(cmd,"GET", MAX_CMD_LENGTH) == SUCCESS) {
        char value[BUFFER_LENGTH_S];
        /* Arma una repuesta por defecto*/
        snprintf(res,max_length,"%s","NOTFOUND\n");

        /* Abre el archivo en modo lectura */
        int f = open(filepath, O_RDONLY);
        if (f == GENERIC_ERROR) {
            return GENERIC_ERROR;
        }
        int n = read(f, value, max_length - 1);
        if (n == GENERIC_ERROR) {
            return GENERIC_ERROR;
        }
        close(f);
        /* Arma la respuesta */
        snprintf(res,max_length,"OK\n%s\n",value);
        return SUCCESS;
    }
    /**********  DEL *********************/
    else if (stdc_strncmp(cmd,"DEL", MAX_CMD_LENGTH) == SUCCESS) {
        /* Arma la respuesta */
        snprintf(res,max_length,"%s","OK\n");

        /* Eliminar el archivo */
        if (unlink(filepath) == GENERIC_ERROR) {
           // perror("Error al eliminar el archivo");
            return GENERIC_ERROR;
        }
        return SUCCESS;
    } else {
        /* Error inadmisible */
        printf("server: Error en el comando recibido\n");
        return FATAL_ERROR;
    }

    return GENERIC_ERROR;
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
        int result = analize_cliente_req(buffer,response,sizeof(buffer));
        if (result != SUCCESS) {
            perror("server: Algo salió mal");
            if (result == FATAL_ERROR) exit(EXIT_FAILURE);
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