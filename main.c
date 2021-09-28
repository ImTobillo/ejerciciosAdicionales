#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// structures

typedef struct
{
    int nroParcial;
    char dni[9];
    char asignatura[20];
    float Nota;
}Examen;

typedef struct nodoEx
{
    Examen dato;
    struct nodoEx* ant;
    struct nodoEx* sig;
}nodoEx;

typedef struct nodo
{
    int dato;
    struct nodo* sig;
}nodo;

/// functions

void inicLista (nodo** lista)
{
    *lista = NULL;
}

nodo* crearNodo (int dato)
{
    nodo* aux = (nodo*)malloc(sizeof(nodo));
    aux->dato = dato;
    aux->sig = NULL;
    return aux;
}

void mostrarLista (nodo* lista)
{
    while (lista)
    {
        printf("%i\t", lista->dato);
        lista = lista->sig;
    }
}

void agregarAlFinal (nodo** lista, nodo* nuevo)
{
    if (*lista)
    {
        nodo* aux = *lista;

        while (aux->sig)
            aux = aux->sig;

        aux->sig = nuevo;
    }
    else
        *lista = nuevo;
}

void eliminarRepetidosRec (nodo** lista, int dato)
{
    if (*lista && (*lista)->dato == dato)
    {
        nodo* aux = *lista;

        *lista = (*lista)->sig;

        free(aux);

        eliminarRepetidosRec(lista, dato);
    }
}

void eliminarRepetidos (nodo** lista)
{
    nodo* aux = *lista;

    while (aux)
    {
        eliminarRepetidosRec(&aux->sig, aux->dato);
        aux = aux->sig;
    }
}

void intercalarOrdenado (nodo** listaA, nodo** listaB, nodo** listaInter)
{
    nodo* aux;

    while (*listaA && *listaB)
    {
        if ((*listaA)->dato < (*listaB)->dato)
        {
            aux = *listaA;
            *listaA = (*listaA)->sig;
            aux->sig = NULL;

            agregarAlFinal(listaInter, aux);
        }
        else
        {
            aux = *listaB;
            *listaB = (*listaB)->sig;
            aux->sig = NULL;

            agregarAlFinal(listaInter, aux);
        }
    }

    if (*listaA)
        agregarAlFinal(listaInter, *listaA);
    else
        agregarAlFinal(listaInter, *listaB);
}

// lista alumnos

void inicListaEx (nodoEx** lista)
{
    *lista = NULL;
}

Examen cargarExamen ()
{
    Examen ex;
    printf("Ingrese nro parcial: ");
    scanf("%i", &ex.nroParcial);
    fflush(stdin);
    printf("Ingrese dni: ");
    scanf("%s", ex.dni);
    fflush(stdin);
    printf("Ingrese asignatura: ");
    scanf("%s", ex.asignatura);
    printf("Ingrese nota: ");
    scanf("%f", &ex.Nota);
    return ex;
}

nodoEx* crearNodoEx (Examen ex)
{
    nodoEx* aux = (nodoEx*)malloc(sizeof(nodoEx));
    aux->dato = ex;
    aux->ant = NULL;
    aux->sig = NULL;
    return aux;
}

void mostrarExamen (Examen ex)
{
    printf("NUMERO PARCIAL: %i\n", ex.nroParcial);
    printf("DNI: %s\n", ex.dni);
    printf("ASIGNATURA: %s\n", ex.asignatura);
    printf("NOTA: %.2f\n", ex.Nota);
}

void mostrarListaEx (nodoEx* lista)
{
    while (lista)
    {
        mostrarExamen(lista->dato);
        lista = lista->sig;
    }
}

void insertarDNIOrden (nodoEx** lista, nodoEx* nuevo)
{
    if (*lista)
    {
        if (strcmpi((*lista)->dato.dni, nuevo->dato.dni) > 0)
        {
            nuevo->sig = *lista;
            (*lista)->ant = nuevo;
            *lista = nuevo;
        }
        else
        {
            nodoEx* ant = *lista, *act = (*lista)->sig;

            while (act && strcmpi(act->dato.dni, nuevo->dato.dni) < 0)
            {
                ant = act;
                act = act->sig;
            }

            ant->sig = nuevo;
            nuevo->ant = ant;

            nuevo->sig = act;
            if (act)
                act->ant = nuevo;
        }
    }
    else
        *lista = nuevo;
}

