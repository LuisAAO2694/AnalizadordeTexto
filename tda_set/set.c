#include "set.h"

//--- Set ---

//Creo un set vacio
/*
    Mi approacch seria
    Le asigno la memeoria al set 
    incializo los punteros a null
    Asigno la comparacion 
    y pongo todo en 0
*/
set* set_create(int (*comparador)(const void*, const void*))
{
    //Aqui asigno la memoria 
    set *Nuevoset = (set*)malloc(sizeof(set));

    //Checo si se asigno bien la memoria 
    if(!Nuevoset)
    {
        return NULL;
    }

    //Y pues inicializo los datos
    Nuevoset->cabeza = NULL; //La lista vacia
    Nuevoset->compare = comparador; //Aqui comparo los elementos
    Nuevoset->tamanio = 0; //Tamaño de 0 al inicio

    //Retorno el nuevo set creado
    return Nuevoset; 
}

//Aqui agrego un elemento al set si no existe
int set_add(set *set, void *elemento)
{
    //Checo si el set y el elemento sea valido
    if(!set || !elemento)
    {
        return 0;
    }

    //Checar si ya existe
    if(set_contains(set, elemento))
    {
        //Ps si ya existe, no lo agrego
        return 0;
    }

    //Creo otro nodo para guardar el elementto 
    setNodo *newNodo = (setNodo*)malloc(sizeof(setNodo));
    if(!newNodo) //Me sirvio para el debugg
    {
        return 0;
    }

    //Almaceno el dato
    newNodo->data = elemento;
    //Apunto al actual de mi primer nodo 
    newNodo->next = set->cabeza;

    //Aqui el nuevo nodo es mi cabeza
    set->cabeza = newNodo;
    set->tamanio++; //incremento el contador

    return 1;
}

//Elimino un elemento del set
int set_remove(set *set, void *elemento)
{
    //Primero checo los parametros y que no estte vacio mi set
    if (!set || !elemento || set->tamanio == 0) 
    {
        return 0;
    }
    
    //Mis punteros para poder recorrer la lista 
    setNodo *current = set->cabeza; //act
    setNodo *prev = NULL; //ant
    
    //Aqui en este while reccorro elemento por elemento
    while (current) 
    {
        //Comparo el act por el que estoy buscando
        if (set->compare(current->data, elemento) == 0) 
        {
            //Lo encontre, reorganizo los punteros
            if (prev) 
            {
                //Elemento en medio o final, me lo salto
                prev->next = current->next;
            } 
            else 
            {
                //Elemento al inicio, actualizo la cabeza
                set->cabeza = current->next;
            }

            free(current);
            set->tamanio--; //Decremento el cont
            return 1;
        }

        //Avanzo al siguiente nodo 
        prev = current;
        current = current->next;
    }
    return 0;
}

//Aqui nomas checo si el elemento estta en el set
int set_contains(const set *set, const void *elemento) 
{
    if (!set || !elemento) 
    {
        return 0;
    }
    
    //Recorro la lista desde la cabeza
    setNodo *current = set->cabeza;
    while (current) 
    {
        if (set->compare(current->data, elemento) == 0) 
        {
            return 1;
        }

        current = current->next;
    }
    return 0;
}

//Le puse size a la funcion, porque tiene tamanio el en el .h
//Obtengo la cantidad de elementos en el set
int set_size(const set *set)
{
    return set ? set->tamanio : 0;
}

//Checo cada elemento y veo si estan duplicados
void set_iterate(set *set, void (*callback)(void*)) 
{
    if (!set || !callback) 
    {
        return;
    }
    
    setNodo *current = set->cabeza;
    
    while (current) 
    {
        callback(current->data);
        current = current->next;
    }
}

void set_destroy(set *set, void (*destructor)(void*)) 
{
    if (!set) 
    {
        return;
    }
    
    setNodo *current = set->cabeza;
    
    while (current)
    {
        setNodo *next = current->next;
        
        if (destructor)
        {
            destructor(current->data);
        }

        free(current);
        current = next;
    }

    free(set);
}

set* set_union(const set *set1, const set *set2)
{
    if (!set1 || !set2) 
    {
        return NULL;
    }
    
    set *result = set_create(set1->compare);

    if (!result) return NULL;
    
    //Agregar todos los elementos del primer set
    setNodo *current = set1->cabeza;
    while (current) 
    {
        set_add(result, current->data);
        current = current->next;
    }

    //Agregar elementos del segundo set que no esten en el primero
    current = set2->cabeza;
    
    while (current) 
    {
        if (!set_contains(set1, current->data)) 
        {
            set_add(result, current->data);
        }

        current = current->next;
    }
    
    return result;
}


set* set_intersection(const set *set1, const set *set2) 
{
    if (!set1 || !set2) 
    {
        return NULL;
    }
    
    set *result = set_create(set1->compare);
    if (!result) 
    {
        return NULL;
    }
    
    setNodo *current = set1->cabeza;
    while (current)
    {
        if (set_contains(set2, current->data))
        {
            set_add(result, current->data);
        }

        current = current->next;
    }
    
    return result;
}

