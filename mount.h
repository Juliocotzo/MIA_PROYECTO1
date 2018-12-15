#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
//#include "estructuras.h"

#define RED   "\x1B[1;31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[1;36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/*const char PARTITION_STATUS_NOT_ACTIVE = 'F';
const char PARTITION_STATUS_ACTIVE = 'T';
const char PARTITION_TYPE_PRIMARY = 'P';
const char PARTITION_TYPE_EXTENDED = 'E';*/

nodo *primero=NULL;
nodo *ultimo=NULL;
int numNodos=0;
int posicion;
int discos = 1;
int discoReturn;

typedef struct mount{
    char path[80];
    char name[25];
} m_init;

void mount(char nombre[], char path[]);
void montar(char nombre[], char path[]);
void particionesMontadas();
void particionesMontadas2(char path[]);
int verificarPaticionMontada(char nombre[], char path[]);
int buscarParticionesMontadas(char path[]);
int buscarDisco(char path[]);
int retornarNumDisco(char path[]);



void mount(char nombre[], char path[]){
    printf("\nMONTAR PARTICION");
    FILE * file;
    MBR disco;
    int extendida=-1;

    file = fopen(path,"rb");
    if(file==NULL){
        printf("\nERROR: El disco %s no existe",path);
        return 1;
    }
    fread(&disco,sizeof(MBR),1,file);
    fclose(file);

    int aux = verificarPaticionMontada(nombre,path);
    if(aux==1){
        printf("\nERROR: Esta particion ya se encuentra montada");
        return 1;
    }

    int i;
    for(i=0;i<4;++i){
        //printf("\nNOMBRE1: %s",disco.mbr_particion[i].part_name);
        //printf("\nNOMBRE2: %s",nombre);
        if(strcmp(disco.mbr_partition[i].part_name,nombre)==0){
            break;
        }
        if(disco.mbr_partition[i].part_type=='e'){
            extendida=i;
        }
    }
    //printf("\nI: %d",i);
    int bandera=0;
    if(i>=4){
        //printf("\nAQUI0");
        if(extendida==-1){
            //printf("\nAQUI1");
            printf("\n1.ERROR: La particion con este nombre no existe dentro del disco %s",path);
            return 1;
        }else{
            //printf("\nAQUI2");
            file = fopen(path,"rb");
            fread(&disco,sizeof(MBR),1,file);
            int posicion = disco.mbr_partition[extendida].part_start;
            EBR_P part;
            do{
                fseek(file,posicion,SEEK_SET);
                fread(&part,sizeof(EBR_P),1,file);
                if(part.part_status==PARTITION_STATUS_ACTIVE){
                    if(strcmp(part.part_name,nombre)==0){
                        bandera=1;
                        break;
                    }
                }
                posicion=part.part_next;
            }while(part.part_next!=-1);
            fclose(file);
        }
    }else{
        printf("\n3.Se monto la partion\n");
        montar(nombre,path);
        return 1;
    }
    if(bandera==1){
        printf("\n4.Se monto la particion\n");
        montar(nombre,path);
    }else{
        //printf("\nAQUI5");
        printf("\n5.ERROR: La particion con este nombre no existe dentro del disco %s **************",path);
        return 1;
    }
}

