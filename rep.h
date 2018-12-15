#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
//#include "estructuras.h"
//#include "mount.h";

#define RED   "\x1B[1;31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[1;36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#include <sys/types.h>
#include <sys/stat.h>

const char * REP_NOMBRE_MBR = "MBR";
const char * REP_NOMBRE_DISK = "DISK";

/*const char PARTITION_STATUS_NOT_ACTIVE = 'F';
const char PARTITION_STATUS_ACTIVE = 'T';
const char PARTITION_TYPE_PRIMARY = 'P';
const char PARTITION_TYPE_EXTENDED = 'E';*/


typedef struct rep{
    char id[15];
    char name[15];
    char path[80];
} rep_init;

void reporteDISK(char path[], char path2[]);
void reporteMBR(char path[], char path2[]);

char *removeExtension(char* mystr) {
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}


int processREP(char str[], int cont){
    char graf[1024];
    memset(&graf,'\0',sizeof(graf));
    strcat(graf, "digraph G {\n node [shape=record];\n  rankdir=LR\n");
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS \n" RESET);
        return 0;
    }
    rep_init rm;
    memset(&rm.id,'\0',sizeof(rm.id));
    memset(&rm.name,'\0',sizeof(rm.name));
    memset(&rm.path,'\0',sizeof(rm.path));
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
            else if(strcasecmp("NAME", name) == 0){
                memset(&rm.name,'\0',sizeof(rm.name));
                if(strcasecmp("MBR",buffer) == 0){
                    strcat(rm.name, "MBR");
                }else if(strcasecmp("DISK",buffer) == 0){
                    strcat(rm.name, "DISK");
                }
            }
            else if(strcasecmp("PATH", name) == 0){
                char *last = strrchr(buffer, '.');
                int size_Name = 0;
                if (last != NULL) {
                    size_Name = strlen(last);
                }
                char substr[100];
                char substr1[100];
                memset(&substr,'\0',sizeof(substr));
                memset(&substr1,'\0',sizeof(substr1));
                strcpy(substr,buffer);
                strncat(substr1, substr, strlen(substr)-size_Name);
                strcat(rm.path, substr1);
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
        }else if(strcasecmp("NAME", name) == 0){
             memset(&rm.name,'\0',sizeof(rm.name));
             if(strcasecmp("MBR",buffer) == 0){
                 strcat(rm.name, "MBR");
             }else if(strcasecmp("DISK",buffer) == 0){
                 strcat(rm.name, "DISK");
             }
        }else if(strcasecmp("PATH", name) == 0){
             char *last = strrchr(buffer, '.');
             int size_Name = 0;
             if (last != NULL) {
                 size_Name = strlen(last);
             }
             char substr[100];
             char substr1[100];
             memset(&substr,'\0',sizeof(substr));
             memset(&substr1,'\0',sizeof(substr1));
             strcpy(substr,buffer);
             strncat(substr1, substr, strlen(substr)-size_Name);
             strcat(rm.path, substr1);
        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE \n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    /*
     *
     * DISK
     *
     * */

    struct nodo *actual;
    char aux[100];
    int p = 1;
    int bandera = 0;
    actual = primero;
    char direccion[100];


    struct nodo *aux2;
    struct nodo *actual2;
    int nodo_eliminar;
    strcpy(aux,rm.id);
    while (actual != NULL) {
        if (strcmp(aux,actual->id)==0) {
            bandera=1;
            strcpy(direccion,actual->path);
            printf("\n%s\n",actual->path);


            break;
        }
        actual=actual->siguiente;

    }

    //printf("\n H: %s \n",direccion);
    //printf("\n H: %s \n",actual->path);

    if(strcasecmp(rm.name, "DISK") == 0){
        if(bandera ==1){
            //printf("DOT: %s\n",rm.path);
            //printf("DISK: %s\n",aux.path);
            // fileNameDestino creacion
            char *dirc, *basec, *bname, *dname;
            dirc = strdup(rm.path);
            //basec = strdup(rm.path);
            dname = dirname(dirc);
            //bname = basename(basec);

            //SPLIT
            char *last = strrchr(rm.path, '/');
            int size_Name = 0;
            if (last != NULL) {
                //printf("%s\n", last+1);
                //strcat(mk.name,last+1);
                size_Name = strlen(last+1);

            }
            char substr[100];
            char substr1[100];
            memset(&substr,'\0',sizeof(substr));
            memset(&substr1,'\0',sizeof(substr1));

            strcpy(substr,rm.path);
            strncat(substr1, substr, strlen(substr)-size_Name);

           char* comando1 = (char*)malloc(500);
           memset(&comando1[0], 0, sizeof(comando1));
           // Crear Path
           DIR* dir = opendir(substr1);
           if (dir){
               /* Directory exists. */
            closedir(dir);
           }
           else if (ENOENT == errno){
               strcat(comando1, "mkdir -p \"");
               strcat(comando1, substr1);
               strcat(comando1, "\"");
               system(comando1);

           }
           memset(&substr1,'\0',sizeof(substr1));

            reporteDISK(actual->path,rm.path);





        }
        else{
            printf(RED "Particion no existe." RESET);
            return 0;
        }
        /*
         *
         * REPORTE MBR
         *
         *
         */

    }else if (strcasecmp(rm.name, "MBR") == 0){
        if (bandera==1) {
            //printf("DOT: %s\n",rm.path);
            //printf("DISK: %s\n",aux.path);
            // fileNameDestino creacion
            char *dirc, *basec, *bname, *dname;
            dirc = strdup(rm.path);
            //basec = strdup(rm.path);
            dname = dirname(dirc);
            //bname = basename(basec);

            //SPLIT
            char *last = strrchr(rm.path, '/');
            int size_Name = 0;
            if (last != NULL) {
                //printf("%s\n", last+1);
                //strcat(mk.name,last+1);
                size_Name = strlen(last+1);

            }
            char substr[100];
            char substr1[100];
            memset(&substr,'\0',sizeof(substr));
            memset(&substr1,'\0',sizeof(substr1));

            strcpy(substr,rm.path);
            strncat(substr1, substr, strlen(substr)-size_Name);

           char* comando1 = (char*)malloc(500);
           memset(&comando1[0], 0, sizeof(comando1));
           // Crear Path
           DIR* dir = opendir(rm.path);
           if (dir){
               /* Directory exists. */
            closedir(dir);
           }
           else if (ENOENT == errno){
               strcat(comando1, "mkdir -p \"");
               strcat(comando1, substr1);
               strcat(comando1, "\"");
               system(comando1);

           }else{
               printf("\nNO EXISTE DISCO\n");
           }
           memset(&substr1,'\0',sizeof(substr1));

           reporteMBR(actual->path,rm.path);
        }else {
            printf(RED "\nNO EXISTE EL DISCO\n" RESET);
        }
    }


    return 0;
}