set* set_difference(const set *set1, const set *set2) 
{
    if (!set1 || !set2) 
    {
        return NULL;
    }
    
    set *result = set_create(set1->compare);
    if (!result) 
    {
        return NULL;
    }
    
    setNodo *current = set1->cabeza;
    
    while (current) 
    {
        if (!set_contains(set2, current->data)) 
        {
            set_add(result, current->data);
        }
        
        current = current->next;
    }
    
    return result;
}

//Aqui convierto el set a un array
void** setToArray(const set *set) 
{
    //Checo si el set existe y hay elementos en el 
    if (!set || set->tamanio == 0)
    {
        return NULL; //Ps no se puede crerar el array
    } 
    
    //Le asigno memoria parra el array de punteros 
    void **array = (void**)malloc(set->tamanio * sizeof(void*));
    if (!array) 
    {
        return NULL;
    }
    
    //Recorro el set y copio los elementos al array 
    setNodo *current = set->cabeza;
    int i = 0;

    while (current && i < set->tamanio) 
    {
        array[i++] = current->data; //Copio el elemento del array
        current = current->next; //Avanzo al siguiente nodo 
    }
    
    //Rettorno el array que se creo 
    return array;
}


//--- Aqui pongo algunas funciones para la parte del texto

//Comparo los dos strings para usar el set
int stringComparar(const void *a, const void *b)
{
    //strcmp <- lo de la clase de repaso
    return strcmp((const char *)a, (const char*)b);
    /*
        Retorna < 0 si a < b
        Retorna 0 si a == b  
        Retorna > 0 si a > b
    */
}

//Aqui libero memoria del string
//Es mi destructor
void stringDestroy(void *data) 
{
    free(data);
}

//Normalizo una palabra para revisar el texto
char* normalizarPalabra(const char *word) 
{
    //Primero checo que la palabra no se null xd
    if (!word) 
    {
        return NULL;
    }
    
    //Duplico la palabra original
    char *normalized = strdup(word);
    if (!normalized) 
    {
        return NULL;
    }
    
    //Convertir a minusculas y eliminar las comas, puntos etc
    int ay, by = 0;

    for (ay = 0; normalized[ay]; ay++) 
    {
        //Aqui checo si es caracter alfabetico
        if (isalpha((unsigned char)normalized[ay])) 
        {
            //Y pues aqui solo conservo los caracteres albeticos en minuscula
            normalized[by++] = tolower((unsigned char)normalized[ay]);
        }
    }

    //Aqui normalizo el string, osea lo termino
    normalized[by] = '\0';
    
    //Si despues de normalizar queda vacio, libero memoria
    if (strlen(normalized) == 0) 
    {
        free(normalized);
        return NULL;
    }
    
    return normalized;
}

//Aqui leo el archivo de texto y creo un set de palabras unicas
//Reutilice codigo de proyecto2 xd
set* read_fileToSet(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("No se pudo abrir el archivo %s\n", filename);
        return NULL;
    }
    
    set *wordSet = set_create(stringComparar);
    if (!wordSet) 
    {
        fclose(file);
        return NULL;
    }
    
    char buffer[256];

    while (fscanf(file, "%255s", buffer) == 1) 
    {
        char *normalized = normalizarPalabra(buffer);
        if (normalized)
        {
            if (!set_add(wordSet, normalized)) 
            {
                //Si la palabra ya existe, libero la memoria
                free(normalized);
            }
        }
    }
    
    fclose(file);

    return wordSet;
}

//Aqui en esta. funcion ordeno el array de strings
//Bubble sort better
void sortStringArr(char **array, int size) 
{
    //Checo que el array ecist, ademas de ver que tenga mas de 1 elemento
    if (!array || size <= 1) 
    { 
        return;
    }
    
    //Aqui. empizo el bubble sort
    for(int i = 0; i < size - 1; i++) 
    {
        for(int j = i + 1; j < size; j++) 
        {
            //Comparo los strings
            if (strcmp(array[i], array[j]) > 0) 
            {
                //Aqui inttercambio strings si estan orden incorrecto
                char *temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

//Aqui, aunque su nombre lo dice, busco la palabra mas larga en los dos archivos
char* encontrarWordLargest(const set *set) {
    //Checoo que los param sean validos 
    if (!set || set->tamanio == 0) 
    { 
        return NULL;
    }
    
    //Varaibles que voy a trackear 
    char *maslargaw = NULL; //La palabra mas larga encontrada
    size_t maxLength = 0; //Long de la palabra mas larga
    
    //Aqui recorro todos los elementos del set 
    setNodo *current = set->cabeza;
    while(current) 
    {
        //Obtengo la palabra actual
        char *word = (char*)current->data;
        size_t length = strlen(word); //Calculo la longitud
        
        //Checo si es mas larga que la actual maxima
        if(length > maxLength) 
        {
            maxLength = length; //Act la la long max
            maslargaw = word; //Actualizo la palabra mas larga
        }
        current = current->next; //Avanzo al siguiente nodo
    }
    
    return maslargaw;
}