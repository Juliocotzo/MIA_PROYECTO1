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

typedef struct mkdir{
    char path[50];
    char P[5]
} mkdir_init;

void mkdir2(char path[], char p[]);
void crearCarpeta2(char path[], int usuario, char direccion[], char name[], int tipo);
void crearCarpeta(char path[], int usuario, char direccion[], char name[], int tipo);
int firstInodo(char direccion[], char name[]);
int firtBlock(char direccion[], char name[]);
int buscarInodo(char direccion[], char name[]);
int buscarBloque(char direccion[], char name[]);

int processMKDIR(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS \n" RESET);
        return 0;
    }
    mkdir_init rm;
    memset(&rm.path,'\0',sizeof(rm.path));
    memset(&rm.P,'\0',sizeof(rm.P));
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
        if(ch == ' '  && !read){
            buffer[j++] = ch;
        }else if((ch == ' ' || ch == '\n') && (j != 0) && read){
            buffer[j] = '\0';
            if(strcasecmp("PATH", name) == 0){
                strcat(rm.path, buffer);
            }
            else if(strcasecmp("P", buffer) == 0){
                strcat(rm.P, "P");

            }else{
                printf(RED "ERROR: COMANDO INEXISTENTE1\n" RESET);

                printf("ch:%c",ch);
                printf("buffer:%s",buffer);
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    }

    if((j != 0) && read ){
        buffer[j] = '\0';
        if(strcasecmp("PATH", name) == 0){
            strcat(rm.path, buffer);
        }
        else if(strcasecmp("P", buffer) == 0){
            strcat(rm.P, "P");

        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE1\n" RESET);

            printf("ch:%c",ch);
            printf("buffer:%s",buffer);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    printf("\nPATH->%s|P->%s|\n",rm.path,rm.P);
    mkdir2(rm.path,rm.P);

    return 0;
}


void mkdir2(char path[], char p[]){
    printf("\n********************MKDIR**************************");
    printf("\nHOLA--->|%s|%s|\n",path,p);
    if(bandera_log==1){
        if(strcmp(path,"")!=0){
            if(strcmp(p,"P")==0){
                printf("\nCON P\n");
                int auxiliar = atoi(id_activo3);
                crearCarpeta2(path,auxiliar,path_activa,name_activo,0);
                int sb1 = firstInodo(path_activa,name_activo);
                int sb2 = firtBlock(path_activa,name_activo);
            }else if(strcmp(p,"")==0){
                printf("\nSIN P\n");
                int auxiliar = atoi(id_activo3);
                crearCarpeta(path,auxiliar,path_activa,name_activo,0);
                int sb1 = firstInodo(path_activa,name_activo);
                int sb2 = firtBlock(path_activa,name_activo);
            }else{
                printf("\nERROR: El parametro -p esta mal declarado");
            }
        }else{
            printf("\nERROR: El parametro path no fue indicado");
        }
    }else{
        printf("\nERROR: No se ha iniciado ninguna sesion");
    }
}

void crearCarpeta2(char path[], int usuario, char direccion[], char name[], int tipo){
    int i;
    int inicio;
    int bandera;
    superBloque sb;
    bloqueCarpetas bbc;
    bloqueArchivos bba;
    inodo ino;
    int inicio_bloque;
    int indice;
    int bmbloque;

    MBR disco;
    FILE *file;
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
                fread(&bbc,sizeof(bloqueCarpetas),1,file);
                for(j=0;j<4;++j){
                    if(strcmp(bbc.b_content[j].b_name,"/")==0){
                        i_archivo = bbc.b_content[j].b_inodo;
                        break;
                    }
                }
            }
            int pos_aux=i_archivo-1;
            pos_aux = pos_aux * sizeof(inodo);

            char auxiliar[25]="";
            int contador1=0;
            int contador2=0;
            int flag_carpeta=0;
            int pos_aux3=0;
            for(i=1;i<strlen(path);++i){
                if(path[i]!='/'){
                    auxiliar[contador1]=path[i];
                    contador1++;
                }else{
                    int k;
                    for(k=0;k<15;k++){
                        if(k<12){
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,pos_aux,SEEK_CUR);
                            fread(&ino,sizeof(inodo),1,file);
                            if(ino.i_block[k]!=-1){
                                int pos_aux2 = ino.i_block[k]-1;
                                pos_aux2 = pos_aux2 * sizeof(bloqueCarpetas);
                                fseek(file,sb.s_block_start,SEEK_SET);
                                fseek(file,pos_aux2,SEEK_CUR);
                                fread(&bbc,sizeof(bloqueCarpetas),1,file);
                                for(j=0;j<4;++j){
                                    if(strcmp(auxiliar,bbc.b_content[j].b_name)==0){
                                        pos_aux=bbc.b_content[j].b_inodo-1;
                                        pos_aux=pos_aux*sizeof(inodo);
                                        flag_carpeta=1;
                                        break;
                                    }
                                }
                            }
                            if(flag_carpeta==1){
                                break;
                            }
                        }
                    }
                    if(flag_carpeta==0){
                        for(k=0;k<15;++k){
                            if(k<12){
                                fseek(file,sb.s_inode_start,SEEK_SET);
                                fseek(file,pos_aux,SEEK_CUR);
                                fread(&ino,sizeof(inodo),1,file);
                                if(ino.i_block[k]!=-1){
                                    int pos_aux2 = ino.i_block[k]-1;
                                    pos_aux2 = pos_aux2 * sizeof(bloqueCarpetas);
                                    fseek(file,sb.s_block_start,SEEK_SET);
                                    fseek(file,pos_aux2,SEEK_CUR);
                                    fread(&bbc,sizeof(bloqueCarpetas),1,file);
                                    for(j=0;j<4;++j){
                                        if(bbc.b_content[j].b_inodo==0||bbc.b_content[j].b_inodo==-1){
                                            int bmi = buscarInodo(direccion,name);
                                            bbc.b_content[j].b_inodo=bmi;
                                            strcpy(bbc.b_content[j].b_name,auxiliar);
                                            fseek(file,sb.s_block_start,SEEK_SET);
                                            fseek(file,pos_aux2,SEEK_CUR);
                                            fwrite(&bbc,sizeof(bloqueCarpetas),1,file);
                                            bmi=bmi-1;
                                            bmi=bmi*sizeof(inodo);

                                            inodo ino2;
                                            time_t tiempo = time(0);

                                            fseek(file,sb.s_inode_start,SEEK_SET);
                                            fseek(file,bmi,SEEK_CUR);
                                            fread(&ino2,sizeof(inodo),1,file);

                                            ino2.i_uid = usuario;
                                            //ino2.i_gid = 1;
                                            ino2.i_size=0;
                                            ino2.i_atime = tiempo;
                                            ino2.i_ctime = tiempo;
                                            ino2.i_mtime = tiempo;
                                            ino2.i_type = tipo;
                                            ino2.i_perm = 664;
                                            fseek(file,sb.s_inode_start,SEEK_SET);
                                            fseek(file,bmi,SEEK_CUR);
                                            fwrite(&ino2,sizeof(inodo),1,file);
                                            pos_aux=bmi;
                                            flag_carpeta=0;
                                            break;
                                        }
                                    }
                                }else{
                                    int bmi = buscarInodo(direccion,name);
                                    int bmb = buscarBloque(direccion,name);
                                    int pos_aux2 = bmb - 1;
                                    pos_aux2 = pos_aux2*sizeof(bloqueCarpetas);

                                    bloqueCarpetas bbc2;
                                    bbc2.b_content[0].b_inodo=bmi;
                                    strcpy(bbc2.b_content[0].b_name,auxiliar);
                                    bbc2.b_content[1].b_inodo=0;
                                    strcpy(bbc2.b_content[1].b_name,"");
                                    bbc2.b_content[2].b_inodo=0;
                                    strcpy(bbc2.b_content[2].b_name,"");
                                    bbc2.b_content[3].b_inodo=0;
                                    strcpy(bbc2.b_content[3].b_name,"");

                                    fseek(file,sb.s_block_start,SEEK_SET);
                                    fseek(file,pos_aux2,SEEK_CUR);
                                    fwrite(&bbc2,sizeof(bloqueCarpetas),1,file);

                                    ino.i_block[k]=bmb;
                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,pos_aux,SEEK_CUR);
                                    fwrite(&ino,sizeof(inodo),1,file);

                                    bmi=bmi-1;
                                    bmi=bmi*sizeof(inodo);

                                    inodo ino2;
                                    time_t tiempo = time(0);

                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fread(&ino2,sizeof(inodo),1,file);

                                    ino2.i_uid = usuario;
                                    //ino2.i_gid = 1;
                                    ino2.i_size=0;
                                    ino2.i_atime = tiempo;
                                    ino2.i_ctime = tiempo;
                                    ino2.i_mtime = tiempo;
                                    ino2.i_type = tipo;
                                    ino2.i_perm = 664;
                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fwrite(&ino2,sizeof(inodo),1,file);
                                    pos_aux=bmi;
                                    flag_carpeta=0;
                                    break;
                                }
                                if(flag_carpeta==0){
                                    break;
                                }
                            }
                        }
                    }
                    flag_carpeta=0;
                    contador1=0;
                    contador2++;
                    for(j=0;j<25;++j){
                        auxiliar[j]='\0';
                    }
                }
            }


            int k;
            for(k=0;k<15;k++){
                if(k<12){
                    fseek(file,sb.s_inode_start,SEEK_SET);
                    fseek(file,pos_aux,SEEK_CUR);
                    fread(&ino,sizeof(inodo),1,file);
                    if(ino.i_block[k]!=-1){
                        int pos_aux2 = ino.i_block[k]-1;
                        pos_aux2 = pos_aux2 * sizeof(bloqueCarpetas);
                        fseek(file,sb.s_block_start,SEEK_SET);
                        fseek(file,pos_aux2,SEEK_CUR);
                        fread(&bbc,sizeof(bloqueCarpetas),1,file);
                        for(j=0;j<4;++j){
                            if(strcmp(auxiliar,bbc.b_content[j].b_name)==0){
                                pos_aux=bbc.b_content[j].b_inodo-1;
                                pos_aux=pos_aux*sizeof(inodo);
                                flag_carpeta=1;
                                break;
                            }
                        }
                    }
                    if(flag_carpeta==1){
                        break;
                    }
                }
            }
            if(flag_carpeta==0){
                flag_carpeta=1;
                for(k=0;k<15;++k){
                    if(k<12){
                        fseek(file,sb.s_inode_start,SEEK_SET);
                        fseek(file,pos_aux,SEEK_CUR);
                        fread(&ino,sizeof(inodo),1,file);
                        if(ino.i_block[k]!=-1){
                            int pos_aux2 = ino.i_block[k]-1;
                            pos_aux2 = pos_aux2 * sizeof(bloqueCarpetas);
                            fseek(file,sb.s_block_start,SEEK_SET);
                            fseek(file,pos_aux2,SEEK_CUR);
                            fread(&bbc,sizeof(bloqueCarpetas),1,file);
                            for(j=0;j<4;++j){
                                if(bbc.b_content[j].b_inodo==0||bbc.b_content[j].b_inodo==-1){
                                    int bmi = buscarInodo(direccion,name);
                                    bbc.b_content[j].b_inodo=bmi;
                                    strcpy(bbc.b_content[j].b_name,auxiliar);
                                    fseek(file,sb.s_block_start,SEEK_SET);
                                    fseek(file,pos_aux2,SEEK_CUR);
                                    fwrite(&bbc,sizeof(bloqueCarpetas),1,file);
                                    bmi=bmi-1;
                                    bmi=bmi*sizeof(inodo);

                                    inodo ino2;
                                    time_t tiempo = time(0);

                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fread(&ino2,sizeof(inodo),1,file);

                                    ino2.i_uid = usuario;
                                    //ino2.i_gid = 1;
                                    ino2.i_size=0;
                                    ino2.i_atime = tiempo;
                                    ino2.i_ctime = tiempo;
                                    ino2.i_mtime = tiempo;
                                    ino2.i_type = tipo;
                                    ino2.i_perm = 664;
                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fwrite(&ino2,sizeof(inodo),1,file);
                                    flag_carpeta=0;
                                    break;
                                }
                            }
                        }else{

                            int bmi = buscarInodo(direccion,name);
                            int bmb = buscarBloque(direccion,name);
                            int pos_aux2 = bmb - 1;
                            pos_aux2 = pos_aux2*sizeof(bloqueCarpetas);

                            bloqueCarpetas bbc2;
                            bbc2.b_content[0].b_inodo=bmi;
                            strcpy(bbc2.b_content[0].b_name,auxiliar);
                            bbc2.b_content[1].b_inodo=0;
                            strcpy(bbc2.b_content[1].b_name,"");
                            bbc2.b_content[2].b_inodo=0;
                            strcpy(bbc2.b_content[2].b_name,"");
                            bbc2.b_content[3].b_inodo=0;
                            strcpy(bbc2.b_content[3].b_name,"");

                            fseek(file,sb.s_block_start,SEEK_SET);
                            fseek(file,pos_aux2,SEEK_CUR);
                            fwrite(&bbc2,sizeof(bloqueCarpetas),1,file);

                            ino.i_block[k]=bmb;
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,pos_aux,SEEK_CUR);
                            fwrite(&ino,sizeof(inodo),1,file);

                            bmi=bmi-1;
                            bmi=bmi*sizeof(inodo);

                            inodo ino2;
                            time_t tiempo = time(0);

                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,bmi,SEEK_CUR);
                            fread(&ino2,sizeof(inodo),1,file);

                            ino2.i_uid = usuario;
                            //ino2.i_gid = 1;
                            ino2.i_size=0;
                            ino2.i_atime = tiempo;
                            ino2.i_ctime = tiempo;
                            ino2.i_mtime = tiempo;
                            ino2.i_type = tipo;
                            ino2.i_perm = 664;
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,bmi,SEEK_CUR);
                            fwrite(&ino2,sizeof(inodo),1,file);
                            flag_carpeta=0;
                            break;
                        }
                        if(flag_carpeta==0){
                            break;
                        }
                    }
                }
                if(sb.s_filesystem_type==3&&flagJour==0){
                    journaling jour;
                    fseek(file,inicio,SEEK_SET);
                    fseek(file,sizeof(superBloque),SEEK_CUR);
                    fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                    fread(&jour,sizeof(journaling),1,file);

                    jour.journal_tipo_operacion=5;
                    time_t tiempo = time(0);
                    struct tm *tlocal = localtime(&tiempo);
                    char output[18]="";
                    strftime(output,18,"%d/%m/%y %H:%M:%S",tlocal);
                    jour.journal_fecha = tiempo;
                    strcpy(jour.journal_nombre,path);
                    jour.journal_permisos = 664;
                    jour.journal_propietario=usuario;
                    jour.journal_tipo=tipo;

                    fseek(file,inicio,SEEK_SET);
                    fseek(file,sizeof(superBloque),SEEK_CUR);
                    fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                    fwrite(&jour,sizeof(journaling),1,file);
                    numJournaling++;
                }
            }else{
                printf("\nERROR: No se puede crear la carpeta porque ya existe");
            }
            fclose(file);
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
}

