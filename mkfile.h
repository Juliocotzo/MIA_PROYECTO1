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

typedef struct mkfile{
    char path[50];
    int size;
    char cont[50];
    char P[5]
} mkfile_init;

void mkfile(char path[], char p[], char size[], char cont[]);
void crearArchivo(char path[], int usuario, int size, char direccion[], char name[]);
void crearArchivo2(char path[], int usuario, int size, char direccion[], char name[]);
void crearArchivo3(char path[], int usuario, int size, char direccion[], char name[], char texto[], char cont[]);
void crearArchivo4(char path[], int usuario, int size, char direccion[], char name[], char texto[], char cont[]);

int processMKFILE(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS \n" RESET);
        return 0;
    }
    mkfile_init rm;
    memset(&rm.path,'\0',sizeof(rm.path));
    memset(&rm.cont,'\0',sizeof(rm.cont));
    memset(&rm.P,'\0',sizeof(rm.P));
    rm.size = 0;
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

            }else if(strcasecmp("SIZE", name) == 0){
                rm.size = atoi(buffer);

            }else if(strcasecmp("CONT", name) == 0){
                strcat(rm.cont, buffer);

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

        }else if(strcasecmp("SIZE", name) == 0){
            rm.size = atoi(buffer);

        }else if(strcasecmp("CONT", name) == 0){
            strcat(rm.cont, buffer);

        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE1\n" RESET);

            printf("ch:%c",ch);
            printf("buffer:%s",buffer);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    printf("\nPATH->%s|SIZE->%d|CONT->%s|P->%s|\n",rm.path,rm.size,rm.cont,rm.P);
    char array[64];
    int myInteger = rm.size;
    sprintf( array, "%d", myInteger );
    //printf( "%s\n", array );
    mkfile(rm.path,rm.P,array,rm.cont);

    return 0;
}

void mkfile(char path[], char p[], char size[], char cont[]){
    printf("\nPATH->%s|SIZE->%s|CONT->%s|P->%s|\n",path,size,cont,p);
    printf("\n********************MKFILE**************************");
    if(bandera_log==1){
        if(strcmp(path,"")!=0){
            if(strcmp(size,"")!=0){
                int auxiliar = atoi(id_activo3);
                int auxiliar2 = atoi(size);
                if(strcmp(p,"P")==0){
                    crearArchivo2(path,auxiliar,auxiliar2,path_activa,name_activo);
                    int sb1 = firstInodo(path_activa,name_activo);
                    int sb2 = firtBlock(path_activa,name_activo);
                }else if(strcmp(p,"")==0){
                    crearArchivo(path,auxiliar,auxiliar2,path_activa,name_activo);
                    int sb1 = firstInodo(path_activa,name_activo);
                    int sb2 = firtBlock(path_activa,name_activo);
                }else{
                    printf("\nERROR: El parametro -p esta mal declarado");
                }
            }else if(strcmp(cont,"")!=0){
                int auxiliar = atoi(id_activo3);
                char auxiliar2[10000]="";
                FILE * file;
                file = fopen(cont,"rt");
                if(file!=NULL){
                    fseek(file,0,SEEK_END);
                    int tamanio = ftell(file);
                    rewind(file);
                    fread(&auxiliar2,tamanio,1,file);
                    fclose(file);
                    if(strcmp(p,"P")==0){
                        crearArchivo4(path,auxiliar,tamanio,path_activa,name_activo,auxiliar2,cont);
                        int sb1 = firstInodo(path_activa,name_activo);
                        int sb2 = firtBlock(path_activa,name_activo);
                    }else if(strcmp(p,"")==0){
                        crearArchivo3(path,auxiliar,tamanio,path_activa,name_activo,auxiliar2,cont);
                        int sb1 = firstInodo(path_activa,name_activo);
                        int sb2 = firtBlock(path_activa,name_activo);
                    }else{
                        printf("\nERROR: El parametro -p esta mal declarado");
                    }
                }else{
                    printf("\nERROR: direccion incorrecta");
                }
            }else{
                int auxiliar = atoi(id_activo3);
                if(strcmp(p,"P")==0){
                    crearCarpeta2(path,auxiliar,path_activa,name_activo,1);
                    int sb1 = firstInodo(path_activa,name_activo);
                    int sb2 = firtBlock(path_activa,name_activo);
                }else if(strcmp(p,"")==0){
                    crearCarpeta(path,auxiliar,path_activa,name_activo,1);
                    int sb1 = firstInodo(path_activa,name_activo);
                    int sb2 = firtBlock(path_activa,name_activo);
                }else{
                    printf("\nERROR: El parametro -p esta mal declarado");
                }
            }
        }else{
            printf("\nERROR: El parametro path no fue indicado");
        }
    }else{
        printf("\nERROR: No se ha iniciado ninguna sesion");
    }
}


