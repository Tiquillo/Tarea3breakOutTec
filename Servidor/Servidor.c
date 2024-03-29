//
// Created by david on 12/06/22.
//

#include "Servidor.h"

/**
 * Inicia el servidor
 * @return int
 */

int iniciarServidor(){

    int socketServidor;
    int socketCliente[MAX_CLIENTES];
    int numeroClientes = 0;
    fd_set descriptoresLectura;

    int buffer;
    int maximo;
    int i;
    int accion;

    char cadena[TAM_BUFFER];
    char *ladrillos;
    int largoLadrillos;

    int auxiliar;
    int longitudCadena;

    socketServidor = Abre_Socket_Inet (PUERTO);
    if (socketServidor == -1){
        printf ("Error al abrir el socket\n");
        exit (1);
    } else {
        printf ("Servidor iniciado\n");
    }

    while (1) {

        ///Se comprueba si algun cliente nuevo desea conectarse y se le admite.
        if (FD_ISSET(socketServidor, &descriptoresLectura))
            nuevoCliente(socketServidor, socketCliente, &numeroClientes);

        ///Se eliminan todos los clientes que hayan cerrado la conexion.
        compactaClaves(socketCliente, &numeroClientes);

        ///Se inicializan los descriptores de lectura.
        FD_ZERO (&descriptoresLectura);

        ///Se agrega al select() el socket servidor.
        FD_SET (socketServidor, &descriptoresLectura);

        ///Se agregan para el select() los sockets con los clientes ya conectados.
        for (i = 0; i < numeroClientes; i++)
            FD_SET (socketCliente[i], &descriptoresLectura);

        ///Se verifica el valor del descriptor mas grande. Si no hay ningun cliente,
        ///devolvera 0.
        maximo = obtenerMaximo(socketCliente, numeroClientes);
        if (maximo < socketServidor)
            maximo = socketServidor;

        ///Espera indefinida hasta que alguno de los descriptores tenga algo que
        ///decir: un nuevo cliente o un cliente ya conectado que envia un mensaje.
        select(maximo + 1, &descriptoresLectura, NULL, NULL, NULL);

        for(i = 0; i < numeroClientes; i++) {

            if(FD_ISSET (socketCliente[i], &descriptoresLectura)) {

                if((Lee_Socket(socketCliente[i], (char *)&buffer, sizeof(int)) > 0)){


                    ///El entero recibido hay que transformalo de formato de red a
                    ///formato de hardware.
                    longitudCadena = ntohl(buffer);
                    ///Se lee la cadena enviada
                    Lee_Socket(socketCliente[i], cadena, longitudCadena);
                    //printf("Cliente %d envia %s\n", i + 1, cadena);

                    accion = AccionesServidor(cadena);

                    //printf("Accion: %d\n", accion);

                    if(accion != 2) {

                        ///Envio de la lista de ladrillos
                        ladrillos = iniciarBricks(accion);
                        printf("%lu\n", strlen(ladrillos));
                        largoLadrillos = htonl(strlen(ladrillos)+1 );
                        printf("bricks enviados: %s\n",ladrillos);
                        printf("bricks enviados en formato de red: %d\n", largoLadrillos);
                        Escribe_Socket(socketCliente[i], (char *)&largoLadrillos, sizeof(int));
                        Escribe_Socket(socketCliente[i], ladrillos, largoLadrillos);
                        free(ladrillos);


                    } else{

                        ///Se envia datos a los clientes observadores
                        for(int j = 0; j < numeroClientes; j++){

                            if(i != j){

                                ///Se envia a los observadores el entero que indica
                                auxiliar = htonl(longitudCadena);
                                Escribe_Socket(socketCliente[j], (char *)&auxiliar, sizeof(int));
                                Escribe_Socket(socketCliente[j], cadena, longitudCadena);

                            }
                        }

                    }

                } else{
                    printf("Cliente %d ha cerrado la conexion\n", i+1);
                    socketCliente[i] = -1;
                }

            }
        }

    }

}

/**
 * Crea un nuevo socket cliente.
 * Se le pasa el socket servidor y el arreglo de clientes, con el numero de clientes
 * ya conectados.
 */
void nuevoCliente(int servidor, int *clientes, int *nClientes){
    ///Acepta la conexion con el cliente, y la guarada en el arreglo.
    clientes[*nClientes] = Acepta_Conexion_Cliente(servidor);
    (*nClientes)++;

    ///Si se super el maximo de clientes, se cierra la conexion, se dejan como estaba
    ///y se retorna.
    if((*nClientes) >= MAX_CLIENTES)
    {
        close(clientes[(*nClientes) - 1]);
        (*nClientes)--;
        return;
    }
    //printf(*nClientes);

    ///Envia su numero de cliente al cliente.
    //Escribe_Socket(clientes[(*nClientes)-1], (char *)&aux, sizeof(int));

    ///Escribe en pantalla que ha aceptado al cliente y se retorna.
    //printf(("Aceptado cliente %d\n", *nClientes));
    printf("Aceptado cliente\n");
}

/**
* Funcion que devuelve el valor maximo en la tabla.
* Supone que los valores validos de la tbal son positivos y mayores que 0.
* Devuelve 0 si n es 0 o la tabla es NULL.
*/
int obtenerMaximo(int *tabla, int n){
    int i;
    int max;
    if((tabla == NULL) || (n < 1))
        return 0;
    max = tabla[0];
    for(i = 0; i < n; i++)
    {
        if(tabla[i] > max)
            max = tabla[i];
    }
    return max;
}

/**
* Buscar en el arreglo todas las posiciones con -1 y las elimina, copiando
* encima las posiciones siguientes.
*/
void compactaClaves(int *tabla, int *n){
    int i,j;
    if((tabla == NULL) || ((*n) == 0))
        return;
    j=0;
    for(i = 0; i < (*n); i++)
    {
        if(tabla[i] != -1)
        {
            tabla[j] = tabla[i];
            j++;
        }
    }
    *n = j;
};