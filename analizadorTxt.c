#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tda_set/set.h"

//Aqui nomas cree una funcion especial para el menu xd
void print_menu() 
{
    printf("\n--- Analizador de textos ---\n");
    printf("\n");
    printf("1. Palabras unicas del archivo 1\n");
    printf("2. Palabras únicas del archivo 2\n");
    printf("3. Palabras comunes a ambos archivos\n");
    printf("4. Cantidad Total de palabras distintas\n");
    printf("5. Palabra comun más larga\n");
    printf("6. Busqueda de una palabra\n");
    printf("7. Salir\n");
    printf("\n");
    printf("Dame una opcion: ");
}

//Aqui esta funcion imprimo los elementos de un set 
void print_sorted_set(const set *set) 
{
    //Checo si el set estta null o esta vacio
    if (!set || set->tamanio == 0) 
    {
        printf("(vacio)\n");
        return;
    }
    
    //Convertir set a array, esto principalmente parar poder ordenralo
    //el setToArray me retorna un array con todos los elementos
    void **array = setToArray(set);
    if (!array) 
    {
        //Checo si se puedo convertir - debugg
        printf("Error al convertir set a array\n");
        return;
    }
    
    //Ordeno el array
    //Le hago un cast a char** porque son strings
    sortStringArr((char**)array, set->tamanio);
    
    //Impprimo palabras ordenadas
    for (int i = 0; i < set->tamanio; i++) 
    {
        printf("%s ", (char*)array[i]); //Imprimo cada palabra
    }
    printf("\n");
    
    free(array);
}

//Aqui en esta funcion es para buscar una palabra que este en ambos archivos
void search_word_in_sets(const set *setA, const set *setB) 
{
    char word[100]; //Esto solo para almacenar la palabra

    printf("Ingrese la palabra a buscar: ");
    //Aqui puedo leer 99 caracteres, mas que todo para no desbordar el buffer
    scanf("%99s", word);
    
    //Normalizo la palabra que me paso el usuario 
    char *normalized = normalizarPalabra(word);
    if (!normalized) 
    {
        printf("Palabra no valida\n");
        return;
    }
    
    //Seguiria buscar en ambos set´s
    //set_contains retorna 1 si encuentra la palabra, 0 si no
    int inA = set_contains(setA, normalized);
    int inB = set_contains(setB, normalized);
    
    if (inA && inB) {
        printf("Aparece en ambos archivos\n");
    } else if (inA) {
        printf("Aparece solo en el archivo 1\n");
    } else if (inB) {
        printf("Aparece solo en el archivo 2\n");
    } else {
        printf("No aparece en ningun archivo\n");
    }
    
    free(normalized);
}

//El mero mero
int main() 
{
    //Aqui cargo los archivos
    //leeo el archivo y noramliza las palabras alamcenadas en el set <- todo con el read_fileToSet
    set *setA = read_fileToSet("texto1.txt");
    set *setB = read_fileToSet("texto2.txt");
    
    //Checo si, si se cargaron los archivos - debugg
    if (!setA || !setB) 
    {
        printf("Error al leer los archivos\n");
        if (setA) set_destroy(setA, stringDestroy);
        if (setB) set_destroy(setB, stringDestroy);
        return 1;
    }
    
    int opcion;
    do {
        print_menu(); //Imprimo el menu
        scanf("%d", &opcion); //Leo la entrada de la terminal
        
        switch (opcion) 
        {
            case 1:
            {
                printf("\nPalabras unicas del archivo 1:\n");
                set *diff = set_difference(setA, setB); //Calculo la diferencia
                print_sorted_set(diff); //Imprio el resultado ordenado
                set_destroy(diff, NULL); //Libero el set
                break;
            }

            case 2: 
            {
                printf("\nPalabras unicas del archivo 2:\n");
                set *diff = set_difference(setB, setA); //Aqui vuelvo a calcular la diferencia perro ahora inversa
                print_sorted_set(diff); //Imprimo el res
                set_destroy(diff, NULL); //Libero el set
                break;
            }

            case 3: 
            {
                printf("\nPalabras comunes a ambos archivos:\n");
                set *intersection = set_intersection(setA, setB); //Calculo la interseccion
                print_sorted_set(intersection);
                set_destroy(intersection, NULL);
                break;
            }
            
            case 4: 
            {
                //Calculo al union de A y B, txt1 y txt2
                set *union_set = set_union(setA, setB);
                printf("\nCantidad total de palabras distintas: %d\n", set_size(union_set));
                set_destroy(union_set, NULL);
                break;
            }
            
            case 5: 
            {
                printf("\nPalabra común más larga:\n");
                set *union_set = set_union(setA, setB); //Aqui obtengo las palabras 
                char *longest = encontrarWordLargest(union_set); //Y pues por la funcion encuentro la palabra mas larga
                if(longest) 
                {
                    printf("La palabra comun mas larga es: %s\n", longest);
                } 
                else 
                {
                    printf("No hay palabras comunes entre los archivos\n");
                }
                set_destroy(union_set, NULL);
                break;
            }
            
            case 6:
            { 
                search_word_in_sets(setA, setB); //Aqui solo busco una palabra que esete en los dos textos
                break;
            }

            case 7: 
            {
                //Ya se acaboo
                printf("Byeeee...\n");
                break;
            }
            
            default: 
            {
                printf("Opción no valida - Intenta de nuevo\n");
                break;
            }
        }
    }while (opcion != 7); //Y ps aqui continuo hasta que el user me de un 7
    
    //Libero la memoria
    //Pense que podria ser buena idea liberar la memoria de cada set, junto a sus palabras
    set_destroy(setA, stringDestroy); //setA
    set_destroy(setB, stringDestroy); //setB
    
    return 0;
}