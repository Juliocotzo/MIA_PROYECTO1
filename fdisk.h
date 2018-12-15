#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
//#include "estructuras.h"

const char * FDISK_UNIT_B = "B";
const char * FDISK_UNIT_K = "K";
const char * FDISK_UNIT_M = "M";
const char * FDISK_TIPO_P = "P";
const char * FDISK_TIPO_E = "E";
const char * FDISK_TIPO_L = "L";
const char FDISK_FIT_BF = 'B';
const char FDISK_FIT_FF = 'F';
const char FDISK_FIT_WF = 'W';
const char * FDISK_DELETE_FAST = "FAST";
const char * FDISK_DELETE_FULL = "FULL";


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

typedef struct intervalo{
    int i_start;
    int i_end;
} i_part;

typedef struct fdisk_init{
    int size;
    char unit[5];
    char path[80];
    char type[5];
    char fit[5];
    char deletePartition[5];
    char name[25];
    int addPartition;


} fdisk_init;

void administrarParticiones(int size, char unit[], char path[], char type[], char fit[], char delet[], char name[],int add);
void crearParticion(int size, char path[], char name[], char type[], char unit[], char fit[]);
int verificarParticiones(char path[]);
int primerAjuste(int primer, char path[], int tamanio);
void escribirParticion(char type, char fit, int size, char name[], char path[], int indice);
//Logicas
int primerEBR(char path[]);
int particionExtendida(char path[]);
void insertarLogica(char path[],int tamanio, char name[], char fit);


void administrarParticiones(int size, char unit[], char path[], char type[], char fit[], char delet[], char name[],int add){
    MBR disco;
    if(strcmp(path,"")!=0  || strcmp(name,"")!=0){
        FILE * file;
        file = fopen(path,"rb");
        if(file!=NULL){
            if(strcmp(delet,"")!=0){
                printf("\nELIMINAR\n");
            }else if(add>0){
                printf("\nAGREGAR ESPACIO\n");
            }else if(size>0){
                crearParticion(size,path,name,type,unit,fit);
            }else{
                printf("\nERROR: No se especifico la accion a realizar (Crear, eliminar o agregar una particion)");
            }
        }else{
            printf("\nERROR: La ruta del disco duro no existe");
            return 1;
        }
        fclose(file);
    }else{
        printf("\nERROR: faltan parametros");
    }
}


