#include "config_reader.h"

void extract_config(const char* fileconfig, Config* conf) {
    extract_peta(fileconfig, &(*conf).conf_peta);
    extract_banyak_bangunan(fileconfig, &(*conf).conf_banyak_bangunan);
    extract_listBangunan(fileconfig, (*conf).conf_banyak_bangunan,&(*conf).conf_list_bangunan);
    extract_relasi(fileconfig, (*conf).conf_banyak_bangunan, &(*conf).conf_relasi);
    CC = ' ';
}

void extract_peta(const char* fileconfig, Peta *peta){
    START_file(fileconfig, 1);
    int r = Token.Bil;
    ADV_file();
    int c = Token.Bil;
    MakePeta(peta, r, c);
}

void extract_banyak_bangunan(const char* fileconfig, int *banyak_bangunan) {
    START_file(fileconfig, 2);
    *banyak_bangunan = Token.Bil;
}

void extract_listBangunan(const char* fileconfig, int banyak_bangunan, TabBangunan* list_bangunan) {
    CreateEmptyTabBangunan(list_bangunan, banyak_bangunan);
    Bangunan B; 
    for(int i = 1; i < banyak_bangunan+1; i++ ) {
        START_file(fileconfig, 2 + i);

        CreateBangunanEmpty(&B);
        B.id = i;
        if(i <= 2) B.owner = i;
        else B.owner = 0;

        B.type = Token.Val;
        ADV_file();
        B.posisi.r = Token.Bil;
        ADV_file();
        B.posisi.c = Token.Bil;

        B.level = 0;
        levelup(&B);
        AddBangunan(B, list_bangunan);
    }
}

void extract_relasi(const char* fileconfig, int banyak_bangunan, Graph* relasi) {
    int i, j;
    *relasi = CreateGraph(banyak_bangunan, banyak_bangunan);
    for ( i = 1; i <= banyak_bangunan; i++ ) {
        START_file(fileconfig, 2 + banyak_bangunan + i);
        for ( j = 1; j <= banyak_bangunan; j++ ) {
            if ( Token.Bil == 1) SetBangunanTerhubung(*relasi, i, j);
            ADV_file();
        }
    }
}