void reporteMBR(char path[], char path2[]){
    char cambio[10];
    char cambio2[10];
    char cambio3[10];
    char cambio4[10];
    MBR disco;
    FILE * file;
    file=fopen(path,"rb+");
    fread(&disco,sizeof(MBR),1,file);
    char grafo[10000]="";
    char grafo2[10000]="";
    fclose(file);
    int extendida = particionExtendida(path);
    int inicioLogica;
    if(extendida==-1){

    }else{
        MBR disco2;
        FILE * file2;
        file2 = fopen(path,"rb");
        if(file2!=NULL){
            rewind(file2);
            fread(&disco2,sizeof(MBR),1,file2);
            inicioLogica=disco2.mbr_partition[extendida].part_start;
            EBR_P logica;
            int i=2;
            char copia[2]="";
            do{
                fseek(file2,inicioLogica,SEEK_SET);
                fread(&logica,sizeof(EBR_P),1,file2);
                sprintf(cambio3,"%d",i);
                if(logica.part_status==PARTITION_STATUS_ACTIVE){
                    strcat(grafo2,"struct");
                    strcat(grafo2,cambio3);
                    strcat(grafo2," [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n");
                    strcat(grafo2,"<tr><td><b>Nombre</b></td><td><b>Valor</b></td></tr>\n");


                    strcat(grafo2,"<tr>");
                    strcat(grafo2,"<td>Part_status");
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"<td>");
                    copia[0]=logica.part_status;
                    strcat(grafo2,copia);
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"</tr>\n");

                    strcat(grafo2,"<tr>");
                    strcat(grafo2,"<td>Part_fit");
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"<td>");
                    copia[0]=logica.part_fit;
                    strcat(grafo2,copia);
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"</tr>\n");

                    strcat(grafo2,"<tr>");
                    strcat(grafo2,"<td>Part_start");
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"<td>");
                    sprintf(cambio2,"%d",logica.part_start);
                    strcat(grafo2,cambio2);
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"</tr>\n");

                    strcat(grafo2,"<tr>");
                    strcat(grafo2,"<td>Part_Size");
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"<td>");
                    sprintf(cambio2,"%d",logica.part_size);
                    strcat(grafo2,cambio2);
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"</tr>\n");

                    strcat(grafo2,"<tr>");
                    strcat(grafo2,"<td>Part_Next");
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"<td>");
                    sprintf(cambio2,"%d",logica.part_next);
                    strcat(grafo2,cambio2);
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"</tr>\n");

                    strcat(grafo2,"<tr>");
                    strcat(grafo2,"<td>Part_Name");
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"<td>");
                    strcat(grafo2,logica.part_name);
                    strcat(grafo2,"</td>");
                    strcat(grafo2,"</tr>\n");
                    strcat(grafo2,"</table>>];\n");
                    i++;
                }
                inicioLogica=logica.part_next;
            }while(logica.part_next!=-1);
        }else{

        }
        fclose(file2);
    }
    //printf("\n\n\n*******************************\n%s\n*******************************",grafo2);
    file =fopen(path2,"wb+");
    //printf("\n\n************************");
    strcat(grafo,"digraph structs{\n");
    strcat(grafo,"node [shape=record];");
    strcat(grafo,"struct1 [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n");
    strcat(grafo,"<tr><td><b>Nombre</b></td><td><b>Valor</b></td></tr>\n");

    strcat(grafo,"<tr>");
    strcat(grafo,"<td>MBR_Tamanio</td>");
    strcat(grafo,"<td>");
    sprintf(cambio,"%d",disco.mbr_tamanio);
    strcat(grafo,cambio);
    strcat(grafo,"</td>");
    strcat(grafo,"</tr>\n");

    strcat(grafo,"<tr>");
    strcat(grafo,"<td>MBR_Fecha</td>");
    strcat(grafo,"<td>");
    strcat(grafo,disco.mbr_fecha_creacion);
    strcat(grafo,"</td>");
    strcat(grafo,"</tr>\n");

    strcat(grafo,"<tr>");
    strcat(grafo,"<td>MBR_Disk_Asignature</td>");
    strcat(grafo,"<td>");
    sprintf(cambio,"%d",disco.mbr_disk_signature);
    strcat(grafo,cambio);
    strcat(grafo,"</td>");
    strcat(grafo,"</tr>\n");

    int i;
    for(i=0;i<4;++i){
        sprintf(cambio,"%d",i+1);
        char copia[2]="";

        if(disco.mbr_partition[i].part_status=='1'){
            strcat(grafo,"<tr>");
            strcat(grafo,"<td>Part_status_");
            strcat(grafo,cambio);
            strcat(grafo,"</td>");
            strcat(grafo,"<td>");
            copia[0]=disco.mbr_partition[i].part_status;
            strcat(grafo,copia);
            strcat(grafo,"</td>");
            strcat(grafo,"</tr>\n");

            strcat(grafo,"<tr>");
            strcat(grafo,"<td>Part_type_");
            strcat(grafo,cambio);
            strcat(grafo,"</td>");
            strcat(grafo,"<td>");
            copia[0]=disco.mbr_partition[i].part_type;
            strcat(grafo,copia);
            strcat(grafo,"</td>");
            strcat(grafo,"</tr>\n");

            strcat(grafo,"<tr>");
            strcat(grafo,"<td>Part_fit_");
            strcat(grafo,cambio);
            strcat(grafo,"</td>");
            strcat(grafo,"<td>");
            copia[0]=disco.mbr_partition[i].part_fit;
            strcat(grafo,copia);
            strcat(grafo,"</td>");
            strcat(grafo,"</tr>\n");

            strcat(grafo,"<tr>");
            strcat(grafo,"<td>Part_start_");
            strcat(grafo,cambio);
            strcat(grafo,"</td>");
            strcat(grafo,"<td>");
            sprintf(cambio2,"%d",disco.mbr_partition[i].part_start);
            strcat(grafo,cambio2);
            strcat(grafo,"</td>");
            strcat(grafo,"</tr>\n");

            strcat(grafo,"<tr>");
            strcat(grafo,"<td>Part_Size_");
            strcat(grafo,cambio);
            strcat(grafo,"</td>");
            strcat(grafo,"<td>");
            sprintf(cambio2,"%d",disco.mbr_partition[i].part_size);
            strcat(grafo,cambio2);
            strcat(grafo,"</td>");
            strcat(grafo,"</tr>\n");

            strcat(grafo,"<tr>");
            strcat(grafo,"<td>Part_Name_");
            strcat(grafo,cambio);
            strcat(grafo,"</td>");
            strcat(grafo,"<td>");
            strcat(grafo,disco.mbr_partition[i].part_name);
            strcat(grafo,"</td>");
            strcat(grafo,"</tr>\n");
        }
    }
    strcat(grafo,"</table>>];\n");
    if(extendida==-1){

    }else{
        strcat(grafo,grafo2);
    }
    strcat(grafo,"}");
    fwrite(&grafo,strlen(grafo),1,file);
    fclose(file);
    char comando[50]="dot -Tpng -O ";
    strcat(comando,path2);
    system(comando);
}