void crearParticion(int size, char path[], char name[], char type[], char unit[], char fit[]){
    printf("CREACION DE PARTICION");
    int tamanio;
    int idtipo;

    int errorUnidad;
    int errorTipo;

    idtipo=0;

    errorUnidad=0;
    errorTipo=0;

    if(strcmp(unit,"B")==0 || strcmp(unit,"b")==0){
        tamanio = size;
    }else if (strcmp(unit,"k")==0 || strcmp(unit,"K")==0 || strcmp(unit,"")==0) {
        tamanio = size  *1024;
    }else if (strcmp(unit,"m")==0 || strcmp(unit,"M")==0){
        tamanio = size*1024*1024;
    }else{
        errorUnidad=1;
    }

    if(errorUnidad==1){
        printf("\nERROR: Unit no reconoce %s como parametro",unit);
    }else{



        idtipo = verificarParticiones(path);
        if(strcmp(type,"P")==0 || strcmp(type,"p")==0 || strcmp(type,"")==0){
            if(idtipo==0||idtipo==10||idtipo==20||idtipo==30||idtipo==1||idtipo==11||idtipo==21){
                printf("\nCrear particion primaria");

                char ajuste;
                if(strcmp(fit,"B")==0){
                    ajuste='b';
                }else if(strcmp(fit,"F")==0){
                    ajuste='f';
                }else if(strcmp(fit,"W")==0||strcmp(fit,"")==0){
                    ajuste='w';
                }else{
                    ajuste='e';
                    printf("\nERROR: Parametro de ajuste incorrecto");
                }


                if(ajuste!='e'){
                    int numeroParticion = primerAjuste(0,path,tamanio);
                    if(numeroParticion>=0 && numeroParticion<4){
                        escribirParticion('p',ajuste,tamanio,name,path,numeroParticion);
                    }else{
                        printf("\nERROR:La particion no puede ser creada\n");
                    }
                }else{
                    printf("\nERROR: Parametro de ajuste incorrecto\n");
                }
            }else{
                printf("\nERROR:No se puede crear la particion primaria\n");
            }
        }else if (strcmp(type,"E")==0 || strcmp(type,"e")==0) {
            int indice;
            if(idtipo==0||idtipo==10||idtipo==20||idtipo==30){
                //printf("\nCrear particion extendida\n");
                char ajuste='e';

                if(strcmp(fit,"B")==0){
                    ajuste='b';
                }else if(strcmp(fit,"F")==0||strcmp(fit,"")==0){
                    ajuste='f';
                }else if(strcmp(fit,"W")==0){
                    ajuste='w';
                }

                if(ajuste!='e'){
                    int numeroParticion = primerAjuste(0,path,tamanio);
                    indice = numeroParticion;
                    if(numeroParticion>=0 && numeroParticion<4){
                        escribirParticion('e',ajuste,tamanio,name,path,numeroParticion);

                        EBR_P particionLogica;
                        MBR disco;
                        FILE * file;
                        int start;

                        file = fopen(path,"rb+");
                        fread(&disco,sizeof(MBR),1,file);
                        start = disco.mbr_partition[indice].part_start;

                        particionLogica.part_fit='w';
                        strcpy(particionLogica.part_name,"");
                        particionLogica.part_next=-1;
                        particionLogica.part_size=-1;
                        particionLogica.part_start=start;
                        particionLogica.part_status=PARTITION_STATUS_NOT_ACTIVE;

                        fseek(file,start,SEEK_SET);
                        fwrite(&particionLogica,sizeof(EBR_P),1,file);

                        fclose(file);
                    }else{
                        printf("\nERROR:La particion no puede ser creada\n");
                    }
                }else{
                    printf("\nERROR: Parametro de ajuste incorrecto\n");
                }
            }else{
                printf("\nERROR:No se puede crear la particion extendida\n");
            }
        }else if (strcmp(type,"L")==0 || strcmp(type,"l")==0){
            if(idtipo==1||idtipo==11||idtipo==21||idtipo==31){
                printf("\nCrear particion logica");
                char ajuste='e';

                if(strcmp(fit,"B")==0){
                    ajuste='b';
                }else if(strcmp(fit,"F")==0||strcmp(fit,"")==0){
                    ajuste='f';
                }else if(strcmp(fit,"W")==0){
                    ajuste='w';
                }

                if(ajuste!='e'){
                    int aux = primerEBR(path);;
                    if(aux==-1){
                        printf("\nERROR: No se puede crear la particion logica porque no existe ninguna particion extendida\n");
                    }else if(aux==0){
                        printf("\nSe creara el primer EBR\n");

                        int indice = particionExtendida(path);

                        EBR_P particionLogica;
                        MBR disco;
                        FILE * file;
                        int start;

                        file = fopen(path,"rb+");
                        fread(&disco,sizeof(MBR),1,file);
                        start = disco.mbr_partition[indice].part_start;
                        if(tamanio>sizeof(EBR_P)&&tamanio<disco.mbr_partition[indice].part_size){
                            particionLogica.part_fit='w';
                            strcpy(particionLogica.part_name,name);
                            particionLogica.part_size=tamanio;
                            particionLogica.part_start=start;
                            particionLogica.part_next=-1;
                            particionLogica.part_status=PARTITION_STATUS_ACTIVE;

                            fseek(file,start,SEEK_SET);
                            fwrite(&particionLogica,sizeof(EBR_P),1,file);
                        }else{
                            printf("\nERROR: La particion logica es demasiado grande o muy pequenia\n");
                        }
                        fclose(file);


                    }else if(aux>0){
                        insertarLogica(path,tamanio,name,ajuste);
                    }
                }else{
                    printf("\nERROR:Parametro inexistente\n");
                }
            }else{
                printf("\nERROR:No se puede crear la particion logica porque no existe ninguna particion extendida\n");
            }
        }else{
            errorTipo=1;
        }
    }
}

