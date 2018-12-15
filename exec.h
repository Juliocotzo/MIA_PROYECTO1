#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>

#define WORD 1
char buf[WORD];
char str2[300*300];

#define RED   "\x1B[1;31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[1;36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


typedef struct exec{
    char path[80];
} exec_init;

int processEXEC(char str[], int cont){
    if(cont == -1){
        printf(RED"ERROR: COMANDO SIN PARAMETROS\n"RESET);
        return 0;
    }
    exec_init rm;
    memset(&rm.path,'\0',sizeof(rm.path));
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
        }
        else if((ch == ' ' || ch == '\n') && (j != 0) && read){
            buffer[j] = '\0';
            if(strcasecmp("PATH", name) == 0){
                strcat(rm.path, buffer);
            }
            else{
                // Errores
                printf(RED"ERROR: EN EL COMANDO\n"RESET);
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    }
    if((j != 0) && read){
        buffer[j] = '\0';
        if(strcasecmp("PATH", name) == 0){
            strcat(rm.path, buffer);
        }else{

            printf(RED"ERROR: EN EL COMANDO\n"RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }
    char* comando = (char*)malloc(500);

    memset(&comando[0], 0, sizeof(comando));
    strcat(comando, "rm -i \"");
    strcat(comando, rm.path);
    strcat(comando, "\"");
    //printf("\n%s\n",rm.path);
    free(comando);
    //printf("\n");


    FILE *file;
    size_t nread;

    //*SE LIMPIA LA ENTRADA
    strcpy(str2, "");


    file = fopen(rm.path, "r");
    if (file) {
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0){

            strcat(str2, buf);
        }

        if (ferror(file)) {
            printf(RED"ERROR: AL LEER EL ARCHIVO\n"RESET);
            return false;
        }
        fclose(file);
    }
    else{

        printf(RED"ERROR: AL LEER EL ARCHIVO\n"RESET);
        return false;
    }


    //printf( CYN "\nDireccion: %s \n" RESET, rm.path);


    return 0;
}
