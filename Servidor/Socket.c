//
// Created by david on 12/06/22.
//

#include "Socket.h"

int Lee_Socket (int fd, char *Datos, int Longitud)
{
    int Leido = 0;
    int Aux = 0;

    /*
    * Comprobacion de que los parametros de entrada son correctos
    */
    if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
        return -1;

    /*
    * Mientras no hayamos leido todos los datos solicitados
    */
    while (Leido < Longitud)
    {
        Aux = read (fd, Datos + Leido, Longitud - Leido);
        if (Aux > 0)
        {
            /*
            * Si hemos conseguido leer datos, incrementamos la variable
            * que contiene los datos leidos hasta el momento
            */
            Leido = Leido + Aux;
        }
        else
        {
            /*
            * Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
            * los caracteres leidos hasta ese momento
            */
            if (Aux == 0)
                return Leido;
            if (Aux == -1)
            {
                /*
                * En caso de error, la variable errno nos indica el tipo
                * de error.
                * El error EINTR se produce si ha habido alguna
                * interrupcion del sistema antes de leer ningun dato. No
                * es un error realmente.
                * El error EGAIN significa que el socket no esta disponible
                * de momento, que lo intentemos dentro de un rato.
                * Ambos errores se tratan con una espera de 100 microsegundos
                * y se vuelve a intentar.
                * El resto de los posibles errores provocan que salgamos de
                * la funcion con error.
                */
                switch (errno)
                {
                    case EINTR:
                    case EAGAIN:
                        usleep (100);
                        break;
                    default:
                        return -1;
                }
            }
        }
    }

    /*
    * Se devuelve el total de los caracteres leidos
    */
    return Leido;
}

int Escribe_Socket (int fd, char *Datos, int Longitud)
{
    int Escrito = 0;
    int Aux = 0;

    /*
    * Comprobacion de los parametros de entrada
    */
    if ((fd == -1) || (Datos == NULL) || (Longitud < 1))
        return -1;

    /*
    * Bucle hasta que hayamos escrito todos los caracteres que nos han
    * indicado.
    */
    while (Escrito < Longitud)
    {
        Aux = write (fd, Datos + Escrito, Longitud - Escrito);
        if (Aux > 0)
        {
            /*
            * Si hemos conseguido escribir caracteres, se actualiza la
            * variable Escrito
            */
            Escrito = Escrito + Aux;
        }
        else
        {
            /*
            * Si se ha cerrado el socket, devolvemos el numero de caracteres
            * leidos.
            * Si ha habido error, devolvemos -1
            */
            if (Aux == 0)
                return Escrito;
            else
                return -1;
        }
    }

    /*
    * Devolvemos el total de caracteres leidos
    */
    return Escrito;
}