int verificarParticiones(char path[]){
    MBR disco;
    int cont_particiones=0;
    int cont_primarias=0;
    int cont_extendidas=0;
    FILE * file;
    file = fopen(path,"rb");
    if(file!=NULL){
        rewind(file);
        fread(&disco,sizeof(MBR),1,file);
        int i;
        for(i=0;i<4;++i){
            if(disco.mbr_partition[i].part_status==PARTITION_STATUS_ACTIVE){
                if(disco.mbr_partition[i].part_type=='p'||disco.mbr_partition[i].part_type=='P'){
                    cont_primarias++;
                }else if(disco.mbr_partition[i].part_type=='e'||disco.mbr_partition[i].part_type=='E'){
                    cont_extendidas++;
                }
            }
        }
        cont_particiones=(cont_primarias*10)+cont_extendidas;
    }else{
        printf("\nERROR: La ruta del disco duro no existe\n");
        return 1;
    }
    fclose(file);
    return cont_particiones;
}

int primerAjuste(int primer, char path[], int tamanio){
    MBR disco;
    FILE * file;
    int segundo;
    int particion;

    segundo = 0;
    particion = -1;
    file = fopen(path,"rb");
    fread(&disco,sizeof(MBR),1,file);

    int i;
    for(i=primer; i<4;++i){
        if(disco.mbr_partition[i].part_status==PARTITION_STATUS_NOT_ACTIVE){
            break;
        }
    }
    int j;
    for(j=i; j<4;++j){
        if(disco.mbr_partition[j].part_status==PARTITION_STATUS_ACTIVE){
            break;
        }
    }

    if(j<4){
        if(i==0){
            /*printf("\nCERO");
            printf("\nTamanio DISCO: %i",disco.mbr_tamanio);
            printf("\nTamanio MBR: %i",sizeof(MBR));
            printf("\nTamanio DISCO: %i",disco.mbr_tamanio);
            printf("\nTamanio PARTICION: %i",tamanio);
            printf("\nTamanio DISPONIBLE: %i",disco.mbr_tamanio-sizeof(MBR));*/
            int disponible = disco.mbr_partition[j].part_start-sizeof(MBR);
            if(disponible>=tamanio){
                particion = i;
                printf("\nSE PUEDE ESCRIBIR EN LA PARTICION: %i\n",particion);
            }else{

                particion = -1;
                printf("\nNO CABE LA PARTICION\n");
            }
        }else{
            /*printf("\nDISTINTO DE CERO");
            printf("\nTamanio DISCO: %i",disco.mbr_tamanio);
            printf("\nTamanio INICIO: %i",disco.mbr_particion[i-1].part_start+disco.mbr_particion[i-1].part_size);
            printf("\nTamanio DISCO: %i",disco.mbr_tamanio);
            printf("\nTamanio PARTICION: %i",tamanio);
            printf("\nTamanio DISPONIBLE: %i",disco.mbr_tamanio-sizeof(MBR));*/
            int disponible = disco.mbr_partition[j].part_start-(disco.mbr_partition[i-1].part_start+disco.mbr_partition[i-1].part_size);
            if(disponible>=tamanio){
                particion = i;
                printf("\nSE PUEDE ESCRIBIR EN LA PARTICION: %i",particion);
            }else{

                particion = -1;
                printf("\nNO CABE LA PARTICION\n");
            }
        }
    }else{
        if(i==0){
            /*printf("\nCERO");
            printf("\nTamanio DISCO: %i",disco.mbr_tamanio);
            printf("\nTamanio MBR: %i",sizeof(MBR));
            printf("\nTamanio DISCO: %i",disco.mbr_tamanio);
            printf("\nTamanio PARTICION: %i",tamanio);
            printf("\nTamanio DISPONIBLE: %i",disco.mbr_tamanio-sizeof(MBR));*/
            int disponible = disco.mbr_tamanio-sizeof(MBR);
            if(disponible>=tamanio){
                particion = i;
                printf("\nSE PUEDE ESCRIBIR EN LA PARTICION: %i\n",particion);
            }else{
                particion = -1;
                printf("\nNO CABE LA PARTICION\n");
            }
        }else{
            /*printf("\nDISTINTO DE CERO");
            printf("\nTamanio DISCO: %i",disco.mbr_tamanio);
            printf("\nTamanio INICIO: %i",disco.mbr_particion[i-1].part_start+disco.mbr_particion[i-1].part_size);
            printf("\nTamanio DISCO: %i",disco.mbr_tamanio);
            printf("\nTamanio PARTICION: %i",tamanio);
            printf("\nTamanio DISPONIBLE: %i",disco.mbr_tamanio-sizeof(MBR));*/
            int disponible = disco.mbr_tamanio-(disco.mbr_partition[i-1].part_start+disco.mbr_partition[i-1].part_size);
            if(disponible>=tamanio){
                particion = i;
                printf("\nSE PUEDE ESCRIBIR EN LA PARTICION: %i",particion);
            }else{
                particion = -1;
                printf("\nNO CABE LA PARTICION");
            }
        }
    }
    return particion;
}
void escribirParticion(char type, char fit, int size, char name[], char path[], int indice){
    MBR disco;
    FILE * file;
    int start;

    file = fopen(path,"rb");
    fread(&disco,sizeof(MBR),1,file);
    fclose(file);

    if(indice==0){
        start=sizeof(MBR);
    }else{
        start=disco.mbr_partition[indice-1].part_start+disco.mbr_partition[indice-1].part_size;
    }

    disco.mbr_partition[indice].part_fit=fit;
    strcpy(disco.mbr_partition[indice].part_name,name);
    disco.mbr_partition[indice].part_size=size;
    disco.mbr_partition[indice].part_start=start;
    disco.mbr_partition[indice].part_status=PARTITION_STATUS_ACTIVE;
    disco.mbr_partition[indice].part_type=type;

    file = fopen(path,"rb+");
    rewind(file);
    fwrite(&disco,sizeof(MBR),1,file);
    fclose(file);


}