void montar(char nombre[], char path[]){

    struct nodo *actual;
    char aux[100];
    int p = 1;
    int bandera = 0;
    int bandera2 =0;
    actual = primero;
    int letra2;

    struct nodo *nuevo;
    nuevo = (nodo*)malloc(sizeof(nodo));
    if (nuevo==NULL) {
        printf("\nERROR: Memoria insuficiente\n");
    }else {

        strcpy(nuevo->nombre,nombre);
        strcpy(nuevo->path,path);
        if (primero==NULL) {

            strcpy(nuevo->letra,"a");

            strcpy(nuevo->numero,"1");
            strcpy(nuevo->id,"vda1");
            nuevo->numdiscos=1;

            nuevo->anterior=NULL;
            nuevo->siguiente=NULL;
            primero=nuevo;
            ultimo=nuevo;
        }else {
            if (buscarDisco(path)==1) {
                printf("\nDISCO ENCONTRADO\n");
                discoReturn = retornarNumDisco(path);
                bandera = 1;
            }else if (buscarDisco(path)==0) {
                printf("\nDisco NO ENCONTRADO\n");
                discos++;
                printf("\nDisco%d",discos);
                bandera = 0;
            }

            int p2=0;
                actual=primero;
                int var;
                for (var = 0; var < numNodos; ++var) {
                    if (strcmp(aux,actual->path)==0) {
                        p2++;
                    }
                }
                if (buscarParticionesMontadas(path)==0) {
                    strcpy(nuevo->numero,"1");
                }else if (buscarParticionesMontadas(path)==1) {
                    strcpy(nuevo->numero,"2");
                }else if (buscarParticionesMontadas(path)==2) {
                    strcpy(nuevo->numero,"3");
                }else if (buscarParticionesMontadas(path)==3) {
                    strcpy(nuevo->numero,"4");
                }else if (buscarParticionesMontadas(path)==4) {
                    strcpy(nuevo->numero,"5");
                }else if (buscarParticionesMontadas(path)==5) {
                    strcpy(nuevo->numero,"6");
                }else if (buscarParticionesMontadas(path)==6) {
                    strcpy(nuevo->numero,"7");
                }else if (buscarParticionesMontadas(path)==7) {
                    strcpy(nuevo->numero,"8");
                }else if (buscarParticionesMontadas(path)==8) {
                    strcpy(nuevo->numero,"9");
                }else if (buscarParticionesMontadas(path)==9) {
                    strcpy(nuevo->numero,"10");
                }else if (buscarParticionesMontadas(path)==10) {
                    strcpy(nuevo->numero,"11");
                }else if (buscarParticionesMontadas(path)==11) {
                    strcpy(nuevo->numero,"12");
                }else if (buscarParticionesMontadas(path)==12) {
                    strcpy(nuevo->numero,"13");
                }else if (buscarParticionesMontadas(path)==13) {
                    strcpy(nuevo->numero,"14");
                }else if (buscarParticionesMontadas(path)==14) {
                    strcpy(nuevo->numero,"15");
                }else if (buscarParticionesMontadas(path)==15) {
                    strcpy(nuevo->numero,"16");
                }else if (buscarParticionesMontadas(path)==16) {
                    strcpy(nuevo->numero,"17");
                }else if (buscarParticionesMontadas(path)==17) {
                    strcpy(nuevo->numero,"18");
                }

                if(bandera ==0){
                    if (discos==1) {
                        strcpy(nuevo->letra,"a");
                        nuevo->numdiscos=1;
                    }else if (discos==2) {
                        strcpy(nuevo->letra,"b");
                        nuevo->numdiscos=2;
                    }else if (discos==3) {
                        strcpy(nuevo->letra,"c");
                        nuevo->numdiscos=3;
                    }else if (discos==4) {
                        strcpy(nuevo->letra,"d");
                        nuevo->numdiscos=4;
                    }else if (discos==5) {
                        strcpy(nuevo->letra,"e");
                        nuevo->numdiscos=5;
                    }else if (discos==6) {
                        strcpy(nuevo->letra,"f");
                        nuevo->numdiscos=6;
                    }else if (discos==7) {
                        strcpy(nuevo->letra,"g");
                        nuevo->numdiscos=7;
                    }else if (discos==8) {
                        strcpy(nuevo->letra,"h");
                        nuevo->numdiscos=8;
                    }else if (discos==9) {
                        strcpy(nuevo->letra,"i");
                        nuevo->numdiscos=9;
                    }else if (discos==10) {
                        strcpy(nuevo->letra,"j");
                        nuevo->numdiscos=10;
                    }else if (discos==11) {
                        strcpy(nuevo->letra,"k");
                        nuevo->numdiscos=11;
                    }else if (discos==12) {
                        strcpy(nuevo->letra,"l");
                        nuevo->numdiscos=12;
                    }else if (discos==13) {
                        strcpy(nuevo->letra,"m");
                        nuevo->numdiscos=13;
                    }else if (discos==14) {
                        strcpy(nuevo->letra,"n");
                        nuevo->numdiscos=14;
                    }else if (discos==15) {
                        strcpy(nuevo->letra,"o");
                        nuevo->numdiscos=15;
                    }else if (discos==16) {
                        strcpy(nuevo->letra,"p");
                        nuevo->numdiscos=16;
                    }else if (discos==17) {
                        strcpy(nuevo->letra,"q");
                        nuevo->numdiscos=17;
                    }else if (discos==18) {
                        strcpy(nuevo->letra,"r");
                        nuevo->numdiscos=18;
                    }else if (discos==19) {
                        strcpy(nuevo->letra,"s");
                        nuevo->numdiscos=19;
                    }
                    char tmp2[10]="";

                    //strcpy(nuevo->letra,"a");
                    strcat(tmp2,"vd");
                    strcat(tmp2,nuevo->letra);
                    strcat(tmp2,nuevo->numero);
                    strcpy(nuevo->id,tmp2);
                }else{
                    if (discoReturn==1) {
                        strcpy(nuevo->letra,"a");
                        nuevo->numdiscos=1;
                    }else if (discoReturn==2) {
                        strcpy(nuevo->letra,"b");
                        nuevo->numdiscos=2;
                    }else if (discoReturn==3) {
                        strcpy(nuevo->letra,"c");
                        nuevo->numdiscos=3;
                    }else if (discoReturn==4) {
                        strcpy(nuevo->letra,"d");
                        nuevo->numdiscos=4;
                    }else if (discoReturn==5) {
                        strcpy(nuevo->letra,"e");
                        nuevo->numdiscos=5;
                    }else if (discoReturn==6) {
                        strcpy(nuevo->letra,"f");
                        nuevo->numdiscos=6;
                    }else if (discoReturn==7) {
                        strcpy(nuevo->letra,"g");
                        nuevo->numdiscos=7;
                    }else if (discoReturn==8) {
                        strcpy(nuevo->letra,"h");
                        nuevo->numdiscos=8;
                    }else if (discoReturn==9) {
                        strcpy(nuevo->letra,"i");
                        nuevo->numdiscos=9;
                    }else if (discoReturn==10) {
                        strcpy(nuevo->letra,"j");
                        nuevo->numdiscos=10;
                    }else if (discoReturn==11) {
                        strcpy(nuevo->letra,"k");
                        nuevo->numdiscos=11;
                    }else if (discoReturn==12) {
                        strcpy(nuevo->letra,"l");
                        nuevo->numdiscos=12;
                    }else if (discoReturn==13) {
                        strcpy(nuevo->letra,"m");
                        nuevo->numdiscos=13;
                    }else if (discoReturn==14) {
                        strcpy(nuevo->letra,"n");
                        nuevo->numdiscos=14;
                    }else if (discoReturn==15) {
                        strcpy(nuevo->letra,"o");
                        nuevo->numdiscos=15;
                    }else if (discoReturn==16) {
                        strcpy(nuevo->letra,"p");
                        nuevo->numdiscos=16;
                    }else if (discoReturn==17) {
                        strcpy(nuevo->letra,"q");
                        nuevo->numdiscos=17;
                    }else if (discoReturn==18) {
                        strcpy(nuevo->letra,"r");
                        nuevo->numdiscos=18;
                    }else if (discoReturn==19) {
                        strcpy(nuevo->letra,"s");
                        nuevo->numdiscos=19;
                    }
                    char tmp2[10]="";

                    //strcpy(nuevo->letra,"a");
                    strcat(tmp2,"vd");
                    strcat(tmp2,nuevo->letra);
                    strcat(tmp2,nuevo->numero);
                    strcpy(nuevo->id,tmp2);
                }
                //nuevo->numdiscos=discos;

            nuevo->siguiente=NULL;
            nuevo->anterior=ultimo;
            ultimo->siguiente=nuevo;
            ultimo=nuevo;
        }
    }
    numNodos++;
}

