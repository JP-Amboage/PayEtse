#include <stdio.h>
#include <stdlib.h>
//#include "abb.h"
#include "FuncionsPayETSE.h"
/*
 *
 */
int main(int argc, char** argv){
    //imprimos o logo da empresa
    printf("\n\n"
           "██████╗  █████╗ ██╗   ██╗     ███████╗████████╗███████╗███████╗\n"
           "██╔══██╗██╔══██╗╚██╗ ██╔╝     ██╔════╝╚══██╔══╝██╔════╝██╔════╝\n"
           "██████╔╝███████║ ╚████╔╝█████╗█████╗     ██║   ███████╗█████╗  \n"
           "██╔═══╝ ██╔══██║  ╚██╔╝ ╚════╝██╔══╝     ██║   ╚════██║██╔══╝  \n"
           "██║     ██║  ██║   ██║        ███████╗   ██║   ███████║███████╗\n"
           "╚═╝     ╚═╝  ╚═╝   ╚═╝        ╚══════╝   ╚═╝   ╚══════╝╚══════╝");
    char opcion;
    abb arbol;
    grafo grafo1;
    //reservamos memoria para el arbol
    crear_arbol(&arbol);
    crear_grafo(&grafo1);
    //leemos los datos que pueda haber guardados
    leerArchivo(&arbol,&grafo1);
    do{
        //menu de opciones
        printf("\n___________________\n");
        printf("\nl) Log in");
        printf("\ns) Sair");
        printf("\n___________________\n");
        printf("\nOpcion: ");
        //dejamos un espacio antes de %c para limpiar el buffer
        scanf(" %c",&opcion);
        switch(opcion){
        case 's' : case'S': //tanto S como s significaran salir
            salir(&arbol,&grafo1);
            break;
       default://por defecto se elige la opcion Log In
           login(&arbol,&grafo1);
           break;
        }
    }while (opcion != 's' && opcion !='S');//se imprime el menu en bucle mientras el usuario no solicite salir
    return 0;
}