int primerEBR(char path[]){
    MBR disco;
    int start;
    int cont_extendidas=-1;
    FILE * file;
    file = fopen(path,"rb");
    if(file!=NULL){
        rewind(file);
        fread(&disco,sizeof(MBR),1,file);
        int i;
        for(i=0;i<4;++i){
            if(disco.mbr_partition[i].part_type=='e'||disco.mbr_partition[i].part_type=='E'){
                cont_extendidas=i;
                break;
            }
        }
    }else{
        printf("\nERROR: La ruta del disco duro no existe");
        return 1;
    }

    int primer=0;
    if(cont_extendidas>=0&&cont_extendidas<4){
        rewind(file);
        int next=-1;
        int next2;
        start = disco.mbr_partition[cont_extendidas].part_start;
        next2 = disco.mbr_partition[cont_extendidas].part_start;
        do{
            fseek(file,next2,SEEK_SET);
            EBR_P logica;
            fread(&logica,sizeof(EBR_P),1,file);
            next = logica.part_next;
            if(logica.part_status==PARTITION_STATUS_ACTIVE){
                primer=primer+1;
            }
            next2 = logica.part_next;
        }while(next!=-1);

    }else{
        primer=-1;
    }
    fclose(file);
    return primer;
}

int particionExtendida(char path[]){
    MBR disco;
    int cont_extendidas=-1;
    FILE * file;
    file = fopen(path,"rb");
    if(file!=NULL){
        rewind(file);
        fread(&disco,sizeof(MBR),1,file);
        int i;
        for(i=0;i<4;++i){
            if(disco.mbr_partition[i].part_type=='e'||disco.mbr_partition[i].part_type=='E'){
                cont_extendidas=i;
                break;
            }
        }
    }else{
        printf("\nERROR: La ruta del disco duro no existe");
        return 1;
    }
    fclose(file);
    return cont_extendidas;
}

