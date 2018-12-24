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
#include "login.h"
#include "logout.h"
#include "mkgrp.h"
#include "rmgrp.h"
#include "mkusr.h"
#include "rmusr.h"
#include "chmod.h"
#include "mkfile.h"
#include "cat.h"
#include "rem.h"
#include "edit.h"
#include "ren.h"
#include "mkdir.h"
#include "cp.h"
#include "mv.h"
#include "find.h"
#include "chown.h"
#include "chgrp.h"
#include "recovery.h"
#include "loss.h"


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
    }else if (strcasecmp("LOGIN",buffer)==0) {
        return 9;
    }else if (strcasecmp("LOGOUT",buffer)==0) {
        return 10;
    }else if (strcasecmp("MKGRP",buffer)==0) {
        return 11;
    }else if (strcasecmp("RMGRP",buffer)==0) {
        return 12;
    }else if (strcasecmp("MKUSR",buffer)==0) {
        return 13;
    }else if (strcasecmp("RMUSR",buffer)==0) {
        return 14;
    }else if (strcasecmp("CHMOD",buffer)==0) {
        return 15;
    }else if (strcasecmp("MKFILE",buffer)==0) {
        return 16;
    }else if (strcasecmp("CAT",buffer)==0) {
        return 17;
    }else if (strcasecmp("REM",buffer)==0) {
        return 18;
    }else if (strcasecmp("EDIT",buffer)==0) {
        return 19;
    }else if (strcasecmp("REN",buffer)==0) {
        return 20;
    }else if (strcasecmp("MKDIR",buffer)==0) {
        return 21;
    }else if (strcasecmp("CP",buffer)==0) {
        return 22;
    }else if (strcasecmp("MV",buffer)==0) {
        return 23;
    }else if (strcasecmp("FIND",buffer)==0) {
        return 24;
    }else if (strcasecmp("CHOWN",buffer)==0) {
        return 25;
    }else if (strcasecmp("CHGRP",buffer)==0) {
        return 26;
    }else if (strcasecmp("RECOVERY",buffer)==0) {
        return 27;
    }else if (strcasecmp("LOSS",buffer)==0) {
        return 28;
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
    case 9:
        processLOGIN(str, start);
        break;
    case 10:
        processLOGOUT(str, start);
        break;
    case 11:
        processMKGRP(str, start);
        break;
    case 12:
        processRMGRP(str, start);
        break;
    case 13:
        processMKUSR(str, start);
        break;
    case 14:
        processRMUSR(str, start);
        break;
    case 15:
        processCHMOD(str, start);
        break;
    case 16:
        processMKFILE(str, start);
        break;
    case 17:
        processCAT(str, start);
        break;
    case 18:
        processREM(str, start);
        break;
    case 19:
        processEDIT(str, start);
        break;
    case 20:
        processREN(str, start);
        break;
    case 21:
        processMKDIR(str, start);
        break;
    case 22:
        processCP(str, start);
        break;
    case 23:
        processMV(str, start);
        break;
    case 24:
        processFIND(str, start);
        break;
    case 25:
        processCHOWN(str, start);
        break;
    case 26:
        processCHGRP(str, start);
        break;
    case 27:
        processRECOVERY(str, start);
        break;
    case 28:
        processLOSS(str, start);
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