void particionesMontadas(){
    printf("\nPARTICIONES MONTADAS:\n");
    struct nodo * actual;
    posicion=1;
    actual=primero;
    while (posicion<=numNodos) {
        printf(" %d.-",posicion);
        printf(" Nombre: %s  --> #Disco: %d\n",actual->id,actual->numdiscos);
        actual=actual->siguiente;
        posicion=posicion+1;
    }
    if (numNodos==0) {
        printf("No existen particiones montadas\n");
    }
}

void particionesMontadas2(char path[]){
    printf("\nPARTICIONES MONTADAS:\n");
    struct nodo * actual;
    posicion=1;
    actual=primero;
    while (posicion<=numNodos) {
        if(strcmp(actual->path,path)==0){
            //printf("\nL->%s ---->%s",actual->letra,actual->path);
        }
        actual=actual->siguiente;
        posicion=posicion+1;
    }
    if (numNodos==0) {
        printf("No existen particiones montadas\n");
    }


}

int buscarParticionesMontadas(char path[]){
    //printf("\nPARTICIONES MONTADAS:\n");
    struct nodo * actual;
    posicion=1;
    int numeroDeVeces = 0;
    actual=primero;
    while (actual!= NULL) {
        if(strcmp(path,actual->path)==0){
            //printf("\nLetra----->%s\n",actual->letra);
            numeroDeVeces++;
        }
        actual=actual->siguiente;
        posicion=posicion+1;
    }
    if (numNodos==0) {
        //printf("No existen particiones montadas\n");
    }
    //printf("\nNumero de Veces montado: %d",numeroDeVeces);
    return numeroDeVeces;
}