void insertarLogica(char path[],int tamanio, char name[],char fit){
    int indice;
    int indice_logico;
    int bandera=0;
    int inicio=-1;
    int fin=-1;
    int limite=-1;

    char vacio;
    FILE * file;
    MBR disco;
    EBR_P part;
    EBR_P nuevo;

    indice = particionExtendida(path);
    file = fopen(path,"rb+");
    if(file!=NULL){
        fread(&disco,sizeof(MBR),1,file);
        indice_logico=disco.mbr_partition[indice].part_start;
        int anterior2;
        do{
            fseek(file,indice_logico,SEEK_SET);
            fread(&part,sizeof(EBR_P),1,file);
            anterior2=part.part_start;
            if(part.part_next==-1){
                inicio=part.part_size+part.part_start;
                fin=disco.mbr_partition[indice].part_start+disco.mbr_partition[indice].part_size;
                limite=inicio+tamanio;
                bandera=1;
            }else{
                if(part.part_next!=(part.part_size+part.part_start)){
                    indice_logico=part.part_next;
                    bandera=1;
                    //break;
                }else{
                    indice_logico=part.part_next;
                    //break;
                }
            }
        }while(bandera==0);
    }

    if(limite<=fin){
        fseek(file,part.part_start,SEEK_SET);
        part.part_next=inicio;
        fwrite(&part,sizeof(EBR_P),1,file);

        strcpy(nuevo.part_name,name);
        nuevo.part_fit=fit;
        nuevo.part_status=PARTITION_STATUS_ACTIVE;
        nuevo.part_next=-1;
        nuevo.part_size=tamanio;
        nuevo.part_start=inicio;

        fseek(file,inicio,SEEK_SET);
        printf("\nEmpieza el ebr ->%d\n",inicio);
        fwrite(&nuevo,sizeof(EBR_P),1,file);
    }
    fclose(file);
}