void borrarNodoDNI (nodoEx** lista, char* dni)
{
    if (*lista)
    {
        if (strcmpi((*lista)->dato.dni, dni) == 0)
        {
            nodoEx* aux = *lista;

            *lista = (*lista)->sig;

            if (*lista)
                (*lista)->ant = aux->ant;

            if (aux->ant)
                aux->ant->sig = *lista;

            free(aux);

            borrarNodoDNI(lista, dni);
        }
        else
            borrarNodoDNI(&(*lista)->sig, dni);
    }
}

void informarPromedioDetMat (nodoEx* listaEx, char* asig)
{
    float suma = 0, cant = 0;

    while (listaEx)
    {
        if (strcmpi(listaEx->dato.asignatura, asig) == 0)
        {
            suma += listaEx->dato.Nota;
            cant++;
        }

        listaEx = listaEx->sig;
    }

    printf("\nEl promedio de notas en %s, es: %.2f", asig, suma/cant);
}

int verificarDniEnAsig (nodoEx* lista, char* dni, char* asig)
{
    if (lista)
    {
        if (strcmpi(lista->dato.dni, dni) == 0 && strcmpi(lista->dato.asignatura, asig) == 0)
            return 1;
        else
            return verificarDniEnAsig(lista->sig, dni, asig);
    }
    else
        return 0;
}

void mostrarListaExRec (nodoEx* listaEx)
{
    if (listaEx)
    {
        mostrarExamen(listaEx->dato);
        mostrarListaExRec(listaEx->sig);
    }
}

int retCantAlumnosDetMat (nodoEx* listaEx, char* asig)
{
    if (listaEx)
    {
        if (strcmpi(listaEx->dato.asignatura, asig) == 0)
            return 1 + retCantAlumnosDetMat(listaEx->sig, asig);
        else
            return retCantAlumnosDetMat(listaEx->sig, asig);
    }
    else
        return 0;
}

void pasarDnisA_ArregloDinamico (char*** arregloDnis, nodoEx* listaEx)
{
    arregloDnis = malloc(sizeof(char*9*retCantAlumnosDetMat(listaEx, "mates")));
}

/// main

int main()
{
    /*nodo* lista;
    inicLista(&lista);

    agregarAlFinal(&lista, crearNodo(1));
    agregarAlFinal(&lista, crearNodo(1));
    agregarAlFinal(&lista, crearNodo(3));
    agregarAlFinal(&lista, crearNodo(3));
    agregarAlFinal(&lista, crearNodo(4));
    agregarAlFinal(&lista, crearNodo(5));
    agregarAlFinal(&lista, crearNodo(6));

    printf("LISTA CON VALORES REPETIDOS SIN BORRAR\n");

    mostrarLista(lista);

    eliminarRepetidos(&lista);

    printf("\n\nLISTA CON VALORES REPETIDOS BORRADOS\n");

    mostrarLista(lista);*/


    /*nodo* listaA, *listaB, *listaInter;
    inicLista(&listaA);
    inicLista(&listaB);
    inicLista(&listaInter);

    agregarAlFinal(&listaA, crearNodo(1));
    agregarAlFinal(&listaA, crearNodo(3));
    agregarAlFinal(&listaA, crearNodo(4));
    agregarAlFinal(&listaA, crearNodo(6));

    agregarAlFinal(&listaB, crearNodo(2));
    agregarAlFinal(&listaB, crearNodo(5));
    agregarAlFinal(&listaB, crearNodo(7));
    agregarAlFinal(&listaB, crearNodo(8));

    printf("LISTA A\n");

    mostrarLista(listaA);

    printf("\n\nLISTA B\n");

    mostrarLista(listaB);

    intercalarOrdenado(&listaA, &listaB, &listaInter);

    printf("\n\nLISTA INTERCALADA\n");

    mostrarLista(listaInter);*/

    nodoEx* listaEx;
    inicListaEx(&listaEx);

    insertarDNIOrden(&listaEx, crearNodoEx(cargarExamen()));
    insertarDNIOrden(&listaEx, crearNodoEx(cargarExamen()));
    //insertarDNIOrden(&listaEx, crearNodoEx(cargarExamen()));
    //insertarDNIOrden(&listaEx, crearNodoEx(cargarExamen()));


    //mostrarListaEx(listaEx);
    mostrarListaExRec(listaEx);


    //borrarNodoDNI(&listaEx, "44562987");

    //mostrarListaEx(listaEx);

    //informarPromedioDetMat(listaEx, "mates");

    //printf("\nEXISTE DNI %s EN %s? %i\n", "44562987", "mates", verificarDniEnAsig(listaEx, "44562987", "mates"));

    //printf("\nCANTIDAD DE ALUMNOS EN %s: %i", "mates", retCantAlumnosDetMat(listaEx, "mates"));

    char** arregloDNIs;



    return 0;
}