void crearArchivo(char path[], int usuario, int size, char direccion[], char name[]){
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
                        }else if(k==12){

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
                                    int bmi2 = firstInodo(direccion,name);
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
                                    ino2.i_size=size;
                                    ino2.i_atime = tiempo;
                                    ino2.i_ctime = tiempo;
                                    ino2.i_mtime = tiempo;
                                    ino2.i_type = '1';
                                    ino2.i_perm = 664;
                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fwrite(&ino2,sizeof(inodo),1,file);


                                    ///**************************************************************************
                                    int bmb;
                                    int bmb2 = firtBlock(direccion,name);
                                    int con_size=0;
                                    int numeros=0;
                                    int flag = 0;
                                    int cont2=0;
                                    char cambio[10]="";

                                    while(con_size<size){
                                        for(i=0;i<15;++i){
                                            if(i<12){
                                                bmb=buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                ino2.i_block[i]=bmb;
                                                char cadena[64]="";
                                                for(j=0;j<63;++j){
                                                    sprintf(cambio,"%d",numeros);
                                                    if(con_size<size){
                                                        cadena[j]=cambio[0];
                                                        numeros++;
                                                    }else{
                                                        flag=1;
                                                        break;
                                                    }
                                                    if(numeros>9){
                                                        numeros=0;
                                                    }
                                                    con_size++;
                                                }

                                                strcpy(bba.b_content,cadena);
                                                bmb=bmb-1;
                                                bmb=bmb*sizeof(bloqueArchivos);
                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmb,SEEK_CUR);
                                                fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                fseek(file,sb.s_inode_start,SEEK_SET);
                                                fseek(file,bmi,SEEK_CUR);
                                                fwrite(&ino2,sizeof(inodo),1,file);
                                                if(flag==1){
                                                    break;
                                                }
                                            }else if(i==12){
                                                bloqueApuntadores bap;
                                                int bmp =buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                ino2.i_block[i]=bmp;

                                                fseek(file,sb.s_inode_start,SEEK_SET);
                                                fseek(file,bmi,SEEK_CUR);
                                                fwrite(&ino2,sizeof(inodo),1,file);

                                                bmp=bmp-1;
                                                bmp=bmp*sizeof(bloqueApuntadores);

                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmp,SEEK_CUR);
                                                fwrite(&bap,sizeof(bloqueApuntadores),1,file);

                                                int q;
                                                for(q=0;q<16;++q){
                                                    bap.b_pointers[q]=0;
                                                }
                                                for(q=0;q<16;++q){
                                                    if(con_size<size){
                                                        bmb=buscarBloque(direccion,name);
                                                        bmb2 = firtBlock(direccion,name);
                                                        bap.b_pointers[q]=bmb;

                                                        char cadena[64]="";
                                                        for(j=0;j<63;++j){
                                                            sprintf(cambio,"%d",numeros);
                                                            if(con_size<size){
                                                                cadena[j]=cambio[0];
                                                                numeros++;
                                                            }else{
                                                                flag=1;
                                                                break;
                                                            }
                                                            if(numeros>9){
                                                                numeros=0;
                                                            }
                                                            con_size++;
                                                        }

                                                        strcpy(bba.b_content,cadena);
                                                        bmb=bmb-1;
                                                        bmb=bmb*sizeof(bloqueArchivos);
                                                        fseek(file,sb.s_block_start,SEEK_SET);
                                                        fseek(file,bmb,SEEK_CUR);
                                                        fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                        fseek(file,sb.s_block_start,SEEK_SET);
                                                        fseek(file,bmp,SEEK_CUR);
                                                        fwrite(&bap,sizeof(bloqueApuntadores),1,file);
                                                    }else{
                                                        break;
                                                    }
                                                }
                                                if(flag==1){
                                                    break;
                                                }
                                            }else if(i==13){
                                                flag=1;
                                            }else if(i==14){
                                                flag=1;
                                            }
                                        }
                                        if(flag==1){
                                            break;
                                        }
                                    }
                                    ///**************************************************************************
                                    flag_carpeta=0;
                                    break;
                                }
                            }
                        }else{

                            int bmi = buscarInodo(direccion,name);
                            int bmb = buscarBloque(direccion,name);
                            int bmi2 = firstInodo(direccion,name);
                            int bmb2 = firtBlock(direccion,name);
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
                            ino2.i_size=size;
                            ino2.i_atime = tiempo;
                            ino2.i_ctime = tiempo;
                            ino2.i_mtime = tiempo;
                            ino2.i_type = '1';
                            ino2.i_perm = 664;
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,bmi,SEEK_CUR);
                            fwrite(&ino2,sizeof(inodo),1,file);


                            int con_size=0;
                            int numeros=0;
                            int flag = 0;
                            int cont2=0;
                            char cambio[10]="";

                            while(con_size<size){
                                for(i=0;i<15;++i){
                                    if(i<12){
                                        bmb=buscarBloque(direccion,name);
                                        bmb2 = firtBlock(direccion,name);
                                        ino2.i_block[i]=bmb;
                                        char cadena[64]="";
                                        for(j=0;j<63;++j){
                                            sprintf(cambio,"%d",numeros);
                                            if(con_size<size){
                                                cadena[j]=cambio[0];
                                                numeros++;
                                            }else{
                                                flag=1;
                                                break;
                                            }
                                            if(numeros>9){
                                                numeros=0;
                                            }
                                            con_size++;
                                        }

                                        strcpy(bba.b_content,cadena);
                                        bmb=bmb-1;
                                        bmb=bmb*sizeof(bloqueArchivos);
                                        fseek(file,sb.s_block_start,SEEK_SET);
                                        fseek(file,bmb,SEEK_CUR);
                                        fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                        fseek(file,sb.s_inode_start,SEEK_SET);
                                        fseek(file,bmi,SEEK_CUR);
                                        fwrite(&ino2,sizeof(inodo),1,file);
                                        if(flag==1){
                                            break;
                                        }
                                    }else if(i==12){
                                        bloqueApuntadores bap;
                                        int bmp =buscarBloque(direccion,name);
                                        bmb2 = firtBlock(direccion,name);
                                        ino2.i_block[i]=bmp;

                                        fseek(file,sb.s_inode_start,SEEK_SET);
                                        fseek(file,bmi,SEEK_CUR);
                                        fwrite(&ino2,sizeof(inodo),1,file);

                                        bmp=bmp-1;
                                        bmp=bmp*sizeof(bloqueApuntadores);

                                        int q;
                                        for(q=0;q<16;++q){
                                            bap.b_pointers[q]=0;
                                        }
                                        for(q=0;q<16;++q){
                                            if(con_size<size){
                                                bmb=buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                bap.b_pointers[q]=bmb;

                                                char cadena[64]="";
                                                for(j=0;j<63;++j){
                                                    sprintf(cambio,"%d",numeros);
                                                    if(con_size<size){
                                                        cadena[j]=cambio[0];
                                                        numeros++;
                                                    }else{
                                                        flag=1;
                                                        break;
                                                    }
                                                    if(numeros>9){
                                                        numeros=0;
                                                    }
                                                    con_size++;
                                                }

                                                strcpy(bba.b_content,cadena);
                                                bmb=bmb-1;
                                                bmb=bmb*sizeof(bloqueArchivos);
                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmb,SEEK_CUR);
                                                fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmp,SEEK_CUR);
                                                fwrite(&bap,sizeof(bloqueApuntadores),1,file);
                                            }else{
                                                break;
                                            }
                                        }
                                        if(flag==1){
                                            break;
                                        }
                                    }else{
                                        flag=1;
                                    }
                                }
                            }
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
                    char cambio[10]="";
                    fseek(file,inicio,SEEK_SET);
                    fseek(file,sizeof(superBloque),SEEK_CUR);
                    fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                    fread(&jour,sizeof(journaling),1,file);

                    jour.journal_tipo_operacion=6;
                    time_t tiempo = time(0);
                    struct tm *tlocal = localtime(&tiempo);
                    char output[18]="";

                    sprintf(cambio,"%d",size);
                    strcpy(jour.journal_contenido,cambio);

                    strftime(output,18,"%d/%m/%y %H:%M:%S",tlocal);
                    jour.journal_fecha = tiempo;
                    strcpy(jour.journal_nombre,path);
                    jour.journal_permisos = 664;
                    jour.journal_propietario=usuario;
                    jour.journal_tipo=1;

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
void crearArchivo2(char path[], int usuario, int size, char direccion[], char name[]){
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
                    for(k=0;k<13;k++){
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
                        for(k=0;k<13;++k){
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
                                            int bmi2 = firstInodo(direccion,name);
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
                                            ino2.i_type = '0';
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
                                    int bmi2 = firstInodo(direccion,name);
                                    int bmb2 = firtBlock(direccion,name);
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
                                    ino2.i_type = '0';
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

            if(flag_carpeta==1){
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
                                }
                            }
                        }
                        if(flag_carpeta==1){
                            break;
                        }
                    }
                }
            }

            if(flag_carpeta==0){
                for(i=0;i<13;++i){
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
                                    int bmi2 = firstInodo(direccion,name);
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
                                    ino2.i_size=size;
                                    ino2.i_atime = tiempo;
                                    ino2.i_ctime = tiempo;
                                    ino2.i_mtime = tiempo;
                                    ino2.i_type = '1';
                                    ino2.i_perm = 664;
                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fwrite(&ino2,sizeof(inodo),1,file);


                                    ///**************************************************************************
                                    int bmb;
                                    int bmb2;
                                    int con_size=0;
                                    int numeros=0;
                                    int flag = 0;
                                    int cont2=0;
                                    char cambio[10]="";

                                    while(con_size<size){
                                        for(i=0;i<15;++i){
                                            if(i<12){
                                                bmb=buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                ino2.i_block[i]=bmb;
                                                char cadena[64]="";
                                                for(j=0;j<63;++j){
                                                    sprintf(cambio,"%d",numeros);
                                                    if(con_size<size){
                                                        cadena[j]=cambio[0];
                                                        numeros++;
                                                    }else{
                                                        flag=1;
                                                        break;
                                                    }
                                                    if(numeros>9){
                                                        numeros=0;
                                                    }
                                                    con_size++;
                                                }

                                                strcpy(bba.b_content,cadena);
                                                bmb=bmb-1;
                                                bmb=bmb*sizeof(bloqueArchivos);
                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmb,SEEK_CUR);
                                                fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                fseek(file,sb.s_inode_start,SEEK_SET);
                                                fseek(file,bmi,SEEK_CUR);
                                                fwrite(&ino2,sizeof(inodo),1,file);
                                                if(flag==1){
                                                    break;
                                                }
                                            }else if(i==12){
                                                bloqueApuntadores bap;
                                                int bmp =buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                ino2.i_block[i]=bmp;

                                                fseek(file,sb.s_inode_start,SEEK_SET);
                                                fseek(file,bmi,SEEK_CUR);
                                                fwrite(&ino2,sizeof(inodo),1,file);

                                                bmp=bmp-1;
                                                bmp=bmp*sizeof(bloqueApuntadores);

                                                int q;
                                                for(q=0;q<16;++q){
                                                    bap.b_pointers[q]=0;
                                                }
                                                for(q=0;q<16;++q){
                                                    if(con_size<size){
                                                        bmb=buscarBloque(direccion,name);
                                                        bmb2 = firtBlock(direccion,name);
                                                        bap.b_pointers[q]=bmb;

                                                        char cadena[64]="";
                                                        for(j=0;j<63;++j){
                                                            sprintf(cambio,"%d",numeros);
                                                            if(con_size<size){
                                                                cadena[j]=cambio[0];
                                                                numeros++;
                                                            }else{
                                                                flag=1;
                                                                break;
                                                            }
                                                            if(numeros>9){
                                                                numeros=0;
                                                            }
                                                            con_size++;
                                                        }

                                                        strcpy(bba.b_content,cadena);
                                                        bmb=bmb-1;
                                                        bmb=bmb*sizeof(bloqueArchivos);
                                                        fseek(file,sb.s_block_start,SEEK_SET);
                                                        fseek(file,bmb,SEEK_CUR);
                                                        fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                        fseek(file,sb.s_block_start,SEEK_SET);
                                                        fseek(file,bmp,SEEK_CUR);
                                                        fwrite(&bap,sizeof(bloqueApuntadores),1,file);
                                                    }else{
                                                        break;
                                                    }
                                                }
                                                if(flag==1){
                                                    break;
                                                }
                                            }else{
                                                flag=1;
                                            }
                                        }
                                    }
                                    ///**************************************************************************
                                    flag_carpeta=0;
                                    break;
                                }else{
                                    flag_carpeta=1;
                                }
                            }
                        }else{
                            int bmi = buscarInodo(direccion,name);
                            int bmb = buscarBloque(direccion,name);
                            int bmi2 = firstInodo(direccion,name);
                            int bmb2 = firtBlock(direccion,name);
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
                            ino2.i_size=size;
                            ino2.i_atime = tiempo;
                            ino2.i_ctime = tiempo;
                            ino2.i_mtime = tiempo;
                            ino2.i_type = '1';
                            ino2.i_perm = 664;
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,bmi,SEEK_CUR);
                            fwrite(&ino2,sizeof(inodo),1,file);


                            int con_size=0;
                            int numeros=0;
                            int flag = 0;
                            int cont2=0;
                            char cambio[10]="";

                            while(con_size<size){
                                for(i=0;i<13;++i){
                                    if(i<12){
                                        bmb=buscarBloque(direccion,name);
                                        bmb2 = firtBlock(direccion,name);
                                        ino2.i_block[i]=bmb;
                                        char cadena[64]="";
                                        for(j=0;j<63;++j){
                                            sprintf(cambio,"%d",numeros);
                                            if(con_size<size){
                                                cadena[j]=cambio[0];
                                                numeros++;
                                            }else{
                                                flag=1;
                                                break;
                                            }
                                            if(numeros>9){
                                                numeros=0;
                                            }
                                            con_size++;
                                        }

                                        strcpy(bba.b_content,cadena);
                                        bmb=bmb-1;
                                        bmb=bmb*sizeof(bloqueArchivos);
                                        fseek(file,sb.s_block_start,SEEK_SET);
                                        fseek(file,bmb,SEEK_CUR);
                                        fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                        fseek(file,sb.s_inode_start,SEEK_SET);
                                        fseek(file,bmi,SEEK_CUR);
                                        fwrite(&ino2,sizeof(inodo),1,file);
                                        if(flag==1){
                                            break;
                                        }
                                    }else if(i==12){
                                        bloqueApuntadores bap;
                                        int bmp =buscarBloque(direccion,name);
                                        bmb2 = firtBlock(direccion,name);
                                        ino2.i_block[i]=bmp;

                                        fseek(file,sb.s_inode_start,SEEK_SET);
                                        fseek(file,bmi,SEEK_CUR);
                                        fwrite(&ino2,sizeof(inodo),1,file);

                                        bmp=bmp-1;
                                        bmp=bmp*sizeof(bloqueApuntadores);

                                        int q;
                                        for(q=0;q<16;++q){
                                            bap.b_pointers[q]=0;
                                        }
                                        for(q=0;q<16;++q){
                                            if(con_size<size){
                                                bmb=buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                bap.b_pointers[q]=bmb;

                                                char cadena[64]="";
                                                for(j=0;j<63;++j){
                                                    sprintf(cambio,"%d",numeros);
                                                    if(con_size<size){
                                                        cadena[j]=cambio[0];
                                                        numeros++;
                                                    }else{
                                                        flag=1;
                                                        break;
                                                    }
                                                    if(numeros>9){
                                                        numeros=0;
                                                    }
                                                    con_size++;
                                                }

                                                strcpy(bba.b_content,cadena);
                                                bmb=bmb-1;
                                                bmb=bmb*sizeof(bloqueArchivos);
                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmb,SEEK_CUR);
                                                fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmp,SEEK_CUR);
                                                fwrite(&bap,sizeof(bloqueApuntadores),1,file);
                                            }else{
                                                break;
                                            }
                                        }
                                        if(flag==1){
                                            break;
                                        }
                                    }else{
                                        flag=1;
                                    }
                                }
                            }
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
                    char cambio[10]="";
                    fseek(file,inicio,SEEK_SET);
                    fseek(file,sizeof(superBloque),SEEK_CUR);
                    fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                    fread(&jour,sizeof(journaling),1,file);

                    jour.journal_tipo_operacion=7;
                    time_t tiempo = time(0);
                    struct tm *tlocal = localtime(&tiempo);
                    char output[18]="";

                    sprintf(cambio,"%d",size);
                    strcpy(jour.journal_contenido,cambio);

                    strftime(output,18,"%d/%m/%y %H:%M:%S",tlocal);
                    jour.journal_fecha = tiempo;
                    strcpy(jour.journal_nombre,path);
                    jour.journal_permisos = 664;
                    jour.journal_propietario=usuario;
                    jour.journal_tipo=1;

                    fseek(file,inicio,SEEK_SET);
                    fseek(file,sizeof(superBloque),SEEK_CUR);
                    fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                    fwrite(&jour,sizeof(journaling),1,file);
                    numJournaling++;
                }
            }else{
                printf("\nERROR: El directorio ya existe");
            }

            fclose(file);
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
}