int processFDISK(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR : COMANDO SIN PARAMETROS\n" RESET);
        return 0;
    }
    // Size Disk
    int sizeDisk = 1024;

    // Command MKDISK
    fdisk_init fc;
    memset(&fc.name,'\0',sizeof(fc.name));
    memset(&fc.path,'\0',sizeof(fc.path));
    memset(&fc.unit,'\0',sizeof(fc.unit));
    memset(&fc.type,'\0',sizeof(fc.type));
    memset(&fc.fit,'\0',sizeof(fc.fit));
    memset(&fc.deletePartition,'\0',sizeof(fc.deletePartition));
    memset(&fc.addPartition,'\0',sizeof(fc.addPartition));
    strcat(fc.unit, "K");
    strcat(fc.type, "P");
    strcat(fc.fit, "W");
    // Base Program
    char ch, buffer[100];
    memset(&buffer,'\0',sizeof(buffer));
    int j=0;
    bool read = true;
    char name[15];
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
        if(ch == '#'){
            break;
        }
        if(isalnum(ch) || ch == '/' || ch == '.' || ch == '_'){
            buffer[j++] = ch;
        }
        if(ch == ' ' && !read){
            buffer[j++] = ch;
        }else if((ch == ' ' || ch == '\n') && (j != 0) && read){
            buffer[j] = '\0';
            if(strcasecmp("SIZE", name) == 0){
                fc.size = atoi(buffer);
            }else if(strcasecmp("ADD", name) == 0){
                fc.addPartition = atoi(buffer);
            }else if(strcasecmp("PATH", name) == 0){
                strcat(fc.path, buffer);
            }else if(strcasecmp("NAME", name) == 0){
                strcat(fc.name, buffer);
            }else if(strcasecmp("TYPE", name) == 0){
                memset(&fc.type,'\0',sizeof(fc.type));
                if(strcasecmp("P",buffer) == 0){
                    strcat(fc.type, "P");
                    sizeDisk = 1024 * 1024;
                }else if(strcasecmp("E",buffer) == 0){
                    strcat(fc.type, "E");
                }else if(strcasecmp("L",buffer) == 0){
                    strcat(fc.type, "L");
                }
            }else if(strcasecmp("FIT", name) == 0){
                memset(&fc.fit,'\0',sizeof(fc.fit));
                if(strcasecmp("BF",buffer) == 0){
                    strcat(fc.fit, "B");
                }else if(strcasecmp("FF",buffer) == 0){
                    strcat(fc.fit, "F");
                }else if(strcasecmp("WF",buffer) == 0){
                    strcat(fc.fit, "W");
                }


            }else if(strcasecmp("DELETE", name) == 0){
                if(strcasecmp("FAST",buffer) == 0){
                    strcat(fc.deletePartition, "FAST");
                }else if(strcasecmp("FULL",buffer) == 0){
                    strcat(fc.deletePartition, "FULL");
                }
            }else if(strcasecmp("UNIT", name) == 0){
                memset(&fc.unit,'\0',sizeof(fc.unit));

                if(strcasecmp("M",buffer) == 0){
                    strcat(fc.unit, "M");
                    sizeDisk = 1024 * 1024;
                }else if(strcasecmp("K",buffer) == 0){
                    strcat(fc.unit, "K");
                    sizeDisk = 1024;
                }else if(strcasecmp("B",buffer) == 0){
                    strcat(fc.unit, "B");
                    sizeDisk = 1;
                }else{
                    printf(RED "ERROR : NO ES UN VALOR PARA EL COMANDO UNIT\n" RESET);
                    return 0;
                }
            }
            else{
                // Errores
                printf(RED "ERROR : COMANDO ERRONEO\n" RESET);
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    }
    if((j != 0) && read){
        buffer[j] = '\0';
        if(strcasecmp("SIZE", name) == 0){
            fc.size = atoi(buffer);
        }else if(strcasecmp("ADD", name) == 0){
            fc.addPartition = atoi(buffer);
        }else if(strcasecmp("PATH", name) == 0){
            strcat(fc.path, buffer);
        }else if(strcasecmp("NAME", name) == 0){
            strcat(fc.name, buffer);
        }else if(strcasecmp("TYPE", name) == 0){
            memset(&fc.type,'\0',sizeof(fc.type));
            if(strcasecmp("P",buffer) == 0){
                strcat(fc.type, "P");
                sizeDisk = 1024 * 1024;
            }else if(strcasecmp("E",buffer) == 0){
                strcat(fc.type, "E");
            }else if(strcasecmp("L",buffer) == 0){
                strcat(fc.type, "L");
            }
        }else if(strcasecmp("FIT", name) == 0){
            memset(&fc.fit,'\0',sizeof(fc.fit));
            if(strcasecmp("BF",buffer) == 0){
                strcat(fc.fit, "B");
            }else if(strcasecmp("FF",buffer) == 0){
                strcat(fc.fit, "F");
            }else if(strcasecmp("WF",buffer) == 0){
                strcat(fc.fit, "W");
            }
        }else if(strcasecmp("DELETE", name) == 0){
            if(strcasecmp("FAST",buffer) == 0){
                strcat(fc.deletePartition, "FAST");
            }else if(strcasecmp("FULL",buffer) == 0){
                strcat(fc.deletePartition, "FULL");
            }
        }else if(strcasecmp("UNIT", name) == 0){
            memset(&fc.unit,'\0',sizeof(fc.unit));
            if(strcasecmp("M",buffer) == 0){
                strcat(fc.unit, "M");
                sizeDisk = 1024 * 1024;
            }else if(strcasecmp("K",buffer) == 0){
                strcat(fc.unit, "K");
                sizeDisk = 1024;
            }else if(strcasecmp("B",buffer) == 0){
                strcat(fc.unit, "B");
                sizeDisk = 1;
            }else{
                printf(RED "ERROR : NO ES UN VALOR PARA EL COMANDO UNIT\n" RESET);
            return 0;
            }
        }else{
            // Errores
            printf(RED "ERROR : COMANDO ERRONEO\n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }
    administrarParticiones(fc.size,fc.unit,fc.path,fc.type,fc.fit,fc.deletePartition,fc.name,fc.addPartition);


    return 0;
}
