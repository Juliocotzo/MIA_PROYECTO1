#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include "estructuras.h"

#define RED   "\x1B[1;31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[1;36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void mkfs(char id[], char type[]);
void formato2(char direccion[], char name[]);
void upper_string(char s[]);
int numeroBloques(int size);
void EXT3(char path[], int indice, int n);

typedef struct mkfs{
    char id[15];
    char type[15];
} mkfs_init;

void upper_string(char s[]) {
   int c = 0;

   while (s[c] != '\0') {
      if (s[c] >= 'a' && s[c] <= 'z') {
         s[c] = s[c] - 32;
      }
      c++;
   }
}

int processMKFS(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS \n" RESET);
        return 0;
    }
    mkfs_init rm;
    memset(&rm.id,'\0',sizeof(rm.id));
    memset(&rm.type,'\0',sizeof(rm.type));
    strcat(rm.type, "FULL");
    // Base Program
    char ch, buffer[100];
    memset(&buffer,'\0',sizeof(buffer));
    int j=0;
    bool read = true;
    bool num = true;
    char name[10];
    memset(&name,'\0',sizeof(name));
    while((ch = str[cont++]) != '\0'){
        if(ch == '-' && read){
            buffer[j] = '\0';
            memset(&name,'\0',sizeof(name));
            strcat(name, buffer);
            j = 0;
            // num = !num;
            memset(&buffer,'\0',sizeof(buffer));
        }
        if(ch == '\"' || ch == '\”'){
            read = !read;
        }
        if(isdigit(ch) && num){
            buffer[j++] = ch;
        }
        if(isalpha(ch) || ch == '/' || ch == '.' || ch == '_'){
            buffer[j++] = ch;
        }
        if(ch == '#'){
            break;
        }
        if(ch == ' ' && !read){
            buffer[j++] = ch;
        }else if((ch == ' ' || ch == '\n') && (j != 0) && read){
            buffer[j] = '\0';
            if(strcasecmp("ID", name) == 0){
                strcat(rm.id, buffer);
            }
            else if(strcasecmp("TYPE", name) == 0){
                memset(&rm.type,'\0',sizeof(rm.type));
                upper_string(name);
                if (strcasecmp("FAST",buffer)==0) {
                    strcat(rm.type, "FAST");
                }else  if (strcasecmp("FULL",buffer)==0) {
                    strcat(rm.type, "FULL");
                }else {
                    strcat(rm.type, "FULL");

                }

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
            strcat(rm.id, buffer);
        }
        else if(strcasecmp("TYPE", name) == 0){
            memset(&rm.type,'\0',sizeof(rm.type));
            upper_string(name);
            if (strcasecmp("FAST",buffer)==0) {
                strcat(rm.type, "FAST");
            }else  if (strcasecmp("FULL",buffer)==0) {
                strcat(rm.type, "FULL");
            }else {
                strcat(rm.type, "FULL");
            }

        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    printf("\nid->%s|type->%s|\n",rm.id,rm.type);
    mkfs(rm.id,rm.type);



    return 0;
}

void mkfs(char id[], char type[]){
    time_t t;
    struct nodo *actual;
    char aux[100];
    int p = 1;
    int bandera = 0;
    actual = primero;
    char direccion[100];
    char name[25];
    struct nodo *aux2;
    struct nodo *actual2;
    int nodo_eliminar;

    if(strcmp(id,"")!=0){

        strcpy(aux,id);
        while (actual != NULL) {
            if (strcmp(aux,actual->id)==0) {
                bandera=1;
                strcpy(direccion,actual->path);
                strcpy(name,actual->nombre);
                break;
            }
            actual=actual->siguiente;

        }

        if (bandera==1) {
            if(strcmp(type,"FAST")==0){
                formato2(direccion,name);
            }else if(strcmp(type,"FULL")==0){
                formato2(direccion,name);
            }else{
                printf("\nERROR: Parametro type erroneo");
            }
        }else {
            printf("\nERROR: El parametro id no existe");
        }
    }else{
        printf("\nERROR: El parametro id no esta especificado");
    }
}

void formato2(char direccion[], char name[]){
    int inicio;
    int numeroN;
    int bandera=0;
    MBR disco;
    FILE *file;
    file = fopen(direccion,"rb+");
    if(file!=NULL){
        fread(&disco,sizeof(MBR),1,file);
        int i;
        for (i = 0; i < 4; ++i) {
            if(strcmp(disco.mbr_partition[i].part_name,name)==0){
                inicio=disco.mbr_partition[i].part_start;
                bandera=1;
                break;
            }
        }
        if(bandera==1){
            numeroN=numeroBloques(disco.mbr_partition[i].part_size);
            printf("\n%s|%d|%i\n",direccion,numeroN,i);
            EXT3(direccion,i,numeroN);
        }else{
            printf("\nERROR: Formato, bandera");
        }
        fclose(file);
    }else{
        printf("\nERROR: No se abrir el archivo");
    }
}

int numeroBloques(int size){
    int n=0;
    float z;
    int x = size - sizeof(superBloque);
    int y = sizeof (journaling)+sizeof (inodo)+3*sizeof (bloqueArchivos)+4;
    z = x/y;
    n = floor(z);
    return n;
}

void EXT3(char path[], int indice, int n){
    MBR disco;
    FILE *file;
    superBloque SB;
    inodo i_nodo;
    bloqueArchivos ba;
    journaling jour;
    int inicio_sb;
    char output[18]="";

    file = fopen(path,"rb+");
    if(file!=NULL){
        fread(&disco,sizeof(MBR),1,file);
        fclose(file);
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
        return;
    }
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    strftime(output,18,"%d/%m/%y %H:%M:%S",tlocal);

    /*********************************************************************************
     **                             SUPER BLOQUE                                    **
     *********************************************************************************/

    SB.s_filesystem_type = 3;
    SB.s_inodes_count = n;
    SB.s_blocks_count = 3*n;
    SB.s_free_blocks_count = 3*n;
    SB.s_free_inodes_count = n;
    SB.s_mtime = tiempo;
    SB.s_umtime = tiempo;
    SB.s_mnt_count = 0;
    SB.s_magic = 61267;
    SB.s_inode_size = sizeof(inodo);
    SB.s_block_size = sizeof(bloqueApuntadores);
    SB.s_first_ino = 1;
    SB.s_first_blo = 1;
    SB.s_bm_inode_start = disco.mbr_partition[indice].part_start + sizeof(superBloque) + (sizeof(journaling)*n);
    SB.s_bm_block_start = SB.s_bm_inode_start + n;
    SB.s_inode_start = SB.s_bm_block_start + (3*n);
    SB.s_block_start = SB.s_inode_start + (sizeof(inodo)*n);

    inicio_sb = disco.mbr_partition[indice].part_start;

    /*********************************************************************************
     **                                   INODO                                     **
     *********************************************************************************/

    i_nodo.i_uid = -1;
    i_nodo.i_gid = -1;
    i_nodo.i_size = -1;
    i_nodo.i_atime = tiempo;
    i_nodo.i_ctime = tiempo;
    i_nodo.i_mtime = tiempo;
    int i;
    for(i=0;i<15;++i){
        i_nodo.i_block[i]=-1;
    }
    i_nodo.i_type = -1;
    i_nodo.i_perm = -1;

    /*********************************************************************************
     **                                JOURNALING                                   **
     *********************************************************************************/

     jour.journal_tipo_operacion=-1;
     jour.journal_tipo=-1;
     strcpy(jour.journal_nombre,"");
     strcpy(jour.journal_contenido,"");
     jour.journal_fecha = tiempo;
     jour.journal_propietario=-1;
     jour.journal_permisos = -1;

    /*********************************************************************************
     **                                 FORMATO                                     **
     *********************************************************************************/

    file = fopen(path,"rb+");
    if(file!=NULL){
        fread(&disco,sizeof(MBR),1,file);
        rewind(file);
        //printf("\nPosicion INICIO: %i",ftell(file));
        fseek(file,inicio_sb,SEEK_SET);
        //printf("\nPosicion SB: %i",ftell(file));
        fwrite(&SB,sizeof(superBloque),1,file);
        int n2 = n*3;
        for(i=0;i<n;++i){
            fwrite(&jour,sizeof(journaling),1,file);
        }
        //fwrite(&jour,sizeof(journaling),1,file);
        //printf("\nPosicion BM1: %i",ftell(file));
        //printf("\nN: %i",n);
        for(i=0;i<n;++i){
            fwrite("0",1,1,file);
        }
        //printf("\nPosicion BM2: %i",ftell(file));
        //printf("\nN: %i",n);
        for(i=0;i<n2;++i){
            fwrite("0",1,1,file);
        }
        //printf("\nPosicion INODO: %i",ftell(file));
        for(i=0;i<n;++i){
            fwrite(&i_nodo,sizeof(inodo),1,file);
        }
        //printf("\nPosicion BloqueS: %i",ftell(file));
        strcpy(&ba.b_content,"");
        for(i=0;i<n2;++i){
            fwrite(&ba,sizeof(bloqueArchivos),1,file);
        }
        //fseek(file,(192*n),SEEK_CUR);
        //printf("\nPosicion FIN: %i",ftell(file));
    /*********************************************************************************
     **                                ACT. SUPER BLOQUE                            **
     *********************************************************************************/

        SB.s_first_ino = 4;
        SB.s_first_blo = 4;

        rewind(file);
        fseek(file,inicio_sb,SEEK_SET);
        fwrite(&SB,sizeof(superBloque),1,file);

    /*********************************************************************************
     **                                ACT. BITMAP INODO                            **
     *********************************************************************************/
     fseek(file,SB.s_bm_inode_start,SEEK_SET);
     for(i=0;i<3;++i){
        fwrite("1",1,1,file);
     }

    /*********************************************************************************
     **                                ACT. BITMAP BLOQUE                           **
     *********************************************************************************/

     fseek(file,SB.s_bm_block_start,SEEK_SET);
     for(i=0;i<3;++i){
        fwrite("1",1,1,file);
     }
        tiempo = time(0);

    /*********************************************************************************
     **                                ESCRIBIR INODO                               **
     *********************************************************************************/

        fseek(file,SB.s_inode_start,SEEK_SET);

        i_nodo.i_uid = 1;
        i_nodo.i_gid = 1;
        i_nodo.i_size = 0;
        i_nodo.i_atime = tiempo;
        i_nodo.i_ctime = tiempo;
        i_nodo.i_mtime = tiempo;
        i_nodo.i_block[0] = 1;
        i_nodo.i_type = 0;
        i_nodo.i_perm = 777;
        fwrite(&i_nodo,sizeof(inodo),1,file);

        i_nodo.i_block[0] = 2;
        fwrite(&i_nodo,sizeof(inodo),1,file);

        i_nodo.i_block[0] = 3;
        i_nodo.i_type = 1;
        fwrite(&i_nodo,sizeof(inodo),1,file);

    /*********************************************************************************
     **                                ESCRIBIR BLOQUE                              **
     **                                     CARPETAS                                **
     *********************************************************************************/

        fseek(file,SB.s_block_start,SEEK_SET);

        bloqueCarpetas aux1;

        for(i=0;i<4;++i){
            aux1.b_content[i].b_inodo=0;
            strcpy(aux1.b_content[i].b_name,"");
        }

        strcpy(aux1.b_content[0].b_name,"/");
        aux1.b_content[0].b_inodo = 2;
        fwrite(&aux1,sizeof(bloqueCarpetas),1,file);

        strcpy(aux1.b_content[0].b_name,"users.txt");
        aux1.b_content[0].b_inodo = 3;
        fwrite(&aux1,sizeof(bloqueCarpetas),1,file);



    /*********************************************************************************
     **                                ESCRIBIR BLOQUE                              **
     **                                     ARCHIVOS                                **
     *********************************************************************************/

        int posicionCaracter=0;
        char caracter[65]="";
        char frase[64]="1, G, root\n1, U, root, root, 123\n";
        int tamanio = strlen(frase);

        for(i=0;i<64;++i){
            if(posicionCaracter<tamanio){
                caracter[i]=frase[posicionCaracter];
                posicionCaracter++;
            }else{
                char aux[10]="";
                caracter[i]=aux[0];
            }
        }
        strcpy(ba.b_content,caracter);
        fwrite(&ba,sizeof(bloqueArchivos),1,file);

        fclose(file);
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
}

