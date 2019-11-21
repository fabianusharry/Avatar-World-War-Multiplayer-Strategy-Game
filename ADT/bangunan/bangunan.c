#include "bangunan.h"

boolean ShieldActive, CritHitActive;

void CreateBangunanEmpty(Bangunan *B){
    (*B).id = -1;
    (*B).jumlah_pasukan = 0;
    (*B).level = 0;
    (*B).nilai_tambah_pasukan = 0;
    (*B).maksimum_tambah_pasukan = 0;
    (*B).owner = -1;
    (*B).pertahanan = false;
    (*B).sudahserang = false;
    (*B).posisi.r = 0;
    (*B).posisi.c = 0;
    (*B).type = ' ';
}

void CopyBangunan(Bangunan Bin, Bangunan *B){
    (*B).id = Bin.id;
    (*B).jumlah_pasukan = Bin.jumlah_pasukan;
    (*B).level = Bin.level;
    (*B).nilai_tambah_pasukan = Bin.nilai_tambah_pasukan;
    (*B).maksimum_tambah_pasukan = Bin.maksimum_tambah_pasukan;
    (*B).owner = Bin.owner;
    (*B).pertahanan = Bin.pertahanan;
    (*B).sudahserang = Bin.sudahserang;
    (*B).posisi.r = Bin.posisi.r;
    (*B).posisi.c = Bin.posisi.c;
    (*B).type = Bin.type;
}

boolean IsBangunanEmpty(Bangunan B){
    return B.id == -1;
}


boolean IsLvlUpValid(Bangunan B)
/* Mengecek apakah bangunan valid untuk di level up, syarat level up adalah jumlah pasukan >= M/2 */
{
    if(Type(B) == 'C' ){
        return(Pasukan(B) >= (MatElmt(AttCastle,Level(B),2)/2));
    }
    else if(Type(B) == 'T'){
        return(Pasukan(B) >= (MatElmt(AttTower,Level(B),2)/2));
    }
    else if(Type(B) == 'F'){
        return(Pasukan(B) >= (MatElmt(AttFort,Level(B),2)/2));
    }
    else if(Type(B) == 'V'){
        return(Pasukan(B) >= (MatElmt(AttVillage,Level(B),2)/2));
    }
}

void levelup(Bangunan *B){
/* Menaikkan level dari Bangunan Pemain */
    if(Level(*B) < 4){
        Pasukan(*B) -= (MatElmt(AttCastle,Level(*B),2)/2);
        Level(*B) += 1;
        // "M" : 1, "A" : 2, "P" : 3
        if(Type(*B) == 'C'){
            RateTambah(*B) = MatElmt(AttCastle, Level(*B), 1);
            MaxPasukan(*B) = MatElmt(AttCastle, Level(*B), 2);
            Pertahanan(*B) = MatElmt(AttCastle, Level(*B), 3);
            if(Level(*B) == 1) Pasukan(*B) = MatElmt(AttCastle, Level(*B), 4);
        }
        else if(Type(*B) == 'T'){
            RateTambah(*B) = MatElmt(AttTower, Level(*B), 1);
            MaxPasukan(*B) = MatElmt(AttTower, Level(*B), 2);
            Pertahanan(*B) = MatElmt(AttTower, Level(*B), 3);
            if(Level(*B) == 1) Pasukan(*B) = MatElmt(AttTower, Level(*B), 4);
        }
        else if(Type(*B) == 'F'){
            RateTambah(*B) = MatElmt(AttFort, Level(*B), 1);
            MaxPasukan(*B) = MatElmt(AttFort, Level(*B), 2);
            Pertahanan(*B) = MatElmt(AttFort, Level(*B), 3);
            if(Level(*B) == 1) Pasukan(*B) = MatElmt(AttFort, Level(*B), 4);
        }
        else if(Type(*B) == 'V'){
            RateTambah(*B) = MatElmt(AttVillage, Level(*B), 1);
            MaxPasukan(*B) = MatElmt(AttVillage, Level(*B), 2);
            Pertahanan(*B) = MatElmt(AttVillage, Level(*B), 3);
            if(Level(*B) == 1) Pasukan(*B) = MatElmt(AttVillage, Level(*B), 4);
        }
    }
    else{
        printf("Level Bangunan sudah Maksimum, tidak dapat melakukan level up lagi\n");
    }
}

