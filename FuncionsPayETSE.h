//
// Created by jp on 3/10/18.
//

//Guardas del compilador
#ifndef __FUNCIONSPAYETSE_H
#define __FUNCIONSPAYETSE_H

//incluimos la biblioteca abb.h con el TAD arbol
#include "abb.h"

//recibe un arbol binario A por referencia , un grafo G por referencia y el nombre del emisor
// pide los datos de la transferencia a emitir y la añade
void introducirTransferencia(abb *A,grafo *G, char* nome);

//recibe un arbol A y un nombre de usuario, lo busca
// en el arbol e imprime sus datos
void consultaPorUsuario(abb A,tipoclave nomeUsuario);

//recibe un arbol A y un nombre de usuario, busca en el arbol A
// las transferencias que recibio e imprime sus datos por pantalla
void consultaPorReceptor(abb A,tipoclave usuario);

//recibe un arbol A y un grafo G por referencia, pide un nombre de usuario, lo busca en
//A y lo elimina, tambien elimina su nombre en las transferencias que recibio
//emitidas por otros usuarios y lo borra del grafo G
void eliminarUsuarioAsAdmin(abb *A, grafo*G);

//recibe un arbol A por referencia,un grafo G por referencia y un nombre de usuario que busca en
//A y lo elimina, tambien elimina su nombre en las transferencias que recibio
//emitidas por otros usuarios y lo borra del grafo G
void eliminarUsuario(abb *A,grafo *G, tipoclave nomeUsuario);

//recibe un arbol A, imprime su inorden y libera su memoria
void salir(abb *A,grafo *G);

//recibe un arbol A y un nombre de usuario e imprime su total de dinero
// transferido y sus maxima/s transferencia/s
void estatisticasUsuario(abb A,tipoclave nomeUsuario);

//recibe un arbol A por referencia y lo modifica introduciendo transferencias guardadas en un .txt
void leerArchivo(abb *A,grafo *G);

//abre un menu con las opciones de los usuarios cuando inician en el programa
void login(abb*A,grafo*G);

//recibe un grafo G e imprime por pantalla el nombre de el/los usuario/s
// cuyo vertice tiene el mayor grado de entrada+grado salida
void usuarioMasActivo(grafo G);

//recibe un grafo G e imprime o seu numero de vertices, o numero de vertices
// medio co que estan relacoinados os vertices e o/s vertice/s con maior grado
// de entrada e de salida
void estadisticasGlobais(grafo G);

//Imprime el recorrdio inorden del arbol A que recibe como parametro (es recursiva)
void inorden(abb A);

//recibe un grafo G como parametro y lo imprime
void imprimirGrafo(grafo G);

//Mostrarase o grafo local do usuario que se corresponde co nome pasado por parametro e pedirase o nome dun
// receptor. Se o usuario está no grafo local, bórrase como amigo tanto no grafo G recibifo por referencia coma
// no arbol A recibido por referencia
void eliminarAmigo(abb *A, grafo *G, char* nome);
#endif /*__FUNCIONSPAYETSE_H*/