void crearCarpeta(char path[], int usuario, char direccion[], char name[], int tipo){
    int i;
    int inicio;
    int bandera;
    superBloque sb;
    bloqueCarpetas bbc;
    bloqueArchivos bba;
    inodo ino;
    int inicio_bloque;
    int indice;
    int bmbloque;

    MBR disco;
    FILE *file;
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
                fread(&bbc,sizeof(bloqueCarpetas),1,file);
                for(j=0;j<4;++j){
                    if(strcmp(bbc.b_content[j].b_name,"/")==0){
                        i_archivo = bbc.b_content[j].b_inodo;
                        break;
                    }
                }
            }
            int pos_aux=i_archivo-1;
            pos_aux = pos_aux * sizeof(inodo);

            char auxiliar[25]="";
            int contador1=0;
            int contador2=0;
            int flag_carpeta=1;
            for(i=1;i<strlen(path);++i){
                if(path[i]!='/'){
                    auxiliar[contador1]=path[i];
                    contador1++;
                }else{
                    flag_carpeta=0;
                    int k;
                    for(k=0;k<15;k++){
                        if(k<12){
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,pos_aux,SEEK_CUR);
                            fread(&ino,sizeof(inodo),1,file);
                            if(ino.i_block[k]!=-1){
                                int pos_aux2 = ino.i_block[k]-1;
                                pos_aux2 = pos_aux2 * sizeof(bloqueCarpetas);
                                fseek(file,sb.s_block_start,SEEK_SET);
                                fseek(file,pos_aux2,SEEK_CUR);
                                fread(&bbc,sizeof(bloqueCarpetas),1,file);
                                for(j=0;j<4;++j){
                                    if(strcmp(auxiliar,bbc.b_content[j].b_name)==0){
                                        pos_aux=bbc.b_content[j].b_inodo-1;
                                        pos_aux=pos_aux*sizeof(inodo);
                                        flag_carpeta=1;
                                        break;
                                    }else{
                                        flag_carpeta=0;
                                    }
                                }
                            }
                            if(flag_carpeta==1){
                                break;
                            }
                        }
                    }
                    if(flag_carpeta==0){
                        break;
                    }
                    contador1=0;
                    contador2++;
                    for(j=0;j<25;++j){
                        auxiliar[j]='\0';
                    }
                }
            }
            if(flag_carpeta==1){
                for(i=0;i<15;++i){
                    if(i<12){
                        fseek(file,sb.s_inode_start,SEEK_SET);
                        fseek(file,pos_aux,SEEK_CUR);
                        fread(&ino,sizeof(inodo),1,file);
                        if(ino.i_block[i]!=-1){
                            int pos_aux2 = ino.i_block[i]-1;
                            pos_aux2 = pos_aux2 * sizeof(bloqueCarpetas);
                            fseek(file,sb.s_block_start,SEEK_SET);
                            fseek(file,pos_aux2,SEEK_CUR);
                            fread(&bbc,sizeof(bloqueCarpetas),1,file);
                            for(j=0;j<4;++j){
                                if(bbc.b_content[j].b_inodo==0||bbc.b_content[j].b_inodo==-1){
                                    int bmi = buscarInodo(direccion,name);
                                    bbc.b_content[j].b_inodo=bmi;
                                    strcpy(bbc.b_content[j].b_name,auxiliar);
                                    fseek(file,sb.s_block_start,SEEK_SET);
                                    fseek(file,pos_aux2,SEEK_CUR);
                                    fwrite(&bbc,sizeof(bloqueCarpetas),1,file);
                                    bmi=bmi-1;
                                    bmi=bmi*sizeof(inodo);

                                    inodo ino2;
                                    time_t tiempo = time(0);

                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fread(&ino2,sizeof(inodo),1,file);

                                    ino2.i_uid = usuario;
                                    //ino2.i_gid = 1;
                                    ino2.i_size=0;
                                    ino2.i_atime = tiempo;
                                    ino2.i_ctime = tiempo;
                                    ino2.i_mtime = tiempo;
                                    ino2.i_type = tipo;
                                    ino2.i_perm = 664;
                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fwrite(&ino2,sizeof(inodo),1,file);
                                    flag_carpeta=0;
                                    break;
                                }
                            }
                        }else{
                            int bmi = buscarInodo(direccion,name);
                            int bmb = buscarBloque(direccion,name);
                            int pos_aux2 = bmb - 1;
                            pos_aux2 = pos_aux2*sizeof(bloqueCarpetas);

                            bloqueCarpetas bbc2;
                            bbc2.b_content[0].b_inodo=bmi;
                            strcpy(bbc2.b_content[0].b_name,auxiliar);
                            bbc2.b_content[1].b_inodo=0;
                            strcpy(bbc2.b_content[1].b_name,"");
                            bbc2.b_content[2].b_inodo=0;
                            strcpy(bbc2.b_content[2].b_name,"");
                            bbc2.b_content[3].b_inodo=0;
                            strcpy(bbc2.b_content[3].b_name,"");

                            fseek(file,sb.s_block_start,SEEK_SET);
                            fseek(file,pos_aux2,SEEK_CUR);
                            fwrite(&bbc2,sizeof(bloqueCarpetas),1,file);

                            ino.i_block[i]=bmb;
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,pos_aux,SEEK_CUR);
                            fwrite(&ino,sizeof(inodo),1,file);

                            bmi=bmi-1;
                            bmi=bmi*sizeof(inodo);

                            inodo ino2;
                            time_t tiempo = time(0);

                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,bmi,SEEK_CUR);
                            fread(&ino2,sizeof(inodo),1,file);

                            ino2.i_uid = usuario;
                            //ino2.i_gid = 1;
                            ino2.i_size=0;
                            ino2.i_atime = tiempo;
                            ino2.i_ctime = tiempo;
                            ino2.i_mtime = tiempo;
                            ino2.i_type = tipo;
                            ino2.i_perm = 664;
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,bmi,SEEK_CUR);
                            fwrite(&ino2,sizeof(inodo),1,file);
                            flag_carpeta=0;
                            break;
                        }
                        if(flag_carpeta==0){
                            break;
                        }
                    }
                }
                if(sb.s_filesystem_type==3&&flagJour==0){
                    journaling jour;
                    fseek(file,inicio,SEEK_SET);
                    fseek(file,sizeof(superBloque),SEEK_CUR);
                    fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                    fread(&jour,sizeof(journaling),1,file);

                    jour.journal_tipo_operacion=4;
                    time_t tiempo = time(0);
                    struct tm *tlocal = localtime(&tiempo);
                    char output[18]="";
                    strftime(output,18,"%d/%m/%y %H:%M:%S",tlocal);
                    jour.journal_fecha = tiempo;
                    strcpy(jour.journal_nombre,path);
                    jour.journal_permisos = 664;
                    jour.journal_propietario=usuario;
                    jour.journal_tipo=tipo;

                    fseek(file,inicio,SEEK_SET);
                    fseek(file,sizeof(superBloque),SEEK_CUR);
                    fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                    fwrite(&jour,sizeof(journaling),1,file);
                    numJournaling++;
                }
            }else{
                printf("\nERROR: El directorio no existe");
            }
            contador2++;
            fclose(file);
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
}