int buscarDisco(char path[]){
    //printf("\nPARTICIONES MONTADAS:\n");
    struct nodo * actual;
    posicion=1;
    actual=primero;
    while (actual!= NULL) {
        if(strcmp(path,actual->path)==0){
            //actual->numdiscos;
            return 1;

        }
        actual=actual->siguiente;
        posicion=posicion+1;
    }
    if (numNodos==0) {
        //printf("No existen particiones montadas\n");
    }
    //discos++;
    //printf("\nNumero de Veces montado: %d",numeroDeVeces);
    return 0;
}

int retornarNumDisco(char path[]){
    //printf("\nPARTICIONES MONTADAS:\n");
    struct nodo * actual;
    posicion=1;
    actual=primero;
    while (actual!= NULL) {
        if(strcmp(path,actual->path)==0){
            //actual->numdiscos;
            //printf("\nRETORNAR NUM %d\n",actual->numdiscos);
            return actual->numdiscos;

        }
        actual=actual->siguiente;
        posicion=posicion+1;
    }
    if (numNodos==0) {
        //printf("No existen particiones montadas\n");
    }
    //printf("\nNumero de Veces montado: %d",numeroDeVeces);
    return 0;
}

int verificarPaticionMontada(char nombre[], char path[]){
    struct nodo * actual;
    posicion=1;
    actual=primero;
    int bandera = 0;
    while (posicion<=numNodos) {
        if(strcmp(actual->nombre,nombre)==0&&strcmp(actual->path,path)==0){
            bandera = 1;
            break;
        }
        actual=actual->siguiente;
        posicion=posicion+1;
    }
    return bandera;
}

int processMOUNT(char str[], int cont){
    //printf("OLA KE HACE %d\n",cont);

    if(cont == -1){
        particionesMontadas();
        return 0;
    }
    m_init m;
    memset(&m.path,'\0',sizeof(m.path));
    memset(&m.name,'\0',sizeof(m.name));
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
            if(strcasecmp("PATH", name) == 0){
                strcat(m.path, buffer);
            }
            else if(strcasecmp("NAME", name) == 0){
                strcat(m.name, buffer);
            }else{
                printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
        //printf("4%s4",m.name);
    }
    if((j != 0) && read){
        buffer[j] = '\0';
        // Posible optimizacion
        if(strcasecmp("PATH", name) == 0){
            strcat(m.path, buffer);
        }else if(strcasecmp("NAME", name) == 0){
            strcat(m.name, buffer);
        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    mount(m.name,m.path);



    return 0;
}
