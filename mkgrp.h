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

extern int numJournaling=0;
extern int flagJour = 0;

typedef struct mkgrp{
    char name[15];
} mkgrp_init;

void mkgrp(char name[]);
void editaruser(char texto[], char direccion[], char name[]);

int processMKGRP(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS \n" RESET);
        return 0;
    }
    mkgrp_init rm;
    memset(&rm.name,'\0',sizeof(rm.name));
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
            if(strcasecmp("NAME", name) == 0){
                strcat(rm.name, buffer);
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
        if(strcasecmp("NAME", name) == 0){
            strcat(rm.name, buffer);
        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    printf("\nName->%s|\n",rm.name);
    mkgrp(rm.name);


    return 0;
}
void mkgrp(char name[]){
    printf("\n********************MKGRP**************************");
    if(bandera_log==1&&strcmp(usuario,"root")==0){
        if(strcmp(name,"")!=0){
            char aux[100]="";
            idg++;
            sprintf(aux,"%d",idg);
            strcat(aux,", G, ");
            strcat(aux,name);
            strcat(aux,"\n");
            printf("\n%s,%s,%s\n",aux,path_activa,name_activo);
            editaruser(aux,path_activa,name_activo);
        }else{
            printf("\nERROR: El parametro name no fue indicado");
        }
    }else{
        printf("\nERROR: No se ha iniciado ninguna sesion o el usuario no es root");
    }
}

void editaruser(char texto[], char direccion[], char name[]){
    int i;
    int inicio;
    int bandera;
    superBloque sb;
    bloqueCarpetas bc;
    bloqueArchivos ba;
    inodo ino;
    int inicio_bloque;
    int indice;
    char text_aux[2000]="";
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

            for(i=0;i<13;++i){
                if(ino.i_block[i]==-1){
                    break;
                }
            }
            if(i!=0){
                i=i-1;
            }
            int principio=0;
            bmbloque = ino.i_block[i]-1;

            fseek(file,sb.s_bm_block_start,SEEK_SET);
            fseek(file,bmbloque,SEEK_CUR);
            fwrite("0",1,1,file);

            fseek(file,sb.s_block_start,SEEK_SET);
            fseek(file,bmbloque*sizeof(bloqueArchivos),SEEK_CUR);
            fread(&ba,sizeof(bloqueArchivos),1,file);
            fseek(file,-1*sizeof(bloqueArchivos),SEEK_CUR);
            strcat(text_aux,ba.b_content);
            strcat(text_aux,texto);

            int cont1=0;
            int cont2=strlen(text_aux);

            char reinicio[100]="";
            int cont3=0;

            int bloque;
            int bt = 0;

            while(cont1<cont2){
                char caracter[65]="";
                int k;
                for(k=0;k<63;k++){
                    if(cont1<cont2){
                        caracter[k]=text_aux[cont1];
                        cont1++;
                    }else{
                        char aux[10]="\0";
                        caracter[k]=aux[0];
                        break;
                    }
                }
                strcpy(ba.b_content,caracter);
                fwrite(&ba.b_content,sizeof(bloqueArchivos),1,file);
                if(bt==0){
                    fseek(file,sb.s_bm_block_start,SEEK_SET);
                    fseek(file,bmbloque,SEEK_CUR);
                    fwrite("1",1,1,file);

                    time_t tiempo = time(0);
                    ino.i_mtime=tiempo;

                    fseek(file,sb.s_inode_start,SEEK_SET);
                    fseek(file,pos_aux,SEEK_CUR);
                    fwrite(&ino,sizeof(inodo),1,file);

                    for(k=1;k<=sb.s_blocks_count;++k){
                        char a;
                        fread(&a,1,1,file);
                        if(a=='0'){
                            break;
                        }
                    }
                    k=k-1;
                    fseek(file,sb.s_block_start,SEEK_SET);
                    fseek(file,sizeof(bloqueArchivos)*k,SEEK_CUR);
                    bt=1;
                }else{
                    fseek(file,sb.s_bm_block_start,SEEK_SET);
                    for(k=1;k<=sb.s_blocks_count;++k){
                        char a;
                        fread(&a,1,1,file);
                        if(a=='0'){
                            break;
                        }
                    }
                    int l;
                    for(l=0;l<13;++l){
                        if(ino.i_block[l]==-1){
                            ino.i_block[l]=k;
                            break;
                        }
                    }



                    time_t tiempo = time(0);
                    ino.i_mtime=tiempo;

                    fseek(file,sb.s_inode_start,SEEK_SET);
                    fseek(file,pos_aux,SEEK_CUR);
                    fwrite(&ino,sizeof(inodo),1,file);

                    k=k-1;

                    fseek(file,sb.s_bm_block_start,SEEK_SET);
                    fseek(file,k,SEEK_CUR);
                    fwrite("1",1,1,file);

                    fseek(file,sb.s_block_start,SEEK_SET);
                    fseek(file,sizeof(bloqueArchivos)*k,SEEK_CUR);
                    fwrite(&ba.b_content,sizeof(bloqueArchivos),1,file);//************************************PODRIA SER BORRADO PROBAR SI NO FUNCIONA BIEN
                }
            }
            fclose(file);
            int inodo_libre = 0;
            int bloque_libre = 0;
            int cont_bloque = 0;
            int cont_inodo = 0;
            char act;

            file = fopen(direccion,"rb+");
            fseek(file,sb.s_bm_block_start,SEEK_SET);
            for(i=0;i<sb.s_blocks_count;++i){
                fread(&act,1,1,file);
                if(act=='1'){
                    cont_bloque++;
                }
            }
            fseek(file,sb.s_bm_block_start,SEEK_SET);
            for(i=1;i<sb.s_blocks_count;++i){
                fread(&act,1,1,file);
                if(act=='0'){
                    bloque_libre=i;
                    break;
                }
            }

            fseek(file,sb.s_bm_inode_start,SEEK_SET);
            for(i=0;i<sb.s_inodes_count;++i){
                fread(&act,1,1,file);
                if(act=='1'){
                    cont_inodo++;
                }else{
                    inodo_libre=i;
                }
            }
            fseek(file,sb.s_bm_inode_start,SEEK_SET);
            for(i=1;i<sb.s_inodes_count;++i){
                fread(&act,1,1,file);
                if(act=='0'){
                    inodo_libre=i;
                    break;
                }
            }
            printf("\n %i, %i, %i, %i",cont_bloque,bloque_libre,cont_inodo,inodo_libre);

            sb.s_first_blo=bloque_libre;
            sb.s_first_ino=inodo_libre;
            sb.s_free_blocks_count=sb.s_blocks_count-cont_bloque;
            sb.s_free_inodes_count=sb.s_inodes_count-cont_inodo;
            printf("\n %i, %i, %i, %i",sb.s_first_blo,sb.s_first_ino,sb.s_free_blocks_count,sb.s_free_inodes_count);
            fseek(file,inicio,SEEK_SET);
            fwrite(&sb,sizeof(superBloque),1,file);

            if(sb.s_filesystem_type==3&&flagJour==0){
                journaling jour;
                fseek(file,inicio,SEEK_SET);
                fseek(file,sizeof(superBloque),SEEK_CUR);
                fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                fread(&jour,sizeof(journaling),1,file);

                jour.journal_tipo_operacion=1;
                jour.journal_propietario=1;
                char output[18]="";
                time_t tiempo = time(0);
                struct tm *tlocal = localtime(&tiempo);
                strftime(output,18,"%d/%m/%y %H:%M:%S",tlocal);

                jour.journal_fecha = tiempo;
                strcpy(jour.journal_contenido,texto);

                fseek(file,inicio,SEEK_SET);
                fseek(file,sizeof(superBloque),SEEK_CUR);
                fseek(file,sizeof(journaling)*numJournaling,SEEK_CUR);
                fwrite(&jour,sizeof(journaling),1,file);
                numJournaling++;
            }
            fclose(file);
        }else{
            printf("\nERROR: Particion no existente");
        }
    }else{
        printf("\nERROR: No se pudo abrir el archivo");
    }
}
