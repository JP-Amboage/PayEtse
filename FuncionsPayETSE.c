//
// Created by jp on 3/10/18.
//

//Incluimos las bibliotecas que vamos a usar
#include "FuncionsPayETSE.h"
#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//En todas las funciones al pedir los datos de nombreUsuario, receptor (que son strings)
//dejamos un espacio antes de %s para limpiar el buffer ademas para leer concepto usaremos %[^\r\n] para
//para recojer todos los caracteres que introduzca el usuario excepto aquellos que esten entre [^ e ]

void _borrarAmigo(abb*A,char* usuario,char* examigo);

//Funcion privada para crear un usuario "nuevo"
//recibe el tipoelemento donde lo va a guardar y el nombre del usuario
//crea una pila vacia para el campo transferencias
void _creaUsuario(tipoelem *E,tipoclave nome,tipoclave emisor){
    tipovertice V1,V2;
    strcpy(V2.usuario,nome);
    strcpy(V1.usuario,emisor);
    strcpy(E->nomeUsuario,nome);
    crear_pila(&(E->transferencias));
    crear_grafo(&(E->amigos));
    insertar_vertice(&(E->amigos),V1);
    insertar_vertice(&(E->amigos),V2);
    crear_arco(&(E->amigos),posicion((E->amigos),V1),posicion((E->amigos),V2));
}
//Funcion privada para realizar un conjunto de instrucciones que se repiten
//mucho en el programa (aporta modularidad)
//recibe dos pilas por refernecia, pasa los valores de la 1 a la 2 y elimina la 1
void _vaciarPila(pila *aux,pila *principal){
    while (!es_vacia_pila(*aux)) {//mientras la pila aux no es vacia
        //devolvemos sus elementos a la pila principal
        push(principal, tope(*aux));
        pop(aux);
    }
    //destruimos la pila auxiliar
    destruir_pila(aux);
}
//privada, recibe un elemento de una pila e imprime sus campos
// (los campos de una transaccion)
void _printTransaccion(tipoelempila elem){
    printf("\t\tReceptor: %s", elem.receptor);
    printf("\t\tCantidad: %f", elem.cantidad);
    printf("\t\tConcepto: %s\n", elem.concepto);
}
//privada, imprime todos los elementos de una pila
// que recibe por referencia como parametro
void _imprimirPila(pila *P) {
    //declaramos y creamos una pila auxiliar
    pila aux;
    crear_pila(&aux);
    //mientras la pila que queremos imprimir no es vacia
    while (!es_vacia_pila(*P)) {
        //sacamos sus elementos para leerlos y los metemos
        //en una pila auxiliar para no perderlos
        push(&aux,tope(*P));
        pop(P);
        _printTransaccion(tope(aux));
    }
    //volvemos a meter los valores almacenados en aux en la pila principal
    _vaciarPila(&aux,P);
}

//funcion privada para imprimir un nodo, recibe como argumento
//el tipoelem E cuyos datos imprimira
void _printNodo(tipoelem *E){
    //imprime simepre el nombre de usuario
    printf("\nUsuario: %s\n",E->nomeUsuario);
    if(!es_vacia_pila(E->transferencias)){
        //si hay transferencias las imprime
        _imprimirPila(&(E->transferencias));
    }
    //Si la pila de transferencias es vacia se imprime un mensaje por pantalla
    else printf("\t\tEl usuario aun no realizo ninguna transferencia\n");
}

