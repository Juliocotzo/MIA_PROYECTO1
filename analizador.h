#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include "mount.h"
#include "unmount.h"
#include "rep.h"
#include "exec.h"
#include "mkfs.h"


#define RED   "\x1B[1;31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[1;36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct command{
    int id;
    char name[15];
} cmd;

void exec();


int isKeyword(char buffer[]){

    if(strcasecmp("MKDISK",buffer)==0){
        return 1;
    }else if (strcasecmp("RMDISK",buffer)==0) {
        return 2;
    }else if (strcasecmp("FDISK",buffer)==0) {
        return 3;
    }else if (strcasecmp("MOUNT",buffer)==0) {
        return 4;
    }else if (strcasecmp("UNMOUNT",buffer)==0) {
        return 5;
    }else if (strcasecmp("REP",buffer)==0) {
        return 6;
    }else if (strcasecmp("EXEC",buffer)==0) {
        return 7;
    }else if (strcasecmp("MKFS",buffer)==0) {
        return 8;
    }
    return 0;
}


void commandAnalyzer(int state, char str[], int start){
    switch(state){
    case 1:
        processMKDISK(str,start);
        break;
    case 2:
        processRMDISK(str,start);
        break;
    case 3:
        processFDISK(str,start);
        break;
    case 4:
        processMOUNT(str,start);
        break;
    case 5:
        processUNMOUNT(str,start);
        break;
    case 6:
        processREP(str, start);
        break;
    case 7:
        processEXEC(str,start);
        exec();
        break;
    case 8:
        processMKFS(str, start);
        break;
    }
}

int processCommand(char str[]){
    char ch, buffer[150];
    memset(&buffer,'\0',sizeof(buffer));
    int i=0,j=0;
    while((ch = str[i++]) != '\0'){
        if(isalnum(ch)){
            buffer[j++] = ch;
        }else if((ch == ' ' || ch == '\n') && (j != 0)){
            buffer[j] = '\0';
            j = 0;
            int state = isKeyword(buffer);
            if(state != 0){
                commandAnalyzer(state,str, i);
            }else{
                printf(RED "%s ERROR: NO ES UN COMANDO VALIDO!\n" RESET, buffer);
            }
            return 0;
        }
    }
    int state = isKeyword(buffer);
    if(state != 0){
        commandAnalyzer(state, str, -1);
    }else{
        printf(RED "%s ERROR: NO ES UN COMANDO VALIDO!\n" RESET, buffer);
    }
    return 0;
}

void exec(){
    char *str = strdup(str2);
    char *tofree = str;
    char *linea;

    while ((linea = strsep(&str, "\n"))){
        if (!strcmp(linea, "")==0){

            int len = strlen(linea);

            if (len > 0){
                if (linea[0] == '#'){
                    printf(YEL "%s \n" RESET, linea);
                }else{
                    //printf(CYN "%s \n" RESET, linea);

                    if (linea[0] != ' '){
                        processCommand(linea);
                    }

                }

            }

        }

    }

    free(tofree);
}
