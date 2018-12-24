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

typedef struct chgrp{
    char usr[20];
    char grp[20];
} chgrp_init;

int processCHGRP(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS \n" RESET);
        return 0;
    }
    chgrp_init rm;
    memset(&rm.usr,'\0',sizeof(rm.usr));
    memset(&rm.grp,'\0',sizeof(rm.grp));

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
            }else if(strcasecmp("GRP", name) == 0){
                strcat(rm.grp, buffer);
            }else{
                printf(RED "ERsaROR: COMANDO INEXISTENTE\n" RESET);
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
        }else if(strcasecmp("GRP", name) == 0){
            strcat(rm.grp, buffer);
        }else{
            printf(RED "ERsaROR: COMANDO INEXISTENTE\n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    printf("\nUSR->%s|GRP->%s|\n",rm.usr,rm.grp);


    return 0;
}