void crearArchivo3(char path[], int usuario, int size, char direccion[], char name[], char texto[], char cont[]){
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
                                    int bmi2 = firstInodo(direccion,name);
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
                                    ino2.i_size=size;
                                    ino2.i_atime = tiempo;
                                    ino2.i_ctime = tiempo;
                                    ino2.i_mtime = tiempo;
                                    ino2.i_type = '1';
                                    ino2.i_perm = 664;
                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fwrite(&ino2,sizeof(inodo),1,file);


                                    ///**************************************************************************
                                    int bmb;
                                    int bmb2;
                                    int con_size=0;
                                    int numeros=0;
                                    int flag = 0;
                                    int cont2=0;
                                    char cambio[10]="";

                                    while(con_size<size){
                                        for(i=0;i<15;++i){
                                            if(i<12){
                                                bmb=buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                ino2.i_block[i]=bmb;
                                                char cadena[64]="";
                                                for(j=0;j<63;++j){
                                                    sprintf(cambio,"%d",numeros);
                                                    if(con_size<size){
                                                        cadena[j]=texto[cont2];
                                                        cont2++;
                                                    }else{
                                                        flag=1;
                                                        break;
                                                    }
                                                    con_size++;
                                                }

                                                strcpy(bba.b_content,cadena);
                                                bmb=bmb-1;
                                                bmb=bmb*sizeof(bloqueArchivos);
                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmb,SEEK_CUR);
                                                fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                fseek(file,sb.s_inode_start,SEEK_SET);
                                                fseek(file,bmi,SEEK_CUR);
                                                fwrite(&ino2,sizeof(inodo),1,file);

                                                //printf("\nNUMEROS: %s",cadena);
                                                if(flag==1){
                                                    break;
                                                }
                                            }else if(i==12){
                                                bloqueApuntadores bap;
                                                int bmp =buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                ino2.i_block[i]=bmp;

                                                fseek(file,sb.s_inode_start,SEEK_SET);
                                                fseek(file,bmi,SEEK_CUR);
                                                fwrite(&ino2,sizeof(inodo),1,file);

                                                bmp=bmp-1;
                                                bmp=bmp*sizeof(bloqueApuntadores);

                                                int q;
                                                for(q=0;q<16;++q){
                                                    bap.b_pointers[q]=0;
                                                }
                                                for(q=0;q<16;++q){
                                                    if(con_size<size){
                                                        bmb=buscarBloque(direccion,name);
                                                        bmb2 = firtBlock(direccion,name);
                                                        bap.b_pointers[q]=bmb;

                                                        char cadena[64]="";
                                                        for(j=0;j<63;++j){
                                                            if(con_size<size){
                                                                cadena[j]=texto[cont2];
                                                                cont2++;
                                                            }else{
                                                                flag=1;
                                                                break;
                                                            }
                                                            con_size++;
                                                        }

                                                        strcpy(bba.b_content,cadena);
                                                        bmb=bmb-1;
                                                        bmb=bmb*sizeof(bloqueArchivos);
                                                        fseek(file,sb.s_block_start,SEEK_SET);
                                                        fseek(file,bmb,SEEK_CUR);
                                                        fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                        fseek(file,sb.s_block_start,SEEK_SET);
                                                        fseek(file,bmp,SEEK_CUR);
                                                        fwrite(&bap,sizeof(bloqueApuntadores),1,file);
                                                    }else{
                                                        break;
                                                    }
                                                }
                                                if(flag==1){
                                                    break;
                                                }
                                            }else{
                                                flag=1;
                                                break;
                                            }
                                        }
                                    }
                                    ///**************************************************************************
                                    flag_carpeta=0;
                                    break;
                                }
                            }
                        }else{
                            int bmi = buscarInodo(direccion,name);
                            int bmb = buscarBloque(direccion,name);
                            int bmi2 = firstInodo(direccion,name);
                            int bmb2 = firtBlock(direccion,name);
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
                            ino2.i_size=size;
                            ino2.i_atime = tiempo;
                            ino2.i_ctime = tiempo;
                            ino2.i_mtime = tiempo;
                            ino2.i_type = '1';
                            ino2.i_perm = 664;
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,bmi,SEEK_CUR);
                            fwrite(&ino2,sizeof(inodo),1,file);


                            int con_size=0;
                            int numeros=0;
                            int flag = 0;
                            int cont2=0;
                            char cambio[10]="";

                            while(con_size<size){
                                for(i=0;i<15;++i){
                                    if(i<12){
                                        bmb=buscarBloque(direccion,name);
                                        bmb2 = firtBlock(direccion,name);
                                        ino2.i_block[i]=bmb;
                                        char cadena[64]="";
                                        for(j=0;j<63;++j){
                                            sprintf(cambio,"%d",numeros);
                                            if(con_size<size){
                                                cadena[j]=texto[cont2];
                                                cont2++;
                                            }else{
                                                flag=1;
                                                break;
                                            }
                                            con_size++;
                                        }

                                        strcpy(bba.b_content,cadena);
                                        bmb=bmb-1;
                                        bmb=bmb*sizeof(bloqueArchivos);
                                        fseek(file,sb.s_block_start,SEEK_SET);
                                        fseek(file,bmb,SEEK_CUR);
                                        fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                        fseek(file,sb.s_inode_start,SEEK_SET);
                                        fseek(file,bmi,SEEK_CUR);
                                        fwrite(&ino2,sizeof(inodo),1,file);

                                        //printf("\nNUMEROS: %s",cadena);
                                        if(flag==1){
                                            break;
                                        }
                                    }else if(i==12){
                                        bloqueApuntadores bap;
                                        int bmp =buscarBloque(direccion,name);
                                        bmb2 = firtBlock(direccion,name);
                                        ino2.i_block[i]=bmp;

                                        fseek(file,sb.s_inode_start,SEEK_SET);
                                        fseek(file,bmi,SEEK_CUR);
                                        fwrite(&ino2,sizeof(inodo),1,file);

                                        bmp=bmp-1;
                                        bmp=bmp*sizeof(bloqueApuntadores);

                                        int q;
                                        for(q=0;q<16;++q){
                                            bap.b_pointers[q]=0;
                                        }
                                        for(q=0;q<16;++q){
                                            if(con_size<size){
                                                bmb=buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                bap.b_pointers[q]=bmb;

                                                char cadena[64]="";
                                                for(j=0;j<63;++j){
                                                    if(con_size<size){
                                                        cadena[j]=texto[cont2];
                                                        cont2++;
                                                    }else{
                                                        flag=1;
                                                        break;
                                                    }
                                                    con_size++;
                                                }

                                                strcpy(bba.b_content,cadena);
                                                bmb=bmb-1;
                                                bmb=bmb*sizeof(bloqueArchivos);
                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmb,SEEK_CUR);
                                                fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmp,SEEK_CUR);
                                                fwrite(&bap,sizeof(bloqueApuntadores),1,file);
                                            }else{
                                                break;
                                            }
                                        }
                                        if(flag==1){
                                            break;
                                        }
                                    }else{
                                        flag=1;
                                        break;
                                    }
                                }
                            }
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

                    jour.journal_tipo_operacion=8;
                    time_t tiempo = time(0);
                    struct tm *tlocal = localtime(&tiempo);
                    char output[18]="";

                    strcpy(jour.journal_contenido,cont);

                    strftime(output,18,"%d/%m/%y %H:%M:%S",tlocal);
                    jour.journal_fecha = tiempo;
                    strcpy(jour.journal_nombre,path);
                    jour.journal_permisos = 664;
                    jour.journal_propietario=usuario;
                    jour.journal_tipo=1;

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

