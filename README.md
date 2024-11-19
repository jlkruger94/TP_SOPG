## TRABAJO PRÁCTICO INTEGRADOR

### SISTEMAS OPERATIVOS DE PROPOSITO GENERAL

### Consigna
Servidor TCP que permite almacenar información ASCII en forma de clave-valor.

El servidor debe:

1. Esperar a que un cliente se conecte mediante el protocolo TCP, puerto 5000.

2. Esperar a que el cliente envíe un comando a ejecutar. El comando se especifica como una secuencia de caracteres ASCII hasta el \n.

3. Realizar la operación correspondiente.

4. Cortar la conexión con el cliente y volver al paso 1.

Los comandos que acepta el servidor son:

``` bash
SET <clave> <valor>\n
```
Se crea en el servidor un archivo llamado `<clave>` con el contenido indicado en `<valor>` (sin incluir el `\n`).
        Se responde al cliente `OK\n`.
```bash
GET <clave>
```
Si existe el archivo correspondiente, se responde al cliente con: `OK\n<valor>\n` (es decir, una línea de texto que dice `OK` y otra que contiene el contenido del archivo). Si no existe, se responde con `NOTFOUND\n`
``` bash
DEL <clave>
```
Si existe la el archivo correspondiente, se elimina. Tanto si existe como no, se responde `OK\n`.

Ante cualquier caso excepcional, informar la causa y finalizar el proceso con código de error.
Cliente

Dado que el protocolo de comunicación es ASCII, no es necesario programar un cliente sino que se pueden utilizar herramientas como nc (netcat) o telnet.


#### Autor: José Luis Krüger