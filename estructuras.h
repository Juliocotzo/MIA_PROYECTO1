#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct mbr_partition{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} MBR_P;

typedef struct MBR{
    int mbr_tamanio;
    char mbr_fecha_creacion[16];
    int mbr_disk_signature;
    MBR_P mbr_partition[4];
} MBR;

typedef struct EBR{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
} EBR_P;

typedef struct nodo{
    char nombre[40];
    char path[100];
    char id[10];
    char letra[10];
    char numero[10];
    int numdiscos;
    struct nodo *siguiente;
    struct nodo *anterior;
}nodo;

typedef struct superBloque{
    char inicio;
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;  //61267
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}superBloque;

typedef struct journaling{
    int journal_tipo_operacion;
    int journal_tipo;
    char journal_nombre[50];
    char journal_contenido[100];
    time_t journal_fecha;
    int journal_propietario;
    int journal_permisos;
} journaling;

typedef  struct inodo{
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
    int i_perm;
}inodo;

typedef  struct content{
    char b_name[12];
    int b_inodo;
}content;

typedef  struct bloqueCarpetas{
    content b_content[4];
}bloqueCarpetas;

typedef struct bloqueArchivos{
    char b_content[64];
}bloqueArchivos;

typedef struct bloqueApuntadores{
      int b_pointers[16];
}bloqueApuntadores;