//Publica, imprime el recorrdio inorden de A (es recursiva)
void inorden(abb A){
    tipoelem E;
    //comprobamos que A no sea vacio
    if(!es_vacio(A)){
        //hacemos el inorden de la izquierda
        inorden(izq(A));
        //imprimimos el nodo en el que estamos con _printNodo()
        info(A,&E);
        _printNodo(&E);
        //modificamos el arbol despues de imprimir
        modificar(&A,E);
        //hacemos el inorden de la derecha
        inorden(der(A));
    }
}//funcion privada, recibe un abb por referencia un tipo nodo y un tipoelemento pila
//se usa en introducir transferencia y en leer archivo para introducir nuevos usuarios
// en el arbol
void _introducirDatos(abb *A,grafo *G,tipoelem E,tipoelempila P){
    tipovertice V1,V2;
    strcpy(V1.usuario,E.nomeUsuario);
    strcpy(V2.usuario,P.receptor);
    if(!strcmp(P.receptor,"*")){
        crear_pila(&(E.transferencias));
        crear_grafo(&(E.amigos));
        insertar_vertice(&(E.amigos),V1);
        insertar(A,E);
        insertar_vertice(G,V1);
        return;
    }
    if(!es_miembro_clave(*A,E.nomeUsuario)){
        //si no existe ningun usuario con el nombre del emisor
        //insertamos los datos de la transferencia
        crear_pila(&(E.transferencias));
        push(&(E.transferencias),P);
        crear_grafo(&(E.amigos));
        insertar_vertice(&(E.amigos),V1);
        insertar_vertice(&(E.amigos),V2);
        crear_arco(&(E.amigos),posicion(E.amigos,V1),posicion(E.amigos,V2));
        insertar(A,E);
        insertar_vertice(G,V1);

    }
    else {
        //si el usuario ya existe modificamos los datos
        //existentes para introducir los de la nueva transferencia
        buscar_nodo(*A, E.nomeUsuario, &E);
        push(&(E.transferencias), P);
        if(!existe_vertice((E.amigos),V2)) insertar_vertice(&(E.amigos),V2);
        crear_arco(&(E.amigos),posicion((E.amigos),V1),posicion((E.amigos),V2));
        modificar(A, E);
    }
    if(!es_miembro_clave(*A,P.receptor)){
        //si el receptor no existe como usuario
        //creamos un usuario nuevo con su nombre y el resto
        //de campos con valores por defecto
        _creaUsuario(&E,P.receptor,E.nomeUsuario);
        //insertamos el usuario que acabamos de crear
        insertar(A,E);
        insertar_vertice(G,V2);
    }
    else{
        buscar_nodo(*A, P.receptor, &E);
        if(!existe_vertice((E.amigos),V1)) insertar_vertice(&(E.amigos),V1);
        crear_arco(&(E.amigos),posicion((E.amigos),V1),posicion((E.amigos),V2));
        modificar(A, E);
    }
    //creamos la relacion en sentido correspondiente en el grafo global
    crear_arco(G,posicion(*G,V1),posicion(*G,V2));
}
//Publica, recibe A por referencia porque lo modifica
//para introducir datos de una transfrencia
void introducirTransferencia(abb *A, grafo *G,char* nome){
    tipoelem E;
    tipoelempila P;
    //Pedimos al usuario los datos de la transferencia
    strcpy(E.nomeUsuario,nome);
    printf("\nIntroduce o nome de usuario recptor:\t");
    scanf(" %s",P.receptor);
    printf("\nIntroduce o conceptpo:\t");
    scanf(" %[^\r\n]",P.concepto);
    do {
        printf("\nIntroduce a cantidade a transferir :\t");
        scanf("%f", &(P.cantidad));
        if(P.cantidad<=0) printf("\nLembra que a cantidade a transferir debe ser superior a 0\n");
    }while (P.cantidad<=0);
    _introducirDatos(A,G,E,P);
    //Ya no imprimimos el inorden del arbol resultante
    //printf("\nRECORRIDO INORDEN:\n\n");
    //inorden(*A);
}

//Publica, recibe el arbol A sin ser por referencia pues no lo modifica
void consultaPorUsuario(abb A,tipoclave nomeUusario){
    tipoelem E;
    //pide el nombre de un usuario
    strcpy(E.nomeUsuario,nomeUusario);
    if(es_miembro_clave(A,E.nomeUsuario)){
        //si existe imprime los datos asociados a todas las transferencias que hizo
        buscar_nodo(A,E.nomeUsuario,&E);
        _printNodo(&E);
        //despues de imprimir modificamos el arbol
        modificar(&A,E);
    }
    //si no existe imprime un mensaje por pantalla
    else printf("No existe ningun usuario con ese nombre\n");
}

