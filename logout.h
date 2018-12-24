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

int processLOGOUT(char str[], int cont){
    if(cont == -1){
        printf(GRN "LOGOUT\n" RESET);
        if(bandera_log==1){
            strcpy(usuario,"");
            strcpy(contrasenia,"");
            strcpy(id_activo,"");
            strcpy(id_activo3,"");
            strcpy(path_activa,"");
            strcpy(name_activo,"");
            id_activo2='\0';
            bandera_log = 0;
            printf("\nSesion cerrada");
        }else{
            printf("\nERROR: No se ha iniciado ninguna sesion");
        }
        return 0;
    }


    return 0;
}