void reporteDISK(char path[], char path2[]){
    FILE *destinoD = fopen(path2, "w");
    FILE *disk1 = fopen(path, "rb+");

    MBR mbr;
    fread(&mbr,sizeof(MBR),1,disk1);


    fprintf(destinoD, "digraph G { \n ");
    fprintf(destinoD, "node_A [shape=record    label=\"");
    fprintf(destinoD, "{\\n\\n\\n MBR \\n\\n\\n}",sizeof(MBR),mbr.mbr_fecha_creacion);

    float suma_Porcentaje = 0;
    float porcentaje_restante = 0;

    MBR disco;
    rewind(disk1);
    fread(&disco,sizeof(MBR),1,disk1);
    int i;
    for(i=0;i<4;++i){
        int j = 0;

        if (disco.mbr_partition[i].part_status==PARTITION_STATUS_ACTIVE){
            //printf("FDSDFSDFSDFSD");
            char * tipo;
            if(disco.mbr_partition[i].part_type == 'p'){
                tipo = "Primaria";
            }else{
                tipo = "Extendida";
            }
            float porcentaje= 0;
            porcentaje = (disco.mbr_partition[i].part_size*100)/mbr.mbr_tamanio;
            suma_Porcentaje = suma_Porcentaje + porcentaje;

            //printf("Porcentaje -> %f\n", porcentaje);

            if(tipo == "Primaria"){
                fprintf(destinoD, "|{%s \\n %d %% del Disco}", tipo, (int)porcentaje);
            }else{

                fprintf(destinoD, "|{%s |{", tipo);
                int extendida = particionExtendida(path);
                int inicioLogica;
                int tamanioExte;
                int sumaLogica;
                int libre;
                MBR disco2;
                FILE * file2;
                file2 = fopen(path,"rb");

                rewind(file2);
                fread(&disco2,sizeof(MBR),1,file2);
                inicioLogica=disco2.mbr_partition[extendida].part_start;
                tamanioExte = (disco2.mbr_partition[extendida].part_size*100)/mbr.mbr_tamanio;
                printf("TamanioExtendida--->%d\n",tamanioExte);
                EBR_P logica;
                int i=2;
                do{
                    fseek(file2,inicioLogica,SEEK_SET);
                    fread(&logica,sizeof(EBR_P),1,file2);
                    if(logica.part_status==PARTITION_STATUS_ACTIVE){
                        porcentaje = (logica.part_size*100)/mbr.mbr_tamanio;

                        fprintf(destinoD, "|EBR |%d %% del Disco", (int)porcentaje);
                        sumaLogica = sumaLogica + (int)porcentaje;
                        printf("SumaLogica--->%d\n",sumaLogica);
                        i++;
                    }else{
                        fprintf(destinoD, "EBR |%d %% del Disco}}", (int)porcentaje);
                    }

                    inicioLogica=logica.part_next;
                }while(logica.part_next!=-1);
                libre = tamanioExte - sumaLogica;
                fprintf(destinoD,"|Libre \\n%d%% del Disco}}",libre);
                sumaLogica = 0;
                libre = 0;
                tamanioExte = 0;
            }


        }else{
            j++;
        }


    }
    porcentaje_restante = 100 - suma_Porcentaje;

    fprintf(destinoD, " | {Libre \\n  %d %% del Disco }", (int)porcentaje_restante);
    fprintf(destinoD, "\"];\n} ");
    fclose(destinoD);
    char comandoD[1000];
    memset(&comandoD,'\0',sizeof(comandoD));
    sprintf(comandoD, "dot -Tpng \"%s\" -o \"%s.png\"", path2, path2);
    system(comandoD);
}
