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


extern char usuario[10]="";
extern char contrasenia[10]="";
extern char id_activo[10]="";
char id_activo2;
extern char id_activo3[10]="";
extern char path_activa[50]="";
extern char name_activo[25]="";
extern int bandera_log = 0;

extern int idu=1;
extern int idg=1;



typedef struct login{
    char usr[15];
    char password[15];
    char id[15];
} login_init;

void login(char usr[], char pwd[], char id[]);

int processLOGIN(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS \n" RESET);
        return 0;
    }
    login_init rm;
    memset(&rm.usr,'\0',sizeof(rm.usr));
    memset(&rm.password,'\0',sizeof(rm.password));
    memset(&rm.id,'\0',sizeof(rm.id));
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
            if(strcasecmp("USR", name) == 0){
                strcat(rm.usr, buffer);
            }
            else if(strcasecmp("PWD", name) == 0){
                strcat(rm.password, buffer);

            }else if(strcasecmp("ID", name) == 0){
                strcat(rm.id, buffer);

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
        if(strcasecmp("USR", name) == 0){
            strcat(rm.usr, buffer);
        }
        else if(strcasecmp("PWD", name) == 0){
            strcat(rm.password, buffer);

        }else if(strcasecmp("ID", name) == 0){
            strcat(rm.id, buffer);

        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    printf("\nUsuario->%s|Password->%s|Id->%s|\n",rm.usr,rm.password,rm.id);
    login(rm.usr,rm.password,rm.id);

    printf("\nBANDERA LOG--->%d\n",bandera_log);
    return 0;
}

void login(char usr[], char pwd[], char id[]){

    struct nodo *actual;
    char aux[100];
    int p = 1;
    int bandera = 0;
    actual = primero;
    char direccion[100];
    char name[25];
    char reinicio[100]="";


    struct nodo *aux2;
    struct nodo *actual2;

    MBR disco;
    FILE *file;
    int inicio;
    superBloque  sb;
    bloqueArchivos ba;
    bloqueCarpetas bc;
    inodo ino;
    int i;
    int inicio_bloque;

    printf(YEL "\nLogin\n" RESET);
    if(bandera_log==0){
        if(strcmp(id,"")!=0 && strcmp(pwd,"")!=0 && strcmp(usr,"")!=0){

            /************************************************************
            **                        BUSCAR                         **
            ************************************************************/
            strcpy(aux,id);

            while (p<=numNodos) {
                if (strcmp(aux,actual->id)==0) {
                    bandera=1;
                    strcpy(direccion,actual->path);
                    strcpy(name,actual->nombre);
                    break;
                }
                p++;
                actual=actual->siguiente;
            }

            file = fopen(direccion,"rb+");
            if(file!=NULL){
                fread(&disco,sizeof(MBR),1,file);
                for (i = 0; i < 4; ++i) {
                    if(strcmp(disco.mbr_partition[i].part_name,name)==0){
                        inicio=disco.mbr_partition[i].part_start;
                        bandera=1;
                        break;
                    }
                }
                if(bandera==1){
                    /************************************************************
                    **                     BUSCAR USUARIO                     **
                    ************************************************************/
                    int i_archivo;
                    fseek(file,inicio,SEEK_SET);
                    fread(&sb,sizeof(superBloque),1,file);
                    inicio_bloque = sb.s_block_start;
                    fseek(file,inicio_bloque,SEEK_SET);
                    int j;
                    for(i=0;i<sb.s_blocks_count;++i){
                        fread(&bc,sizeof(bloqueCarpetas),1,file);
                        for(j=0;j<4;++j){
                            if(strcmp(bc.b_content[j].b_name,"users.txt")==0){
                                i_archivo = bc.b_content[j].b_inodo;
                                break;
                            }
                        }
                    }
                    int pos_aux=i_archivo-1;
                    pos_aux = pos_aux * sizeof(inodo);
                    fseek(file,sb.s_inode_start,SEEK_SET);
                    fseek(file,pos_aux,SEEK_CUR);
                    fread(&ino,sizeof(inodo),1,file);

                    int ban_block=0;
                    int contador=0;
                    char contenido[2000]="";
                    while(ban_block==0){
                        if(ino.i_block[contador]!=-1){
                            int lectura;
                            lectura = ino.i_block[contador]-1;
                            lectura = sizeof(bloqueArchivos)*lectura;
                            fseek(file,inicio_bloque,SEEK_SET);
                            fseek(file,lectura,SEEK_CUR);
                            fread(&ba,sizeof(bloqueArchivos),1,file);
                            strcat(contenido,ba.b_content);
                        }else{
                            ban_block=1;
                        }
                        contador++;
                    }
                    char busqueda[1000]="";
                    strcat(busqueda,usr);
                    strcat(busqueda,", ");
                    strcat(busqueda,pwd);


                    int ban1=0;
                    char auxiliar;
                    char busqueda2[50]="";
                    int cont2=0;
                    int cont3=0;
                    char auxiliar2[10]="";

                    for(i=0;i<strlen(contenido);++i){
                        auxiliar=contenido[i];
                        if(auxiliar!='0'){
                            while(contenido[i]!=','){
                                auxiliar2[cont3]=contenido[i];
                                i++;
                                cont3++;
                            }
                            i=i+2;
                            if(contenido[i]=='U'||contenido[i]=='u'){
                                i=i+2;
                                while(contenido[i]!=','){
                                    i++;
                                }
                                i=i+2;
                                int contador2 = 0;
                                int j;
                                while(contenido[i]!='\n'){
                                    busqueda2[cont2]=contenido[i];
                                    cont2++;
                                    i++;
                                }
                            }else{
                                while(contenido[i]!='\n'){
                                    busqueda2[cont2]=contenido[i];
                                    cont2++;
                                    i++;
                                }
                            }
                            if(strcmp(busqueda,busqueda2)==0){
                                //printf("\n**%c",auxiliar);
                                //printf("\n1. %s -- 2. %s",busqueda,busqueda2);
                                ban1=1;
                                break;
                            }else{
                                int k;
                                for(k=0;k<50;k++){
                                    busqueda2[k]='\0';
                                }
                                cont2=0;
                            }
                            cont3=0;
                        }
                    }
                    if(ban1==1){
                        strcpy(usuario,usr);
                        strcpy(contrasenia,pwd);
                        strcpy(id_activo,id);
                        strcpy(path_activa,direccion);
                        strcpy(name_activo,name);
                        id_activo2=auxiliar;
                        strcpy(id_activo3,auxiliar2);
                        bandera_log = 1;
                    }else{
                        printf("\nEl usuario no existe");
                    }
                }else{
                    printf("\nERROR: Particion no existente");
                }
                fclose(file);
            }else{
                printf("\nERROR: No se abrir el archivo");
            }
        }else{
            printf("\nERROR: Atributos mal especificados");
        }
    }else{
        printf("\nERROR: Cierre sesion antes de iniciar otra");
    }
}
