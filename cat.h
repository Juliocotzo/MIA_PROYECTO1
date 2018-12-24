#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>

#define RED   "\x1B[1;31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[1;36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct cat{
    char file1[80];
    char file2[80];
    char file3[80];
    char file4[80];
    char file5[80];
    char file6[80];
    char file7[80];
    char file8[80];
    char file9[80];
} cat_init;

int processCAT(char str[], int cont){
    if(cont == -1){
        printf(RED "ERROR: COMANDO SIN PARAMETROS\n" RESET);
        return 0;
    }
    cat_init rm;
    memset(&rm.file1,'\0',sizeof(rm.file1));
    memset(&rm.file2,'\0',sizeof(rm.file2));
    memset(&rm.file3,'\0',sizeof(rm.file3));
    memset(&rm.file4,'\0',sizeof(rm.file4));
    memset(&rm.file5,'\0',sizeof(rm.file5));
    memset(&rm.file6,'\0',sizeof(rm.file6));
    memset(&rm.file7,'\0',sizeof(rm.file7));
    memset(&rm.file8,'\0',sizeof(rm.file8));
    memset(&rm.file9,'\0',sizeof(rm.file9));
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
            if(strcasecmp("FILE1", name) == 0){
                strcat(rm.file1, buffer);
            }else if (strcasecmp("FILE2", name) == 0) {
                strcat(rm.file2, buffer);
            }else if (strcasecmp("FILE3", name) == 0) {
                strcat(rm.file3, buffer);
            }else if (strcasecmp("FILE4", name) == 0) {
                strcat(rm.file4, buffer);
            }else if (strcasecmp("FILE5", name) == 0) {
                strcat(rm.file5, buffer);
            }else if (strcasecmp("FILE6", name) == 0) {
                strcat(rm.file6, buffer);
            }else if (strcasecmp("FILE7", name) == 0) {
                strcat(rm.file7, buffer);
            }else if (strcasecmp("FILE8", name) == 0) {
                strcat(rm.file8, buffer);
            }else if (strcasecmp("FILE9", name) == 0) {
                strcat(rm.file9, buffer);
            }else{
                // Errores
                printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
                return 0;
            }
            j = 0;
            memset(&buffer,'\0',sizeof(buffer));
        }
    }
    if((j != 0) && read){
        buffer[j] = '\0';
        if(strcasecmp("FILE1", name) == 0){
            strcat(rm.file1, buffer);
        }else if (strcasecmp("FILE2", name) == 0) {
            strcat(rm.file2, buffer);
        }else if (strcasecmp("FILE3", name) == 0) {
            strcat(rm.file3, buffer);
        }else if (strcasecmp("FILE4", name) == 0) {
            strcat(rm.file4, buffer);
        }else if (strcasecmp("FILE5", name) == 0) {
            strcat(rm.file5, buffer);
        }else if (strcasecmp("FILE6", name) == 0) {
            strcat(rm.file6, buffer);
        }else if (strcasecmp("FILE7", name) == 0) {
            strcat(rm.file7, buffer);
        }else if (strcasecmp("FILE8", name) == 0) {
            strcat(rm.file8, buffer);
        }else if (strcasecmp("FILE9", name) == 0) {
            strcat(rm.file9, buffer);
        }else{
            // Errores
            printf(RED "ERROR: COMANDO INEXISTENTE\n" RESET);
            return 0;
        }
        j = 0;
        memset(&buffer,'\0',sizeof(buffer));
    }

    printf("\nFILE1->%s\n",rm.file1);
    printf("FILE2->%s\n",rm.file2);
    printf("FILE3->%s\n",rm.file3);
    printf("FILE4->%s\n",rm.file4);
    printf("FILE5->%s\n",rm.file5);
    printf("FILE6->%s\n",rm.file6);
    printf("FILE7->%s\n",rm.file7);
    printf("FILE8->%s\n",rm.file8);
    printf("FILE9->%s\n",rm.file9);

    return 0;
}
