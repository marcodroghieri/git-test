#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include "gamelib.h"

static void genera_mappa(void);
//static void inserisci_zona(void);
//static void cancella_zona(void);
static void stampa_mappa(void);
//static void chiudi_mappa(void);
static char* nomi_stanze(Zona_segrete* , int);

struct Zona_segrete* pFirst;
struct Zona_segrete* pLast;

void imposta_gioco(){
    //Quanti giocaori partecipano alla partita?
    unsigned int numero_giocatori;
    while(1){
        puts("\nQuanti giocatori parteciperanno alla partita? (#1-4)");   //ERRORE 2fjhashg
        if(scanf("%u", &numero_giocatori) == 1 && (numero_giocatori >= 1 && numero_giocatori <= 4)){
            break;
        }
        else{
            puts("Numero inserito non valido, ritenta!");
            while(getchar() != '\n');   //in caso di input non valido da parte dell'utente pulisco il buffer di input per il tentativo successivo dell'utente
            continue;
        }
    }
    while(getchar() != '\n');
    //Inizializzazione giocatori:
    Giocatore* giocatori[4]={NULL,NULL,NULL,NULL};

    for(int i=0;i<numero_giocatori;i++){
        Giocatore* nuovo_giocatore = (Giocatore*) malloc(sizeof(Giocatore));    //Creazione array di struct in memoria dinamica
        printf("\nGiocatore %d come ti vuoi chiamare?\n",i+1);
        fgets(nuovo_giocatore->nome_giocatore,sizeof(nuovo_giocatore->nome_giocatore),stdin);
        giocatori[i] = nuovo_giocatore;     //memorizzo giocatore nell'array
    }

    time_t t;
    srand((unsigned) time(&t));
    int random_barbaro = rand()%2 + 1;
    int random_nano = rand()%2 + 2;
    int random_elfo = rand()%2 + 3;
    int random_mago = rand()%2 + 4;

    bool flag = true;
    int contatore=0;

    while(flag){
        for(int i=0;i<numero_giocatori;i++){
            int opzione;
            printf("\n %s scegli una classe tra le seguenti: \n", giocatori[i]->nome_giocatore);
            puts("1)BARBARO");
            puts("2)NANO");
            puts("3)ELFO");
            puts("4)MAGO");

                if(scanf("%d", &opzione) !=1 || opzione<1 || opzione>4){
                    puts("\nScelta invalida, ritenta!");
                    while(getchar() != '\n');    //in caso di fallimento della funzione scanf, il buffer di input viene ripulito
                    i=-1;   //in caso di input invalido (ossia non un numero tra 1 e 4) il ciclo for rinizia da capo e richiede a tutti giocatori di riselezionare una classe valida
                }
                else{
                    switch(opzione){
                        case 1:
                            giocatori[i]->classe=0;
                            giocatori[i]->dadi_attacco=3;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=8;
                            giocatori[i]->mente=random_barbaro;
                        case 2:
                            giocatori[i]->classe=1;
                            giocatori[i]->dadi_attacco=2;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=7;
                            giocatori[i]->mente=random_nano;
                        case 3:
                            giocatori[i]->classe=2;
                            giocatori[i]->dadi_attacco=2;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=6;
                            giocatori[i]->mente=random_elfo;
                        case 4:
                            giocatori[i]->classe=3;
                            giocatori[i]->dadi_attacco=1;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=4;
                            giocatori[i]->mente=random_mago;
                    }   
                contatore++;    //conteggio numero di input validi da parte dei giocaotri
                }

                    if(contatore==numero_giocatori){
                        flag = false;   //nel momento in cui tutti i giocatori hanno selezionato una classe accettabile, esco dal ciclo
                    }
            }   
    }

    /*Conclusa l'inizializzazione, ogni giocatore ha la possibilità 
    di sacrificare un punto mente per un punto vita e viceversa*/

    bool flag2 = true;
    int contatore2 = 0;

    while(flag2){
        int opzione;
        for(int i=0; i<numero_giocatori;i++){
            printf("\n%s sceglie una tra le seguenti opzioni: \n", giocatori[i]->nome_giocatore);
            puts("1) SACRIFICARE UN PUNTO MENTE PER UN PUNTO VITA");
            puts("2) SACRIFICARE UN PUNTO VITA PER UN PUNTO MENTE");
            puts("3) NESSUNA DELLE PRECEDENTI");
                if(scanf("%d", &opzione) != 1 || opzione<1 || opzione>3){ //se viene digitata opzione invalida richiede a tutti giocatori la possibilià
                    puts("Opzione invalida, ritenta!");
                    while(getchar() != '\n');
                    i=-1;
                }
                else{
                    switch(opzione){
                        case 1: 
                            giocatori[i]-> mente -= 1;
                            giocatori[i]-> p_vita += 1;
                        case 2:
                            giocatori[i]-> mente += 1;
                            giocatori[i]-> p_vita -= 1;
                    }
                contatore2++;
                }

                    if(contatore2==numero_giocatori){
                        flag2 = false;
                    }
        }   
    }

    //Creazione Mappa
    
    int opzione;
    unsigned int numero_zone=0;
    do{
        puts("\n GameMaster imposta la mappa di gioco: \n");
        puts("1) GENERA MAPPA");
        puts("2) INSERISCI ZONA");
        puts("3) CANCELLA ZONA");
        puts("4) STAMPA MAPPA");
        puts("5) CHIUDI MAPPA");

            if(scanf("%d", &opzione) != 1 || opzione<1 || opzione>5){
                puts("Scelta invalida, ritenta!\n");
                while(getchar() != '\n');
                continue;
            }
            else{
                switch(opzione){
                    case 1:
                        genera_mappa();
                        numero_zone = 15;
                        puts("Mappa creata con successo!");
                        break;
                    case 2:
                        //inserisci_zona();
                        numero_zone++;
                        break;
                    case 3:
                        //cancella_zona();
                        numero_zone--;
                        break;
                    case 4:
                        stampa_mappa();
                        break;
                    case 5:
                        //chiudi_mappa();
                        break;
                }
            }

    }while(opzione != 5);



 }       
 

