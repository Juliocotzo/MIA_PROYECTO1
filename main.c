#include <stdio.h>
#include <stdlib.h>
#include "analizador.h"

int main(){

    char str[250];
    memset(&str,'\0',sizeof(str));
    printf(GRN "[=========================== MIA-2018 PRACTICA 1 ===========================]\n" RESET);

    do{
        printf(GRN "MIA-201504381" RESET);
        printf(BLU " ~ $ " RESET);

        if (scanf ("%[^\n]%*c", str) == 0){
            char ch;
            while( ( ch = getchar() ) != '\n' && ch != EOF );
            continue;
        }
        if (strcmp(str, "EXIT") == 0 || strcmp(str, "exit")==0){
            break;
        }
        if(strcmp(str, "EXEC")==0){
            processCommand("exec -path->/home/julio/Descargas/Entrada_1.sh");
        }
        if(strcmp(str, "EXEC1")==0){

            processCommand("exec -path->\"/home/julio/Documentos/Qt Creator/PROYECTO1/Entrada.sh\"");
        }


        //processCommand(str);
        memset(&str,'\0',sizeof(str));

    } while (true);

    printf(GRN "[======================================================================== Exit() \n" RESET);
    return 0;
}
