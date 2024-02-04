#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include "gamelib.h"

static void genera_mappa(void);
static void inserisci_zona(unsigned int numero_zone);
static void cancella_zona(unsigned int numero_zone);
static void stampa_mappa(unsigned int numero_zone);
static unsigned int chiudi_mappa(unsigned int numero_zone, unsigned int opzione);
static char* nomi_stanze(Zona_segrete*);
static char* tipi_porte(Zona_segrete*);
static char* tipi_tesori(Zona_segrete*);
static int* generatore_numeri_casuali(void);
static void avanza(Giocatore*);

char* nome_stanza;
Giocatore* giocatori[4]={NULL,NULL,NULL,NULL};
unsigned short numero_giocatori;
static unsigned int controllo_mappa = 0;
static struct Zona_segrete* pFirst = NULL;
static struct Zona_segrete* pLast = NULL;


void imposta_gioco(void){
    //Quanti giocaori partecipano alla partita?
    while(1){
        puts("\nQuanti giocatori parteciperanno alla partita? (#1-4)");   //ERRORE 2fjhashg
        if(scanf("%hu", &numero_giocatori) == 1 && (numero_giocatori >= 1 && numero_giocatori <= 4)){
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
                            break;
                        case 2:
                            giocatori[i]->classe=1;
                            giocatori[i]->dadi_attacco=2;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=7;
                            giocatori[i]->mente=random_nano;
                            break;
                        case 3:
                            giocatori[i]->classe=2;
                            giocatori[i]->dadi_attacco=2;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=6;
                            giocatori[i]->mente=random_elfo;
                            break;
                        case 4:
                            giocatori[i]->classe=3;
                            giocatori[i]->dadi_attacco=1;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=4;
                            giocatori[i]->mente=random_mago;
                            break;
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
    
    unsigned int opzione;
    unsigned int numero_zone=0;
    bool generazione_mappa = false;
    do{
        puts("\nGameMaster imposta la mappa di gioco: \n");
        puts("Per prima cosa genera la mappa! Poi si può procedere nel modificarla a piacimento!\n");
        puts("1) GENERA MAPPA");
        puts("2) INSERISCI ZONA");
        puts("3) CANCELLA ZONA");
        puts("4) STAMPA MAPPA");
        puts("5) CHIUDI MAPPA");

            if(scanf("%u", &opzione) != 1 || opzione<1 || opzione>5){
                puts("Scelta invalida, ritenta!\n");
                while(getchar() != '\n');
                continue;
            }
            else{
                switch(opzione){
                    case 1:
                        genera_mappa();
                        generazione_mappa = true;
                        numero_zone = 15;
                        puts("Mappa creata con successo!");
                        break;
                    case 2:
                        if(generazione_mappa){
                            inserisci_zona(numero_zone);
                            numero_zone++;
                            puts("\nZona inserita con successo!");
                        }
                        else{
                            puts("\nPrima di poter inserire una zona è necessario selezionare l'opzione GENERA MAPPA!");
                        }
                        break;
                    case 3:
                        if(numero_zone == 2){
                            generazione_mappa = false;
                        }
                        if(generazione_mappa){
                            cancella_zona(numero_zone);
                            numero_zone--;
                            puts("\nZona cancellata con successo!");
                        }
                        else{
                            puts("\nPrima di poter cancellare una zona è necessario selezionare l'opzione GENERA MAPPA!");
                        }
                        break;
                    case 4:
                        stampa_mappa(numero_zone);
                        break;
                    case 5:
                        opzione = chiudi_mappa(numero_zone, opzione);
                        break;
                }

            while(getchar() != '\n');
            
            }

    }while(opzione != 5);
    
 system("clear");    //pulizia schermo

 }

 void gioca(void){
    int* turno_random;
    if(controllo_mappa == 1){
        for(int i=0; i<numero_giocatori;i++){
            giocatori[i] -> posizione = pFirst;
        }
        for(int i=0; i<numero_giocatori;i++){ 
            int* turno_random = generatore_numeri_casuali();
                int contatore = 0;
                do{ 
                    for(int j=0; j<numero_giocatori; j++){
                        if(turno_random[j] == i){
                            contatore++;
                                if(contatore == 3){ 
                                    printf("TURNO DI %s\n", giocatori[j] -> nome_giocatore);
                                    Giocatore* giocatore_in_turno = giocatori[j];
                                        
                                        unsigned short option;
                                        int c;
                                        bool fine_gioco = true;

                                        do{ 
                                            puts("1) AVANZA");
                                            puts("2) INDIETREGGIA");
                                            puts("3) STAMPA GIOCATORE");
                                            puts("4) STAMPA ZONA");

                                            scanf("%hu", &option);
                                            while((c = getchar()) != '\n' && c != EOF);     //pulizia buffer

                                            switch(option){
                                                case 1: 
                                                    avanza(giocatore_in_turno);
                                                    break;
                                                case 2:
                                                    //indietreggia();
                                                    break;
                                                case 3:
                                                    //stampa_giocatore();
                                                    break;
                                                case 4:
                                                    //stampa_zona();
                                                    break;
                                            }
                                        }while(fine_gioco);


                                }   
                        }
                    }
                }while(contatore < 3);
        }
    }
    else{
        puts("\n!!!La mappa non è stata impostata correttamente, ricontrolla in impostazioni!!!\n");
    }
 }
 
 //Ad ogni chiamata di questa funzione vengono create/sovrascritte 15 zone
 void genera_mappa(void){
    puts("\nGenerazione 15 zone arbitrariamente in corso...\n");

    time_t h;
    srand((unsigned) time(&h));

    for(int i=0;i<15;i++){
        Zona_segrete* nuova_zona = (Zona_segrete*) malloc(sizeof(Zona_segrete));  //Creo array di struct in memoria dinamica

        int random_zona = rand() % 10;    // 10 = numero di possibili tipi di zone 
        nuova_zona -> zona = random_zona;     //inserisco dati nella zona in memoria dinamica
        
        int random_porta = rand() % 3;    //3 = numero di possibili tipi di porte
        nuova_zona -> porta = random_porta;     //inserisco dati nella zona in memoria dinamica

        int random_tesoro = rand() % 4;     //4 = numero di possibilii tipi di tesori
        nuova_zona -> tesoro = random_tesoro;      //inserisco dati nella zona in memoria dinamica

        if(nuova_zona==NULL){
            printf("Errore: impossibile allocare memoria per la nuova zona.\n");
        }

        //Essendo una doppiamente collegata servono due puntatori: 1 per poter avanzare alla zona successiva e l'altro per indietreggiare
        if(i==0){
            nuova_zona->zona_precedente = NULL;
            nuova_zona->zona_successiva = NULL;
            pFirst = nuova_zona;    //Memorizzo l'indirizzo del primo node in pFirst
        }
        else{
            nuova_zona->zona_precedente = pLast; 
            nuova_zona->zona_successiva = NULL;
            pLast-> zona_successiva = nuova_zona;   //Memorizzo l'indirizzo dell'ulitmo in pLast
        }

        pLast = nuova_zona;      //aggiorna il puntatore all'ulitmo nodo

    }
 }

 void inserisci_zona(unsigned int numero_zone){
    unsigned int posizione_zona;
    unsigned int tipo_zona;
    
        do{
        puts("\nQuale tipo di zona vuoi inserire? (#1-10)\n");
        puts("1) CORRIDOIO");
        puts("2) SCALA");
        puts("3) SALA BANCHETTO");
        puts("4) MAGAZZINO");
        puts("5) GIARDINO");
        puts("6) POSTO GUARDIA");
        puts("7) PRIGIONE");
        puts("8) CUCINA");
        puts("9) ARMERIA");
        puts("10) TEMPIO");

        if(scanf("%u", &tipo_zona)==1 && tipo_zona>=1 && tipo_zona<=10){
            break;
        }
        else{
            puts("\nScegli una opzione valida!\n");
        }
        }while(tipo_zona<1 || tipo_zona>10);

        while(1){
        puts("\nIn quale posizione vuoi inserire la tua zona?");
            if(scanf("%u", &posizione_zona) == 1 && posizione_zona>=1 && posizione_zona <= numero_zone+1){
                Zona_segrete* nuova_zona = (Zona_segrete*) malloc(sizeof(Zona_segrete));
                nuova_zona->zona = tipo_zona -1;

                    if(posizione_zona==1){      //inserzione zona nella posizione di head nella linked list
                        nuova_zona->zona_precedente = NULL;
                        nuova_zona->zona_successiva = pFirst;
                        pFirst->zona_precedente = nuova_zona;
                        pFirst = nuova_zona;
                    }
                    else if(posizione_zona>=2 && posizione_zona<=numero_zone){    //inserzione zona in una posizione intermedia fra due nodi
                        Zona_segrete* pScan = pFirst;
                        for(int i=0;i<posizione_zona-2 && pScan != NULL;i++){
                            pScan = pScan -> zona_successiva;
                        }
                        nuova_zona -> zona_successiva = (pScan -> zona_successiva);
                        (pScan -> zona_successiva) -> zona_precedente = nuova_zona;
                        pScan -> zona_successiva = nuova_zona;
                        nuova_zona -> zona_precedente = pScan;
                    }
                    else if(posizione_zona==numero_zone+1){
                        nuova_zona->zona_precedente = pLast;
                        nuova_zona->zona_successiva = NULL;
                        pLast->zona_successiva = nuova_zona;
                        pLast = nuova_zona;
                    }
                    else if(pFirst == NULL){
                        puts("No Node In The List!");
                    }
                    else{
                        puts("Errore nel inserire la zona!");
                    }
            }
             else{
                printf("\nNumero zona digitato invalido, ritenta! Hai inserito %d zone fino ad ora.\n", numero_zone);
                continue;
            }
            break;
        }
 }

 void cancella_zona(unsigned int numero_zone){
    unsigned int zona_eliminata;
    while(1){
        puts("\nIn quale posizione si trova la zona che vuoi cancellare?");
        if(scanf("%u", &zona_eliminata) == 1 && zona_eliminata>=1 && zona_eliminata <= numero_zone){
            if(pFirst == NULL){
                puts("No Node In The List!");
            }
            else if(zona_eliminata == 1){
                Zona_segrete* pScan = pFirst -> zona_successiva;
                free(pFirst);
                pFirst = pScan;
                pFirst -> zona_precedente = NULL;
            }
            else if(zona_eliminata >= 2 && zona_eliminata < numero_zone){
                Zona_segrete* pScan = pFirst;
                Zona_segrete* temp = NULL;
                for(int i=0; i < zona_eliminata-2 && pScan != NULL; i++){
                    pScan = pScan -> zona_successiva;
                }
                temp = (pScan -> zona_successiva) -> zona_successiva;
                free(pScan -> zona_successiva);
                pScan -> zona_successiva = temp;
            }
            else if(zona_eliminata == numero_zone){
                Zona_segrete* pScan =  pFirst;

                do{
                    pScan = pScan -> zona_successiva;
                }while((pScan -> zona_successiva) != pLast);

                free(pLast);
                pLast = pScan;
                pLast -> zona_successiva = NULL;
            }
            else{
                puts("Scegli una zona valida da cancellare nella tua mappa attuale!");
            }
        break;
        }
    }
}

 void stampa_mappa(unsigned int numero_zone){
    puts("\nLa Mappa attualmente è: \n");

    Zona_segrete* scanner = pFirst;

    for(int i=1; i<=numero_zone;i++){
        char* nome_stanza = nomi_stanze(scanner);
        char* tipo_porta = tipi_porte(scanner);
        char* tipo_tesoro = tipi_tesori(scanner);
        printf(" %2d) %-15s \t %-20s \t %s \n\n", i, nome_stanza, tipo_porta, tipo_tesoro);
        scanner = scanner -> zona_successiva;    //aggiorna il puntatore alla zona successiva
    }
 }

 unsigned int chiudi_mappa(unsigned int numero_zone, unsigned int opzione){
    if(numero_zone>=15){
        puts("\nLa mappa è stata impostata correttamente!\n");
        puts("Si può iniziare a giocare!\n");
        controllo_mappa = 1;
    }
    else{
        puts("\nIl gioco non è stato impostato correttamente!\n");
        puts("Il minimo numero di zone nella mappa di gioco deve essere 15!");
        opzione = 0; 
    }

    return opzione;
 }

 

 char* tipi_porte(Zona_segrete* scanner){
    if(scanner == NULL){
        puts("error");
    }
    
    switch(scanner -> porta){
        case nessuna_porta:
            return "nessuna porta";
        case porta_normale:
            return "porta normale";
        case porta_da_scassinare:
            return "porta da scassinare";
    }

    return NULL;

 }

 char* tipi_tesori(Zona_segrete* scanner){
    if(scanner == NULL){
        puts("error");
    }

    switch(scanner -> tesoro){
        case nessun_tesoro:
            return "nessun tesoro";
        case veleno:
            return "veleno";
        case guarigione:
            return "guarigione";
        case doppia_guarigione:
            return "doppia_guarigione";
    }

    return NULL;

 }

 int* generatore_numeri_casuali(void){
    time_t h;
    srand((unsigned) time(&h));
    int* numeri_generati = (int*)malloc(numero_giocatori * sizeof(int));     

        for(int i=0; i<numero_giocatori; i++){
            int new_number;
            bool flag_duplicato;

            do{
            flag_duplicato = false;
                new_number = rand() % numero_giocatori;       //genera numero in maniera arbitraria da 0 a 3

                for(int j=0; j<i; j++){
                    if(new_number == numeri_generati[j]){
                        flag_duplicato = true;       //se il numero è già stato generato interviene con la flag
                        break;
                    }
                }
            }while(flag_duplicato);     //continua a generare numeri casuali finchè ne ho 4 tuti diversi tra loro

            numeri_generati[i] = new_number;    
        }

    printf("\n");
    return numeri_generati;
 }

void avanza(Giocatore* giocatore_in_turno){
    Zona_segrete* scanner = giocatore_in_turno -> posizione; //-> zona_successiva;

    if(giocatore_in_turno -> posizione -> zona_successiva != NULL){
        giocatore_in_turno -> posizione = giocatore_in_turno -> posizione -> zona_successiva;
        char* zona_aggiornata = nomi_stanze(scanner);

        printf("\n%s è avanzato nella zona succesiva!\n", giocatore_in_turno -> nome_giocatore);
        printf("zona attuale %s", zona_aggiornata);
    }
    else{
        printf("%s sei già nella zona finale, non puoi avanzare ulteriormente!",  giocatore_in_turno -> nome_giocatore);
        puts("Scegli un'altra mossa!");
    }
}

char* nomi_stanze(Zona_segrete* scanner){
    if(scanner==NULL){
        printf("error");
    }

    switch(scanner -> zona){
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