void crearArchivo4(char path[], int usuario, int size, char direccion[], char name[], char texto[], char cont[]){
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
                                            int bmi2 = firstInodo(direccion,name);
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
                                            ino2.i_type = '0';
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
                                    int bmi2 = firstInodo(direccion,name);
                                    int bmb2 = firtBlock(direccion,name);
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
                                    ino2.i_type = '0';
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
                                    int bmi2 = firstInodo(direccion,name);
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
                                    ino2.i_size=size;
                                    ino2.i_atime = tiempo;
                                    ino2.i_ctime = tiempo;
                                    ino2.i_mtime = tiempo;
                                    ino2.i_type = '1';
                                    ino2.i_perm = 664;
                                    fseek(file,sb.s_inode_start,SEEK_SET);
                                    fseek(file,bmi,SEEK_CUR);
                                    fwrite(&ino2,sizeof(inodo),1,file);


                                    ///**************************************************************************
                                    int bmb;
                                    int bmb2;
                                    int con_size=0;
                                    int numeros=0;
                                    int flag = 0;
                                    int cont2=0;
                                    char cambio[10]="";

                                    while(con_size<size){
                                        for(i=0;i<15;++i){
                                            if(i<12){
                                                bmb=buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                ino2.i_block[i]=bmb;
                                                char cadena[64]="";
                                                for(j=0;j<63;++j){
                                                    sprintf(cambio,"%d",numeros);
                                                    if(con_size<size){
                                                        cadena[j]=texto[cont2];
                                                        cont2++;
                                                    }else{
                                                        flag=1;
                                                        break;
                                                    }
                                                    con_size++;
                                                }

                                                strcpy(bba.b_content,cadena);
                                                bmb=bmb-1;
                                                bmb=bmb*sizeof(bloqueArchivos);
                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmb,SEEK_CUR);
                                                fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                fseek(file,sb.s_inode_start,SEEK_SET);
                                                fseek(file,bmi,SEEK_CUR);
                                                fwrite(&ino2,sizeof(inodo),1,file);

                                                //printf("\nNUMEROS: %s",cadena);
                                                if(flag==1){
                                                    break;
                                                }
                                            }else if(i==12){
                                                bloqueApuntadores bap;
                                                int bmp =buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                ino2.i_block[i]=bmp;

                                                fseek(file,sb.s_inode_start,SEEK_SET);
                                                fseek(file,bmi,SEEK_CUR);
                                                fwrite(&ino2,sizeof(inodo),1,file);

                                                bmp=bmp-1;
                                                bmp=bmp*sizeof(bloqueApuntadores);

                                                int q;
                                                for(q=0;q<16;++q){
                                                    bap.b_pointers[q]=0;
                                                }
                                                for(q=0;q<16;++q){
                                                    if(con_size<size){
                                                        bmb=buscarBloque(direccion,name);
                                                        bmb2 = firtBlock(direccion,name);
                                                        bap.b_pointers[q]=bmb;

                                                        char cadena[64]="";
                                                        for(j=0;j<63;++j){
                                                            if(con_size<size){
                                                                cadena[j]=texto[cont2];
                                                                cont2++;
                                                            }else{
                                                                flag=1;
                                                                break;
                                                            }
                                                            con_size++;
                                                        }

                                                        strcpy(bba.b_content,cadena);
                                                        bmb=bmb-1;
                                                        bmb=bmb*sizeof(bloqueArchivos);
                                                        fseek(file,sb.s_block_start,SEEK_SET);
                                                        fseek(file,bmb,SEEK_CUR);
                                                        fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                        fseek(file,sb.s_block_start,SEEK_SET);
                                                        fseek(file,bmp,SEEK_CUR);
                                                        fwrite(&bap,sizeof(bloqueApuntadores),1,file);
                                                    }else{
                                                        break;
                                                    }
                                                }
                                                if(flag==1){
                                                    break;
                                                }
                                            }else{
                                                flag=1;
                                                break;
                                            }
                                        }
                                    }
                                    ///**************************************************************************
                                    flag_carpeta=0;
                                    break;
                                }else{
                                    flag_carpeta=1;
                                }
                            }
                        }else{
                            int bmi = buscarInodo(direccion,name);
                            int bmi2 = firstInodo(direccion,name);
                            int bmb = buscarBloque(direccion,name);
                            int bmb2 = firtBlock(direccion,name);
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
                            ino2.i_size=size;
                            ino2.i_atime = tiempo;
                            ino2.i_ctime = tiempo;
                            ino2.i_mtime = tiempo;
                            ino2.i_type = '1';
                            ino2.i_perm = 664;
                            fseek(file,sb.s_inode_start,SEEK_SET);
                            fseek(file,bmi,SEEK_CUR);
                            fwrite(&ino2,sizeof(inodo),1,file);

                            int con_size=0;
                            int numeros=0;
                            int flag = 0;
                            int cont2=0;
                            char cambio[10]="";

                            while(con_size<size){
                                for(i=0;i<15;++i){
                                    if(i<12){
                                        bmb=buscarBloque(direccion,name);
                                        bmb2=firtBlock(direccion,name);
                                        ino2.i_block[i]=bmb;
                                        char cadena[64]="";
                                        for(j=0;j<63;++j){
                                            sprintf(cambio,"%d",numeros);
                                            if(con_size<size){
                                                cadena[j]=texto[cont2];
                                                cont2++;
                                            }else{
                                                flag=1;
                                                break;
                                            }
                                            con_size++;
                                        }

                                        strcpy(bba.b_content,cadena);
                                        bmb=bmb-1;
                                        bmb=bmb*sizeof(bloqueArchivos);
                                        fseek(file,sb.s_block_start,SEEK_SET);
                                        fseek(file,bmb,SEEK_CUR);
                                        fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                        fseek(file,sb.s_inode_start,SEEK_SET);
                                        fseek(file,bmi,SEEK_CUR);
                                        fwrite(&ino2,sizeof(inodo),1,file);

                                        //printf("\nNUMEROS: %s",cadena);
                                        if(flag==1){
                                            break;
                                        }
                                    }else if(i==12){
                                        bloqueApuntadores bap;
                                        int bmp =buscarBloque(direccion,name);
                                        bmb2 = firtBlock(direccion,name);
                                        ino2.i_block[i]=bmp;

                                        fseek(file,sb.s_inode_start,SEEK_SET);
                                        fseek(file,bmi,SEEK_CUR);
                                        fwrite(&ino2,sizeof(inodo),1,file);

                                        bmp=bmp-1;
                                        bmp=bmp*sizeof(bloqueApuntadores);

                                        int q;
                                        for(q=0;q<16;++q){
                                            bap.b_pointers[q]=0;
                                        }
                                        for(q=0;q<16;++q){
                                            if(con_size<size){
                                                bmb=buscarBloque(direccion,name);
                                                bmb2 = firtBlock(direccion,name);
                                                bap.b_pointers[q]=bmb;

                                                char cadena[64]="";
                                                for(j=0;j<63;++j){
                                                    if(con_size<size){
                                                        cadena[j]=texto[cont2];
                                                        cont2++;
                                                    }else{
                                                        flag=1;
                                                        break;
                                                    }
                                                    con_size++;
                                                }

                                                strcpy(bba.b_content,cadena);
                                                bmb=bmb-1;
                                                bmb=bmb*sizeof(bloqueArchivos);
                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmb,SEEK_CUR);
                                                fwrite(&bba,sizeof(bloqueArchivos),1,file);

                                                fseek(file,sb.s_block_start,SEEK_SET);
                                                fseek(file,bmp,SEEK_CUR);
                                                fwrite(&bap,sizeof(bloqueApuntadores),1,file);
                                            }else{
                                                break;
                                            }
                                        }
                                        if(flag==1){
                                            break;
                                        }
                                    }else{
                                        flag=1;
                                        break;
                                    }
                                }
                            }
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

                    jour.journal_tipo_operacion=9;
                    time_t tiempo = time(0);
                    struct tm *tlocal = localtime(&tiempo);
                    char output[18]="";

                    strcpy(jour.journal_contenido,cont);

                    strftime(output,18,"%d/%m/%y %H:%M:%S",tlocal);
                    jour.journal_fecha = tiempo;
                    strcpy(jour.journal_nombre,path);
                    jour.journal_permisos = 664;
                    jour.journal_propietario=usuario;
                    jour.journal_tipo=1;

                    fseek(file,inicio,SEEK_SET);
                    fseek(file,sizeof(superBloque),SEEK_CUR);
                    fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                    fwrite(&jour,sizeof(journaling),1,file);
                    numJournaling++;
                }
            }else{
                printf("\nERROR: El directorio ya existe");
            }

            fclose(file);
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo %s",direccion);
    }
}
