#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include "estructuras.h"

#define RED   "\x1B[1;31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[1;36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

const char PARTITION_STATUS_NOT_ACTIVE = '0';
const char PARTITION_STATUS_ACTIVE = '1';
const char PARTITION_TYPE_PRIMARY = 'P';
const char PARTITION_TYPE_EXTENDED = 'E';

typedef struct mkdisk{
    int size;
    char path[80];
    char name[25];
    char unit[5];
    char fit[5];
} mk_init;

void configEmptyPartition(MBR_P * partition){
    partition->part_status = PARTITION_STATUS_NOT_ACTIVE;
}

int processMKDISK(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS\n" RESET);
        return 0;
    }
    // Size Disk
    int sizeDisk = 1024*1024;
    // Command MKDISK
    mk_init mk;
    memset(&mk.name,'\0',sizeof(mk.name));
    memset(&mk.path,'\0',sizeof(mk.path));
    memset(&mk.unit,'\0',sizeof(mk.unit));
    memset(&mk.fit,'\0',sizeof(mk.fit));
    strcat(mk.unit, "M");
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
            if(strcasecmp("SIZE", name) == 0){
                if(atoi(buffer) < 0){
                    printf(RED "ERROR: DEBE SER UN NUMERO POSITIVO Y MAYOR A 0\n" RESET);
                    return 0;
                }
                mk.size = atoi(buffer);
            }else if(strcasecmp("PATH", name) == 0){
                strcat(mk.path, buffer);
            }else if(strcasecmp("FIT", name) == 0){
                if(strcasecmp("BF",buffer) == 0){
                    strcat(mk.unit, "BF");
                }else if(strcasecmp("FF",buffer) == 0){
                    strcat(mk.unit, "FF");
                }else if(strcasecmp("WF",buffer) == 0){
                    strcat(mk.unit, "WF");
                }else{
                    printf(RED "ERROR: PARAMETRO ERRONEO\n" RESET);
                }
            }else if(strcasecmp("UNIT", name) == 0){
                memset(&mk.unit,'\0',sizeof(mk.unit));
                if(strcasecmp("M",buffer) == 0){
                    strcat(mk.unit, "M");
                    sizeDisk = 1024 * 1024;
                }else if(strcasecmp("K",buffer) == 0){
                    strcat(mk.unit, "K");
                    sizeDisk = 1024;
                }else{
                    printf(RED "ERROR: PARAMETRO ERRONERO\n" RESET);
                }
            }else{
                printf(RED "ERROR: COMANDO ERRONEO\n" RESET);
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    }
    if((j != 0) && read){
        buffer[j] = '\0';
        //printf("----%s\n",buffer);
        // Posible optimizacion
        if(strcasecmp("SIZE", name) == 0){
            if(atoi(buffer) < 0){
                printf(RED "ERROR: DEBE SER UN NUMERO POSITIVO Y MAYOR A 0\n" RESET);
                return 0;
            }
            mk.size = atoi(buffer);
        }else if(strcasecmp("PATH", name) == 0){
            strcat(mk.path, buffer);
        }else if(strcasecmp("FIT", name) == 0){
            if(strcasecmp("BF",buffer) == 0){
                strcat(mk.unit, "BF");
            }else if(strcasecmp("FF",buffer) == 0){
                strcat(mk.unit, "FF");
            }else if(strcasecmp("WF",buffer) == 0){
                strcat(mk.unit, "WF");
            }else{
                printf(RED "ERROR: PARAMETRO ERRONEO\n" RESET);
            }
        }else if(strcasecmp("UNIT", name) == 0){
            memset(&mk.unit,'\0',sizeof(mk.unit));
            if(strcasecmp("M",buffer) == 0){
                strcat(mk.unit, "M");
                sizeDisk = 1024 * 1024;
            }else if(strcasecmp("K",buffer) == 0){
                strcat(mk.unit, "K");
                sizeDisk = 1024;
            }else{
                printf(RED "ERROR: PARAMETRO ERRONERO\n" RESET);
            }
        }else{
            // Errores
            printf(RED "ERROR: COMANDO ERRONEO \n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }


    char *last = strrchr(mk.path, '/');
    int size_Name = 0;
    if (last != NULL) {
        //printf("%s\n", last+1);
        strcat(mk.name,last+1);
        size_Name = strlen(last+1);

    }
    char substr[100];
    strcpy(substr,mk.path);
    memset(&mk.path,'\0',sizeof(mk.path));

    //printf("%d",strlen(substr));
    strncat(mk.path, substr, strlen(substr)-size_Name);




    printf("Tamanio: %d ",mk.size);
    printf("Unit: %s ",mk.unit);
    printf("Path: %s ",mk.path);
    printf("Name: %s\n",mk.name);



    //CREAR DISCO.DSK
    char* comando = (char*)malloc(500);
    memset(&comando[0], 0, sizeof(comando));
    // Crear Path
    DIR* dir = opendir(mk.path);
    if (dir){
        /* Directory exists. */
        closedir(dir);
    }
    else if (ENOENT == errno){
        strcat(comando, "mkdir -p \"");
        strcat(comando, mk.path);
        strcat(comando, "\"");
        system(comando);

    }else{
    /* opendir() failed for some other reason. */
    }

    char nul = '\0';
    int size_for = 0;



    //*Path
    char* pathaux=(char*)malloc(150);
    char* pathperm=(char*)malloc(150);
    char* permission=(char*)malloc(150);
    memset(&pathperm[0], 0, sizeof(pathaux));
    memset(&pathaux[0], 0, sizeof(pathaux));
    memset(&permission[0], 0, sizeof(permission));
    strcat(pathaux, mk.path);
    strcat(pathaux, mk.name);
    strcat(pathperm, "\"");
    strcat(pathperm, mk.path);
    strcat(pathperm, mk.name);
    strcat(pathperm, "\"");

    if(strcasecmp(mk.unit,"M")==0){
        size_for = 1024*1024;
    }else if(strcasecmp(mk.unit,"k")==0){
        size_for = 1024;
    }
    FILE *file;
    file= fopen(pathaux,"wb+");
    for (int i = 0; i < size_for*mk.size; i++) {
        fwrite(&nul,sizeof(nul),1,file);
    }
    fclose(file);

    strcat(permission,"chmod 777 ");
    strcat(permission, pathperm);
    system(permission);


    // Crear MBR
    MBR diskInfo;
    MBR_P blankPart;
    blankPart.part_start = 0;
    memset(&blankPart.part_name,'\0',sizeof(blankPart.part_name));
    strcat(blankPart.part_name,"");
    for(int var=0;var<4;++var){
        diskInfo.mbr_partition[var] = blankPart;
    }

    for(int var=0;var<4;++var){
        configEmptyPartition(&(diskInfo.mbr_partition[var]));
    }
    diskInfo.mbr_tamanio = mk.size * sizeDisk;
    memset(&diskInfo.mbr_fecha_creacion,0,sizeof(diskInfo.mbr_fecha_creacion));
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    strftime(diskInfo.mbr_fecha_creacion,16,"%d/%m/%y %H:%M",tlocal);
    srand(time(NULL));
    diskInfo.mbr_disk_signature = rand();

    FILE* disk = fopen(pathaux, "rb+");
    fseek(disk, 0, SEEK_SET);
    fwrite(&diskInfo,sizeof(MBR),1,disk);
    MBR test;
    //*Situar cursor al inicio del archivo
    fseek(disk, 0, SEEK_SET);
    fread(&test,sizeof(MBR),1,disk);
    free(pathaux);
    fclose(disk);

    return 0;
}
