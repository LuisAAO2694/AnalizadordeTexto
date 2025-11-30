#ifndef SET_H
#define SET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct setNodo
{
    void *data;
    struct setNodo *next;
}setNodo;

typedef struct set
{
    setNodo *cabeza;

    int (*compare)(const void*, const void*);
    int tamanio;
}set;

//--- Las funciones del Set ---
set* set_create(int(*comparador)(const void*, const void*));
int set_add(set *set, void *elemento);
int set_remove(set *set, void *elemento);
int set_contains(const set * set, const void *elemento);
int set_size(const set *set);
void set_iterate(set *set, void (*callback)(void*));
void set_destroy(set *set, void (*destructor)(void*));
set* set_union(const set *set1, const set *set2);
set* set_intersection(const set *set1, const set *set2);
set* set_difference(const set *set1, const set *set2);
void** setToArray(const set *set);

//--- Funciones para el analizador del texto ---
//Con esta comparo las strings
int stringComparar(const void *a, const void *b);

//Destructor para las strings
void stringDestroy(void *data);

//Normalizamos el etxto (osea todo lo hacemos minusculas)
char* normalizarPalabra(const char *palabra);

//Leemos el archivo y creamos el set de las palabras
set* read_fileToSet(const char *filename);

//Ordenamos el array de letras(strings)
void sortStringArr(char **array, int size);

//Trato de encontrar la palaara mas larga que este en el set
char* encontrarWordLargest(const set *set);

#endif