//Privada, recibe un arbol A ,un nombre de usuario , y un * int (debe tener valor 0)
// que incrementamos cada vez que se encuentra una transferencia de "interes"(recursiva)
void _busquedaReceptor(abb A,tipoclave nome,int *bandera){
    if(!es_vacio(A)){//si el arbol no es vacio
        tipoelem E;
        tipoelempila P;
        //declaramos y creamos una pila auxiliar
        pila aux;
        crear_pila(&aux);
        //recuperamos la informacion de la raiz en E
        info(A,&E);
        while(!es_vacia_pila(E.transferencias)){//mientras la pila de transferencias no es vacia
            //sacamos sus elementos para leerlos y los metemos
            //en una pila auxiliar para no perderlos
            P=tope(E.transferencias);
            pop(&(E.transferencias));
            push(&aux,P);
        //si el receptor de la tranferencia recuperada coincide
        //con el nombre de usuario que recibioi la funcion  imprime la transaccion
            if(!strcmp(P.receptor,nome)) {
                printf("\nEmisor: %s\n",E.nomeUsuario);
                _printTransaccion(P);
            //aumentamos en 1 el valor de bandera
                *bandera += 1;
            }
        }
        //volvemos a meter los valores almacenados en aux en la pila principal
        _vaciarPila(&aux,&(E.transferencias));
        //modificamos el nodo E del arbol
        modificar(&A,E);
        //invocamos la funcion a la derecha y a la izquierda de A
        //para recorrer el arbol completo
        _busquedaReceptor(der(A),nome,bandera);
        _busquedaReceptor(izq(A),nome,bandera);
    }

}
//Publica mas info en .h
void consultaPorReceptor(abb A ,tipoclave usuario){
    int bandera=0;
    //ya no se pide al usuario el nombre de un receptor
    /*printf("Introduce el nombre del receptor:\t");
    scanf(" %s",usuario);*/
    //si no existe ningun usuario con ese nombre imprime un mensaje por pantalla
    if(!es_miembro_clave(A,usuario)) printf("\nNon existe ese usuario\n");
    else{//si existe dicho usuario invocamos _busquedaReceptor
        //para imprimir todas las transferencias de las que haya sido receptor el usuario
        _busquedaReceptor(A,usuario,&bandera);
        //imprimimos el numero de transferencias o un mensaje si no hay ninguna
        if(bandera) printf("\n%s recibiu un total de %d transferencias\n",usuario,bandera);
        else printf("\nNon constan transferencias recibidas a nome de %s\n",usuario);
    }
    
}

