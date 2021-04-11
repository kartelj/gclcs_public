#define MAXS 2001
#define MAXP 15
#define MAXALPHABET 26
#define IINF 1000000000
#define DEC(c) (problem->inputAlphabet[c-'a'])


#define finp vns->inp
#define fout vns->out

typedef struct
{
    int fv;             /* vrednost */
    int iter,iterMax, iterWoChange, maxTime;   /* br bj iteracije, maks. iteracija, maks. iteracija bez promene, maksimalno sekundi */
    int iterp;          /* iteracija u kojoj je dobijena poslednja promena */
    int nls,nlsp;       /* koliko je bilo local search-a */
    unsigned int seed;  /* random seed */
    int k,kmin, kmax, kmaxinit;   /* velicina okoline i min i max vel okoline */
    double prob;        /* verovatnoca da se ide u istu vrednost resenja */
    FILE *inp,*sol,*out;
    char imeul[MAXS],imeout[MAXS],imelog[MAXS], imeInit[MAXS], probl[MAXS];
    int initialFileSolution;
    clock_t runstart,runend,runp;
    time_t tstart,tend,tp; 
    char sts[MAXS],ste[MAXS],stp[MAXS];
    double rt,rtp;  /* vreme izvrsavanja u sekundama */
    char cmdl[MAXS];
    int *v;  /* za izbor k elemenata koji se menjaju */
	int *y; /* pomocni niz za izbor ovih k elemenata */
    int iraceMode;
} vnsStruct;


typedef struct
{ 
  double fv; /* fitnes funkcija*/
  int nonCorrect; /* nezadovoljenih */
  int card; /* kardinalnost resenja */
  char *x;   /* vrednost karaktera na i-toj poziciji */
  int **leftMostYpath; /* indeksi nadniski i podniski unutar aktuelnog resenja prema najlevljem uparivanju*/
  int **rightMostYpath; /* isto to samo za desno uparivanje */
  int *sc;/* prethodna velicina poklapanja, ili -1 ako je neprimenjivo parcijalno racunanje, tj. jer levo i desno uparivanje nije isto */
} solStruct;

typedef struct {
    double fv;
    int nonCorrect;
    int card;
} solCachedValueStruct;

typedef struct
{
    int ns,sig, np, n;  /* Broj nadstringova, velicina azbuke, broj sablona, duzina najduzeg sablona */
    int *nns;   /* velicina i-tog nadstringa */
    char** s;    /* i-ti nadstring */
    int* nnp;    /* velicina i-tog sablona */
    char** p;   /* i-ti sablon */
    char inputAlphabet[MAXALPHABET];
    int inputAlphabetCount;
    char fromChar[MAXALPHABET];
    char toChar[MAXALPHABET];
	solStruct x,xt; /* aktivno resenje, resenje posle shaking-a */
} problemStruct;


void init();
void allocation();
void copy(solStruct *dest, solStruct* src);
void copyt();
void input();
void print(FILE *);
int shaking();
void LS1(solStruct* sol, int logImprovement);
int validValue(char c);
int variation(int, int, int *, int *, int*);

