#ifndef GLOBAL_H
#define GLOBAL_H

#define PetaBrsMin 1
#define PetaBrsMax 20
#define PetaKolMin 1
#define PetaKolMax 30
#define MaxSaveData 4

boolean IUActive;
boolean ExtraTurnActive;

typedef struct {
    int Mem[PetaBrsMax+1][PetaKolMax+1];
    int NPetaBrsEff; /* banyaknya/ukuran baris yg terdefinisi */
    int NPetaKolEff; /* banyaknya/ukuran PetaKolom yg terdefinisi */
} Peta;

typedef struct {
	int r;
	int c;
} Point;
typedef struct {
	Point posisi;
	int id;
	int jumlah_pasukan;
	int level;
	int nilai_tambah_pasukan;
	int maksimum_tambah_pasukan;
	int owner;
	boolean pertahanan;
	boolean sudahserang;
	boolean sudahpindah;
	char type;
} Bangunan;
typedef struct {
  Bangunan *TB; /* memori tempat penyimpan elemen (container) */
  int NeffTB;   /* >=0, banyaknya elemen efekTITBf */
  int MaxElTB; 
} TabBangunan;

typedef struct tNodeGraph* adrNode;
typedef struct tAdrSucc* adrSucc;
typedef struct tNodeGraph {
	int Id;
    int NPred;
    adrSucc Trail; // ke bawah
    adrNode Next; // ke kanan
} NodeGraph;
typedef struct tAdrSucc {
    adrNode Succ;
    adrSucc Next;
} SuccGraph;
typedef struct {
    adrNode First;
}Graph;

typedef struct tElmtlist *address;
typedef struct tElmtlist {
	int info;
	address next;
} ElmtList;
typedef struct {
	address First;
} ListBangunan;

typedef struct {
	int * T;   /* tabel penyimpan elemen */
	int HEAD;  /* alamat penghapusan */
	int TAIL;  /* alamat penambahan */
	int MaxEl;     /* Max elemen queue */
	int owner;
} Queue;

typedef struct {
	Queue Skill; /* Skill yang dimiliki player */
	ListBangunan list_bangunan;
	boolean ShieldActive;
	boolean CritHitActive;
	boolean AttUpActive;
} Player;
typedef struct {
    Player Players[2]; // Array of player, menyimpan data player1 dan player2
	TabBangunan ListBangunan; // Array of bangunan, menyimpan data bangunan
    unsigned int turn;  // Bernilai 1 atau 2, yaitu id player yang sedang memiliki turn
} GameCondition;
#define StackMaxEl 100
typedef struct {
  GameCondition T[StackMaxEl+1]; /* tabel penyimpan elemen */
  int TOP;  /* alamat TOP: elemen puncak */
} GameStack;

typedef struct {
    Peta map; // Menyimpan peta
    Graph Relasi;
    GameStack GameConditions; // Stackt of GameCondition, menyimpan kondisi dari setiap aksi dilakukan (dikosongkan setelah endturn)
	char* config_file;
} Game;
// GameCondition yang digunakan adalah yang berada di Top
// setiap melakukan aksi lakukan Push pada stackt
// saat melakukan UNDO lakukan Pop pada stackt 

typedef struct {
    GameCondition Gc;
    boolean not_empty;
    char* savename;
    char* config_file;
} Data;

typedef struct {
    Data data[MaxSaveData+1];
	boolean not_empty;
    int Max;
} SaveData;

#define TopStackt(S) (S).TOP
#define InfoTopStackt(S) (S).T[(S).TOP]

#define Player(n) InfoTopStackt(G.GameConditions).Players[(n)-1]
#define CurTurn() InfoTopStackt(G.GameConditions).turn
#define OtherTurn() ((InfoTopStackt(G.GameConditions).turn)%2)+1
#define CurPlayer() Player(CurTurn())
#define OtherPlayer() Player(OtherTurn())

extern Game G;

#endif