//Privada, recursiva recorre el arbol A (no lo recibe por referencia pero lo modifica)
//y un nombre de usuario, borra dicho nombre de todas las transferencias que haya recibido
//y lo sustituye por los  valores por defecto
void _eliminacionReceptor(abb A, tipoclave nome){
    tipoelem E;
    if(!es_vacio(A)){//si el arbol no es vacio
        //recuperamos en E la informacion de su raiz
        info(A,&E);
        /*crear_pila(&aux);
        while(!es_vacia_pila(E.transferencias)) {//mientras la pila transferencias no es vacia
            //sacamos sus elementos para leerlos
                P=tope(E.transferencias);
                pop(&(E.transferencias));
            if (strcmp(nome, P.receptor)) {
                //si la transaccion no se hizo al usuario a eliminar se inserta
                //en la pila aux para no perderla
                push(&aux,P);
            }
        }
        //devolvemos los elementos de aux a la pila principal
        _vaciarPila(&aux,&(E.transferencias));
        tipovertice V;
        strcpy(V.usuario,nome);
        borrar_vertice(&(E.amigos),V);
        //modificamos el arbol
        modificar(&A,E);
         */
        //borramos en el nodo E todas las refernecias al usuario a eliminar
        _borrarAmigo(&A,E.nomeUsuario,nome);
        //invocamos la funcion a la derecha y a la izquierda de A_eliminacionReceptor(der(A),nome);
        _eliminacionReceptor(izq(A),nome);
        _eliminacionReceptor(der(A),nome);
    }

}
//Publica, recibe un arbol A por referencia pues lo modifica
void eliminarUsuario(abb *A,grafo *G, tipoclave nomeUsuario){
    tipoelem E;
    //ya no se pide un nombre de usuario
    //printf("\nIntroduce el nombre del usuario a eliminar:\t");
    //scanf(" %s",E.nomeUsuario);
    //en su lugar:
    strcpy(E.nomeUsuario,nomeUsuario);
    //si no existe dicho usuario imprime un mensaje por pantalla
    if(!es_miembro_clave(*A,E.nomeUsuario)) printf("\nNon existe ese usuario\n");
    else{
        //si existe lo eliminamos en el arbol
        buscar_nodo(*A,E.nomeUsuario,&E);
        suprimir(A,E);
        tipovertice V;
        //lo eliminamos en el grafo global
        strcpy(V.usuario,nomeUsuario);
        borrar_vertice(G,V);
        //y borramos su nombre con _eliminacionRecpetor() de todas las transferencias que recibio
        _eliminacionReceptor(*A,E.nomeUsuario);
    }

}
//publica funcionaidad descrita en .h
void eliminarUsuarioAsAdmin(abb *A,grafo*G){
    char nome[LONGITUD];
    // se pide un nombre de usuario
    printf("\nIntroduce el nombre del usuario a eliminar:\t");
    scanf(" %s",nome);
    //se elimina dicho usuario
    eliminarUsuario(A,G,nome);
}
void _imprimirArchivo(abb A);