int firstInodo(char direccion[], char name[]){
    int i;
    int inicio;
    int bandera;
    superBloque sb;

    MBR disco;
    FILE *file;
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
            fseek(file,sb.s_bm_inode_start,SEEK_SET);
            char aux;
            for(i=1;i<=sb.s_inodes_count;++i){
                fread(&aux,1,1,file);
                if(aux=='0'){
                    break;
                }
            }
            sb.s_first_ino=i;
            fseek(file,inicio,SEEK_SET);
            fwrite(&sb,sizeof(superBloque),1,file);
            fclose(file);
            return i;
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
    return -1;
}

int firtBlock(char direccion[], char name[]){
    int i;
    int inicio;
    int bandera;
    superBloque sb;

    MBR disco;
    FILE *file;
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
            fseek(file,sb.s_bm_block_start,SEEK_SET);
            char aux;
            for(i=1;i<=sb.s_blocks_count;++i){
                fread(&aux,1,1,file);
                if(aux=='0'){
                    break;
                }
            }
            sb.s_first_blo=i;
            fseek(file,inicio,SEEK_SET);
            fwrite(&sb,sizeof(superBloque),1,file);
            fclose(file);
            return i;
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
    return -1;
}

int buscarInodo(char direccion[], char name[]){
    int i;
    int inicio;
    int bandera;
    superBloque sb;
    bloqueCarpetas bbc;
    bloqueArchivos bba;
    inodo ino;
    int inicio_bloque;
    int indice;
    int bmbloque;

    MBR disco;
    FILE *file;
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
            fseek(file,sb.s_bm_inode_start,SEEK_SET);
            char aux;
            for(i=1;i<=sb.s_inodes_count;++i){
                fread(&aux,1,1,file);
                if(aux=='0'){
                    break;
                }
            }
            fseek(file,sb.s_bm_inode_start,SEEK_SET);
            fseek(file,i-1,SEEK_CUR);
            fwrite("1",1,1,file);
            sb.s_free_inodes_count--;
            fseek(file,inicio,SEEK_SET);
            fwrite(&sb,sizeof(superBloque),1,file);
            fclose(file);
            return i;
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
    return -1;
}

int buscarBloque(char direccion[], char name[]){
    int i;
    int inicio;
    int bandera;
    superBloque sb;
    bloqueCarpetas bbc;

    MBR disco;
    FILE *file;
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
            fseek(file,sb.s_bm_block_start,SEEK_SET);
            char aux;
            for(i=1;i<=sb.s_blocks_count;++i){
                fread(&aux,1,1,file);
                if(aux=='0'){
                    break;
                }
            }
            fseek(file,sb.s_bm_block_start,SEEK_SET);
            fseek(file,i-1,SEEK_CUR);
            fwrite("1",1,1,file);
            sb.s_free_blocks_count--;
            fseek(file,inicio,SEEK_SET);
            fwrite(&sb,sizeof(superBloque),1,file);
            fclose(file);
            return i;
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
    return -1;
}