Zona_segrete* zone[30]={ NULL };    //in  questa maniera tutti elementi dell'array sono settati inizialmente a NULL

 //Ad ogni chiamata di questa funzione vengono create/sovrascritte 15 zone
 void genera_mappa(void){
    puts("\nGenerazione 15 zone arbitrariamente in corso...\n");

    time_t h;
    srand((unsigned) time(&h));
    int random_porta = rand()%3; //3 = numero porte

    for(int i=0;i<15;i++){
        Zona_segrete* nuova_zona = (Zona_segrete*) malloc(sizeof(Zona_segrete));  //Creo array di struct in memoria dinamica
        zone[i] = nuova_zona;
        int random_zona = rand()%10;    // 10 = numero zone 
        nuova_zona->zona = random_zona;
    }
 }

 void inserisci_zona(void){
    unsigned int posizione_zona;
    while(1){
        puts("In quale posizione vuoi inserire la tua zona?");
            if(scanf("%d", &posizione_zona) && posizione_zona>=1 && posizione_zona <= numero_zone){

            }
    }
 }

 void stampa_mappa(void){
    puts("\nLa Mappa attualmente è: \n");
    int contatore=0;

    for(int i=0; i<15;i++){
        char* nome_stanza = nomi_stanze(zone[i], contatore);
        contatore++;
        printf(" %d) %s\n", i+1, nome_stanza);
    }
 }

 char* nomi_stanze(Zona_segrete* nuova_zona, int contatore){
    switch(nuova_zona->zona){
        case corridoio:
            return "corridoio";
        case scala:
            return "scala";
        case sala_banchetto:
            return "sala_banchetto";
        case magazzino:
            return "magazzino";
        case giardino:
            return "giardino";
        case posto_guardia:
            return "posto_guardia";
        case prigione:
            return "prigione";
        case cucina:
            return "cucina";
        case armeria:
            return "armeria";
        case tempio:
            return "tempio";
    }
    return NULL;
 }

    