//Publica, funcion para salir del programa
void salir(abb *A,grafo*G){
    //printf("\nINORDEN:\n");
    //_inorden(*A);//ya no es necesario imprimir el arbol al salir del pograma
    _imprimirArchivo(*A);//guardamos las transferencias en un archivo de texto
    destruir_arbol(A);//liberamos la memoria asociada al arbol
    borrar_grafo(G);//liberamos la memoria asociada al grafo global
    printf("\nPayEtse desexalle un bo dia, grazas por elixirnos\n");
}
//Publica descrita en .h
void estatisticasUsuario(abb A, tipoclave nomeUsuario){
    tipoelem E;
    float transf=0;
    int count=0;
    tipoelempila max;
    pila aux,maximos;
    //ya no pedimos un nombre de usuario
    //printf("\nIntroduce un nombre de usuario:\t");
    //scanf(" %s",E.nomeUsuario);
    //en su lugar hacemos
    strcpy(E.nomeUsuario,nomeUsuario);
    //si no existe imprimimos un mensaje por pantalla
    if(!es_miembro_clave(A,E.nomeUsuario)) printf("\nNon existe ese usuario\n");
    //si existe
    else{
        //buscamos su nodo del arbol
        buscar_nodo(A,E.nomeUsuario,&E);
        //si no realizo transacciones imprimimos un mensaje por pantalla
        if(es_vacia_pila(E.transferencias)){
            printf("\nNon constan transferencias emitidas por %s\n",E.nomeUsuario);
        }
        else {
            crear_pila(&aux);
            crear_pila(&maximos);
            //si realizo transacciones inicializamos max
            max=tope(E.transferencias);
            while (!es_vacia_pila(E.transferencias)) {//mientras la pila de transferencias no es vacia
                //sacamos sus elementos para leerlos y los metemos
                //en una pila auxiliar para no perderlos
                push(&aux, tope(E.transferencias));
                pop(&(E.transferencias));
                //incrementamos la suma de dinero transferido
                transf += tope(aux).cantidad;
                //comprobamos si es candidata a ser la transferencia de mas cantidad
                if (tope(aux).cantidad > max.cantidad){ //si es la maxima
                    max=tope(aux);//la guardamos en max
                    //destruimos la pila y la volvemos a crear para que este vacia
                    destruir_pila(&maximos);
                    crear_pila(&maximos);
                    //metemos la transferencia maxima
                    push(&maximos,max);
                    //ponemos el contador a 1
                    count=1;
                }
                if(tope(aux).cantidad==max.cantidad){//si iguala a la maxima
                    max=tope(aux);
                    push(&maximos,max);//la metemos en la pila de maximas
                    count++;//aumentamos el contador
                }
            }
            //devolvemos los elementos de aux a la pila principal
            _vaciarPila(&aux,&(E.transferencias));
            //modificamos el arbol
            modificar(&A, E);
            //imprimimos la transferencia mas cuantiosa y el dinero total transferido
            printf("\nTotal dinero transferido:\t%f\n", transf);
            if(count==1) {//si el maximo es unico lo imprimimos
                printf("\nTransferencia emitida de maior cantidade:\n\t  ");
                _printTransaccion(tope(maximos));
            }
            else{//sino imprimimos todos los elementos de la pila de maximos
                printf("\nO importe maximo transferido e:\t%f\n",tope(maximos).cantidad);
                printf("Realizaronse un total de %d transferencias por este importe:\n",count);
                for(int i=0;i<count;i++){
                    printf("\t");
                    _printTransaccion(tope(maximos));
                    pop(&maximos);
                }
            }
            destruir_pila(&maximos);//destruimos la pila de maximos
        }

    }

}
//funcion publica, recibe un abb por referencia y lo modifica introduciendo
// los datos de transferencias de un fichero de texto
void leerArchivo(abb *A,grafo *G){
    FILE *doc=NULL;
    tipoelem E;
    tipoelempila elem;
    //Abrimos el documento en modo escritura
    //para que lo abriese en mi ordenador fue necesario escribir  el path completo
    //del documento a pesar de que estaba en la misma carpeta que el resto de archivos del
    //programa
    //doc=fopen("/home/jp/Documentos/PayEtse_v7/listaTransferencias.txt","rt");
    doc=fopen("listaTransferencias.txt","rt");
    if(doc!=NULL) {//si se abrio correctamente
        //leemos los datos de la primera linea
        fscanf(doc,"%s %s",E.nomeUsuario, elem.receptor);
        if(strcmp(elem.receptor,"*")) fscanf(doc, " %f %[^\r\n]",&(elem.cantidad), elem.concepto);
        while (!feof(doc)) {//mientras no se llegue al final del archivo
             _introducirDatos(A,G,E, elem);//introducimos los datos leidos en el arbol A
            //seguimos leyendo datos
            fscanf(doc,"%s %s",E.nomeUsuario, elem.receptor);
            if(strcmp(elem.receptor,"*")) fscanf(doc, " %f %[^\r\n]",&(elem.cantidad), elem.concepto);
        }
        //cerramos el archivo
        fclose(doc);
    }
}
//f privada y recursiva, imprime las transferencias de un arbol en un documento de texto
void _recorridoImprimArchivo(abb A, FILE *doc){
    tipoelem E;
    pila aux;
    if(!es_vacio(A)) {//comprobamos si  el arbol no es vacio para frenar la ejecucion recursiva
        crear_pila(&aux);//creamos una pila auxiliar
        info(A, &E);//recuperamos la informacion de la raiz
        if(num_vertices(E.amigos)<=1){//si en su grafo de amigos solo esta el mismo imprimimos su nombre y *
            fprintf(doc, "%s *\n", E.nomeUsuario);
        }
        else {
            while (!es_vacia_pila(E.transferencias)) {//mientras la pila del elemento E no es vacia
                //vaciamos sus elementos en la pila auxiliar
                push(&aux, tope(E.transferencias));
                pop(&(E.transferencias));
            }
            while (!es_vacia_pila(aux)) {//mientras la pila auxiliar no es vacia
                //imprimimos su tope en el archivo
                fprintf(doc, "%s %s %f %s\n", E.nomeUsuario, tope(aux).receptor, tope(aux).cantidad, tope(aux).concepto);
                //devolvemos el tope a la pila principal
                push(&(E.transferencias), tope(aux));
                pop(&aux);
            }
            //En esta ocasion los datos se leen al pasarlos de la pila auxiliar a la principal
            // al reves que en el resto del programa, esto es para que cuando se lea el archivo
            //se guarden las transferencias en el orden correcto en que se hicieron

            //liberamos la memoria asociada a la pila
            destruir_pila(&aux);
            //modificamos el arbol para que no se pierdan datos
            modificar(&A, E);
            //invocamos la funcion a la derecha y la izquierda de la raiz
        }
        _recorridoImprimArchivo(izq(A),doc);
        _recorridoImprimArchivo(der(A),doc);
    }
}
//Imprime los datos de transferencias de un abb
// en un archivo de texto para que se guarden para la siguiente ejecucion
void _imprimirArchivo(abb A){
    FILE *doc=NULL;
    //abrimos el archivo en modo lectura
    //para que lo abriese en mi ordenador fue necesario escribir  el path completo
    //del documento a pesar de que estaba en la misma carpeta que el resto de archivos del
    //programa
    //doc=fopen("/home/jp/Documentos/PayEtse_v7/listaTransferencias.txt","wt");
    doc=fopen("listaTransferencias.txt","wt");
    if(doc!=NULL) {//si el archivo se abrio correctamente
        //recorremos el arbol e imprimimos las transferencias
        _recorridoImprimArchivo(A, doc);
        //cerramos el archivo
        fclose(doc);
    }
}
//publica descrita en .h
void imprimirGrafo(grafo G) {
    tipovertice *VECTOR; //Para almacenar el vector de vertices del grafo
    int N; //numero de vertices del grafo

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    int i, j;
    printf("El grafo actual es:\n");
    for (i = 0; i < N; i++) {
        //Imprimo el vertice
        printf("Vertice(%d): %s\n", i, VECTOR[i].usuario);
        //Chequeo sus arcos
        for (j = 0; j < N; j++)
            if (son_adyacentes(G, i, j))
                printf("\t%s-->%s\n", VECTOR[i].usuario, VECTOR[j].usuario);
    }
}
//privada recibe un arbol A por referencia y dos nombres de usuario, borrara las
//referencias al segundo (examigo) que pueda haber en el nodo del primer usuario (usuario)
void _borrarAmigo(abb*A,char* usuario,char* examigo){
    tipoelem E;
    tipoelempila P;
    pila aux;
    strcpy(E.nomeUsuario,usuario);
    //buscamos en A el nodo con el nombre del usuario
    buscar_nodo(*A,usuario,&E);
    crear_pila(&aux);//creamos una pila auxiliar
    while(!es_vacia_pila(E.transferencias)) {//mientras la pila transferencias no es vacia
    //sacamos sus elementos para leerlos
        P=tope(E.transferencias);
        pop(&(E.transferencias));
        if (strcmp(examigo ,P.receptor)) {
        //si la transaccion no se hizo al usuario a eliminar se inserta
        //en la pila aux para no perderla
            push(&aux,P);
        }
    }
    //devolvemos los elementos de aux a la pila principal
    _vaciarPila(&aux,&(E.transferencias));
    tipovertice V;
    strcpy(V.usuario,examigo);
    //borramos el vertice examigo del grafo local de usuario
    if(existe_vertice((E.amigos),V)) borrar_vertice(&(E.amigos),V);
    //modificamos el arbol para guardar los cambios
    modificar(A,E);
}
//publica descripcion en el .h
void eliminarAmigo(abb *A, grafo *G,char* nome){
    tipoelem E;
    tipovertice V1,V2;
    strcpy(V1.usuario,nome);
    char examigo[LONGITUD];
    //buscamos el nodo que corresponde al usuario "nome"
    buscar_nodo(*A,nome,&E);
    if(num_vertices(E.amigos)<=1){//si en el grafo local solo esta el mismo se para la ejecucion de la funcion
        printf("\nAun no tienes amigos :(\n");
        return;
    }
    imprimirGrafo(E.amigos);//imprimimos el grafo local de amigos
    do {
        printf("\nIntroduce o nome do amigo que queres borrar:\t");//pedimos el nombre del amigo a borrar
        scanf("%s", examigo);
        strcpy(V2.usuario, examigo);
        if (!existe_vertice(E.amigos, V2))
            printf("\nNon tes ningun amigo que se chame %s introduce outro nome\n", examigo);
        if(!strcmp(examigo,nome))
            printf("\nNon te podes eliminar a ti mesmo\n");
    }while(!existe_vertice(E.amigos,V2)||!strcmp(examigo,nome));//solo permitimos borrar vertices que existan o no sean el del propio usuario
    //borramos la relacion de los verices en el grafo global
    borrar_arco(G,posicion(*G,V1),posicion(*G,V2));
    borrar_arco(G,posicion(*G,V2),posicion(*G,V1));

    //borramos la relacion de los usuarios en el arbol
    _borrarAmigo(A,nome,examigo);
    _borrarAmigo(A,examigo,nome);

    printf("Operacion realixada con exito, %s ya no es su amigo",examigo);
}
//recibe un grafo G y un int que representa la posicion de un vertice en el grafo g
// y calcula su grado de entrada
int _gradoEntrada(grafo G,int V){
    int grado=0;
    for(int i=0;i<num_vertices(G);i++){//recorremos el grafo y sumamos 1 a grado por cada adyacencia
        if(son_adyacentes(G,i,V)) grado++;
    }
    return grado; //devolvemos el grado
}
//recibe un grafo G y un int que representa la posicion de un vertice en el grafo G
// y calcula su grado de salida
int _gradoSalida(grafo G, int V){
    int grado=0;
    for(int i=0;i<num_vertices(G);i++){
        if(son_adyacentes(G,V,i)) grado++;//recorremos el grafo y sumamos 1 por cada adyacencia
    }
    return grado;//devolvemos el grado
}
//publica descripcion no .h
void estadisticasGlobais(grafo G) {
    float media=0;
    tipovertice V;
    int entrada=0, salida=0;
    //recorremos o grafo
    for(int i=0;i<num_vertices(G);i++){
        for(int j=0;j<num_vertices(G);j++){
            //para cada vertice comprobamos se tivo relacoin cos outros vertices
            if(son_adyacentes(G,i,j)||son_adyacentes(G,j,i)) media++;//se tivo relacion sumamos 1 a media
        }
        //ademais calculamos os grados de entrada e de salida maximos no grafo
        if(_gradoEntrada(G,i)>entrada) entrada=_gradoEntrada(G,i);
        if(_gradoSalida(G,i)>salida) salida=_gradoSalida(G,i);
    }
    media=media/(float)num_vertices(G);//calculamos a media
    //imprimimos os resultados
    printf("\n>O numero total de usuarios rexistrados e:\t%d\n",num_vertices(G));
    printf("\n>O numero medio de amigos por usuario e:\t%.2f\n",media);
    printf("\n>Usuario/s que emitiron transferencias a un maior numero de usuarios:\n");
    for(int i=0;i<num_vertices(G);i++){
        //recorremos os vertices e se o seu grado iguala ao maximo imprimimos o nome de usuario
        if(_gradoSalida(G,i)==salida) printf("--->%s\n",array_vertices(G)[i].usuario);
    }
    printf("\n>Usuario/s que  recibiron transferencias dun maior numero de usuarios:\n");
    for(int i=0;i<num_vertices(G);i++){
        //recorremos os vertices e se o seu grado iguala ao maximo imprimimos o nome de usuario
        if(_gradoEntrada(G,i)==entrada) printf("--->%s\n",array_vertices(G)[i].usuario);
    }
}
//publica descripcion en .h
void usuarioMasActivo(grafo G) {
    int actividade[num_vertices(G)],nmax=0,ni;
    for(int i =0;i<num_vertices(G);i++){//recorremos o grafo
        ni=_gradoEntrada(G,i)+_gradoSalida(G,i);//calculamos a actividade de cada vertice
        actividade[i]=ni;//a gardamos nun vector
        if(ni>nmax) nmax=ni;//se e a actividade maior ata o momento o marcamos
    }
    //imprimimos os usuarios mais activos
    printf("\n>Usuario/s mais activo/s:\n");
    for(int i=0;i<num_vertices(G);i++){//recorremos o vector coa actividade de cada usuario
        //se esa actividade iguala a actividade maxima imprimimos o nome de dito usuario
        if(nmax==actividade[i]) printf("--->%s\n",array_vertices(G)[i].usuario);
    }
}
void login(abb *A, grafo *G) {
    char nombre[LONGITUD], opcion;
    printf("Introduzca su usuario:\t");//pedimos un nome de usuario
    scanf(" %s", nombre);
    if (!strcmp(nombre, "admin")) {//se o nome e admin
        do {
            //menu de opciones administrador
            printf("\n___________________\n");
            printf("\na) Imprimir usuarios");
            printf("\nb) Imprimir grafo global");
            printf("\nc) Estadisticas globaies");
            printf("\nd) Usuario/a/os/as mais activo/a/os/as");
            printf("\ne) Dar de baixa usuario");
            printf("\nf) Sair do menu administrador");
            printf("\n___________________\n");
            printf("\nOpcion: ");
            //dejamos un espacio antes de %c para limpiar el buffer
            scanf(" %c", &opcion);
            switch (opcion) {
                case 'a':
                    inorden(*A);
                    break;
                case 'b':
                    imprimirGrafo(*G);
                    break;
                case 'c':
                    estadisticasGlobais(*G);
                    break;
                case 'd':
                    usuarioMasActivo(*G);
                    break;
                case 'e':
                    eliminarUsuarioAsAdmin(A,G);
                    break;
                case 'f':
                    break;
                default:
                    printf("Opcion incorrecta\n");
            }
        } while (opcion != 'f');//se imprime el menu en bucle mientras el usuario no slicite salir
        return;//si se solicita salir termina la ejecucion de la funcion
    }
    else {//Se o nome non e admin
        if (!es_miembro_clave(*A, nombre)) {//se non existe o usuario
            //se lle obliga a facer unha transferencia para rexistrarse
            printf("Para darse de alta no servizo debe realizar a sua primeira transferencia\n");
            introducirTransferencia(A,G,nombre);
        }
        do {
            //menu de opciones usuario
            printf("\n___________________\n");
            printf("\na) Introducir transferencia");
            printf("\nb) Consultar transferencias emitidas");
            printf("\nc) Consultar transferencias recibidas");
            printf("\nd) Estadisticas de usuario");
            printf("\ne) Darse de baixa no servixo");
            printf("\nf) Borrar amigo");
            printf("\ng) Sair do menu usuario");
            printf("\n___________________\n");
            printf("\nOpcion: ");
            //dejamos un espacio antes de %c para limpiar el buffer
            scanf(" %c", &opcion);
            switch (opcion) {
                case 'a':
                    introducirTransferencia(A,G,nombre);
                    break;
                case 'b':
                    consultaPorUsuario(*A,nombre);
                    break;
                case 'c':
                    consultaPorReceptor(*A,nombre);
                    break;
                case 'd':
                    estatisticasUsuario(*A,nombre);
                    break;
                case 'e':
                    eliminarUsuario(A,G,nombre);
                    //se se da de baixa sae automaticamente do menu usuario
                    return;
                case 'f':
                    eliminarAmigo(A,G,nombre);
                case 'g':
                    break;
                default:
                    printf("Opcion incorrecta\n");
            }
        } while (opcion != 'g');//se imprime el menu en bucle mientras el usuario no slicite salir
        return;//si se solicita salir termina la ejecucion de la funcion
    }
}