boolean attack(Bangunan *BAtt, Bangunan *BDef, int jumlah_penyerang){
    // return apakah berhasil attack/tidak (walaupun gagal merebut bangunan ttp dikatakan berhasil menyerang)
    if((*BAtt).sudahserang){
        printf("Bangunan sudah digunakan untuk menyerang pada turn ini! Silakan gunakan bangunan lain!");
        return false;
    }
    (*BAtt).sudahserang = true;
    Pasukan(*BAtt) -= jumlah_penyerang;
    if(CritHitActive){
        printf("Kamu punya skill critical hit aktif! Damage pasukanmu menjadi 2x lipat!");
        if(2*jumlah_penyerang >= Pasukan(*BDef)){
            BangunanOwner(*BDef) = BangunanOwner(*BAtt);
            Pasukan(*BDef) = jumlah_penyerang-((Pasukan(*BDef)-1)/2)+1;
        } else {
            Pasukan(*BDef) -= 2*jumlah_penyerang;
            printf("Namun bangunan tetap gagal direbut."); ENDL;
            printf("Pasukan musuh menjadi %d", Pasukan(*BDef)); ENDL;
        }
    } else {
        if(ShieldActive || Pertahanan(*BDef)){
            printf("Bangunan musuh memiliki pertahanan! Damage pasukanmu hanya efektif 75%% saja!"); ENDL;
            if((3*jumlah_penyerang/4) >= Pasukan(*BDef)){
                BangunanOwner(*BDef) = BangunanOwner(*BAtt);
                Pasukan(*BDef) = jumlah_penyerang - ((4*Pasukan(*BDef)-1)/3)+1;
            } else {
                Pasukan(*BDef) -= 3*jumlah_penyerang/4;
            }
        } else {
            /* Perubahan Jumlah Pasukan saat melancarkan Penyerangan */
            Pasukan(*BDef) -= jumlah_penyerang;
            
            if(Pasukan(*BDef) >= 0){
                printf("Bangunan tidak berhasil diambil.\n");
            } else {
                BangunanOwner(*BDef) = BangunanOwner(*BAtt);
                Pasukan(*BDef) = -Pasukan(*BDef);
                printf("Bangunan berhasil diambil, ");
                if(Pasukan(*BDef) < 0){
                    printf("pasukan yang tersisa sebanyak %d pasukan di dalam bangunan.\n", Pasukan(*BDef));
                } else {
                    printf("tidak ada pasukan yang tersisa untuk menjaga bangunan.\n");
                }
            }
        }
    }
    return true;
}
/* Menghitung perubahan jumlah pasukan saat terjadi penyerangan oleh BAtt kepada BDef */

void move(Bangunan *BAwal, Bangunan *BAkhir, int jumlah_pasukan_pindah){
    if(Pasukan(*BAkhir) == MaxPasukan(*BAkhir)){
         printf("Bangunan yang dituju penuh, tidak dapat MatElmtindahkan pasukan\n");
    }
    else{ /*Bangunan Akhir tidak penuh, perpindahan dapat dilakukan */

        if(Pasukan(*BAwal) < jumlah_pasukan_pindah){ /* Jumlah pasukan di bangunan awal kurang dari jumlah pasukan yang akan dipindahkan */

            if((Pasukan(*BAwal) + Pasukan(*BAkhir)) > MaxPasukan(*BAkhir)){ /*Perpindahan melebihi kapasitas bangunan akhir */
                printf("Pasukan di bangunan awal kurang, MatElmtindahkan %d pasukan karena bangunan akhir sudah penuh\n", MaxPasukan(*BAkhir) - Pasukan(*BAkhir));
                Pasukan(*BAwal) -= MaxPasukan(*BAkhir) - Pasukan(*BAkhir);
                Pasukan(*BAkhir) += MaxPasukan(*BAkhir) - Pasukan(*BAkhir);
            }

            else{ /* Perpindahan masih dapat ditampung kapasitas bangunan akhir */
                printf("Pasukan di bangunan awal kurang, MatElmtindahkan %d pasukan\n", Pasukan(*BAwal));
                Pasukan(*BAwal) -= Pasukan(*BAwal);
                Pasukan(*BAkhir) += Pasukan(*BAwal);
            }
        }

        else{ /*Jumlah pasukan di bangunan awal lebih dari jumlah pasukan yang akan dipindahkan */

            if((jumlah_pasukan_pindah + Pasukan(*BAkhir)) > MaxPasukan(*BAkhir)){ /*Perpindahan melebihi kapasitas bangunan akhir */
                printf("MatElmtindahkan %d pasukan karena bangunan akhir sudah penuh\n", MaxPasukan(*BAkhir) - Pasukan(*BAkhir));
                Pasukan(*BAwal) -= MaxPasukan(*BAkhir) - Pasukan(*BAkhir);
                Pasukan(*BAkhir) += MaxPasukan(*BAkhir) - Pasukan(*BAkhir);
            }
            else{/* Perpindahan terjadi secara normal tanpa ada bangunan yang habis pasukannya atau penuh */
                Pasukan(*BAwal) -= jumlah_pasukan_pindah;
                Pasukan(*BAkhir) += jumlah_pasukan_pindah;
            }
        }
    }
}
/* Menghitung perubahan jumlah pasukan saat terjadi perpindahan pasukan (Command : MOVE) */

void add_pasukan(Bangunan *B){
    if(Pasukan(*B) < MaxPasukan(*B)){
        Pasukan(*B) += RateTambah(*B);
    }
}
/* Menghitung perubahan jumlah pasukan saat awal turn */

void TulisBangunan(Bangunan B){
    printf("<%d> %c at (%d,%d)\n", B.id, B.type, B.posisi.r, B.posisi.c);
    printf("Milik player "); println(B.owner);
    printf("Level: "); println(B.level);
    printf("Jumlah Pasukan: "); println(B.jumlah_pasukan);
    printf("Shield active? ");
    if(B.pertahanan) printf("yes");
    else printf("no");
    ENDL;
    printf("Sudah serang? ");
    if(B.sudahserang) printf("yes");
    else printf("no");
    ENDL;
}


/* Mengeprint Type sebuah bangunan */
void printTypeBangunan(Bangunan B){
    if (Type(B) == 'C') printf("Castle");
    else if (Type(B) == 'T') printf("Tower");
    else if (Type(B) == 'F') printf("Fort");
    else if (Type(B) == 'V') printf("Village");
}