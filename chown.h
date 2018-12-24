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

typedef struct chown{
    char path[50];
    char usr[15];
    char R[15];
} chown_init;


int processCHOWN(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS \n" RESET);
        return 0;
    }
    chown_init rm;
    memset(&rm.path,'\0',sizeof(rm.path));
    memset(&rm.usr,'\0',sizeof(rm.usr));
    memset(&rm.R,'\0',sizeof(rm.R));
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
            else if(strcasecmp("R", buffer) == 0){
                strcat(rm.R, "R");

            }else if(strcasecmp("USR", name) == 0){
                strcat(rm.usr, buffer);

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
        // Posible optimizacion
        if(strcasecmp("PATH", name) == 0){
            strcat(rm.path, buffer);
        }
        else if(strcasecmp("R", buffer) == 0){
            strcat(rm.R, "R");

        }else if(strcasecmp("USR", name) == 0){
            strcat(rm.usr, buffer);

        }else{
            printf(RED "ERROR: COMANDO INEXISTENTE2\n" RESET);

            printf("ch:%c",ch);
            printf("buffer:%s",buffer);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    printf("\nPATH->%s|USR->%s|R->%s|\n",rm.path,rm.usr,rm.R);


    return 0;
}


