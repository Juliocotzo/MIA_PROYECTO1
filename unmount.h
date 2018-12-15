#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>

#define RED   "\x1B[1;31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[1;36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct unmount{
    char id[15];
} um_init;

void desmontar(char id[]);

int processUNMOUNT(char str[], int cont){
    if(cont == -1){

        return 0;
    }
    um_init m;
    memset(&m.id,'\0',sizeof(m.id));
    // Base Program
    char ch, buffer[100];
    memset(&buffer,'\0',sizeof(buffer));
    int j=0;
    bool read = true;
    char name[10];
    memset(&name,'\0',sizeof(name));
    while((ch = str[cont++]) != '\0'){
        if(ch == '-' && read){
            buffer[j] = '\0';
            memset(&name,'\0',sizeof(name));
            strcat(name, buffer);
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
        if(ch == '\"' || ch == '\”'){
            read = !read;
        }
        if(isalnum(ch) || ch == '/' || ch == '.' || ch == '_'){
            buffer[j++] = ch;
        }
        if(ch == '#'){
            break;
        }
        if(ch == ' ' && !read){
            buffer[j++] = ch;
        }else if((ch == ' ' || ch == '\n') && (j != 0) && read){
            buffer[j] = '\0';
            // Posible optimizacion
            if(strcasecmp("ID", name) == 0){
                strcat(m.id, buffer);
            }else{
                printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }

    }
    if((j != 0) && read){
        buffer[j] = '\0';
        // Posible optimizacion
        if(strcasecmp("ID", name) == 0){
            strcat(m.id, buffer);
        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }
    //printf("\n");
    printf("IDDDD---->%s\n",m.id);
    desmontar(m.id);
    return 0;
}

void desmontar(char id[]){
    printf("\nDESMONTAR PARTICIONES\n");
    struct nodo *actual;
    char aux[100];
    int p = 1;
    int bandera = 0;
    actual = primero;


    struct nodo *aux2;
    struct nodo *actual2;
    int nodo_eliminar;
    strcpy(aux,id);
    printf("IDDDD---->%s\n",id);
    while (p<=numNodos) {
        if (strcmp(aux,actual->id)==0) {
            nodo_eliminar=p;
            bandera=1;
        }
        p++;
        actual=actual->siguiente;
    }
    if (bandera==1) {
        if (nodo_eliminar>numNodos || nodo_eliminar<1) {
            printf("\nERROR: No existe esa particion\n");
        }
        if (nodo_eliminar==1) {
            aux2=primero;
            primero=primero->siguiente;
            if (primero==NULL) {
                ultimo=NULL;
            }else {
                primero->anterior=NULL;
            }
        }else if (nodo_eliminar==numNodos) {
            aux2=ultimo;
            ultimo->anterior->siguiente=NULL;
            ultimo=ultimo->anterior;
        }else {
            actual2=primero;
            int var;
            for (var = 1; var < nodo_eliminar; ++var) {
                actual2=actual2->siguiente;
            }
            aux2=actual2;
            actual2->anterior->siguiente = actual2->siguiente;
            actual2->siguiente->anterior = actual2->anterior;
        }
        free(aux2);
        numNodos--;
    }else {
        printf("\nERROR: No existe particion montada con ese id");
    }
}

