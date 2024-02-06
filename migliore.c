#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<math.h>
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
static void indietreggia(Giocatore*);
static void stampa_giocatore(Giocatore*);
static void stampa_zona(Giocatore*);
static int apri_porta(Giocatore*);
static void prendi_tesoro(Giocatore*);
static int combatti(Giocatore*);
static int scappa(Giocatore*);
static int gioca_potere_speciale(Giocatore*);

bool passa_turno;
char* nome_stanza;
Giocatore* giocatori[4]={NULL,NULL,NULL,NULL};
unsigned short numero_giocatori;
static unsigned int controllo_mappa = 0;
static struct Zona_segrete* pFirst = NULL;
static struct Zona_segrete* pLast = NULL;
unsigned int numero_zone;



void imposta_gioco(void){
    //Quanti giocatori partecipano alla partita?
        while(1){
             puts("\nQuanti giocatori parteciperanno alla partita? (#1-4)");   
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
                            giocatori[i]->potere_speciale=0;
                            break;
                        case 2:
                            giocatori[i]->classe=1;
                            giocatori[i]->dadi_attacco=2;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=7;
                            giocatori[i]->mente=random_nano;
                            giocatori[i]->potere_speciale=1;
                            break;
                        case 3:
                            giocatori[i]->classe=2;
                            giocatori[i]->dadi_attacco=2;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=6;
                            giocatori[i]->mente=random_elfo;
                            giocatori[i]->potere_speciale=1;
                            break;
                        case 4:
                            giocatori[i]->classe=3;
                            giocatori[i]->dadi_attacco=1;
                            giocatori[i]->dadi_difesa=2;
                            giocatori[i]->p_vita=4;
                            giocatori[i]->mente=random_mago;
                            giocatori[i]->potere_speciale=3;
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
        for(int i=0; i < numero_giocatori;i++){
            giocatori[i] -> posizione = pFirst;
        }
            while(numero_giocatori != 0){ 
                int* turno_random = generatore_numeri_casuali();
                    int contatore = 0;  //variabile utilizzata per determinare quanti giocatori hanno giocato ogni turno
                    do{ 
                        for(int i = 0; i< numero_giocatori; i++){ 
                        for(int j=0; j<numero_giocatori; j++){
                            if(turno_random[j] == i){
                                printf("numero casuale: %d", turno_random[j]);
                                printf("contatore %d", contatore);
                                    if(j == turno_random[i]){      //ricontrolla in caso fosse qui l'errore
                                        contatore++;
                                        printf("TURNO DI %s\n", giocatori[j] -> nome_giocatore);
                                        Giocatore* giocatore_in_turno = giocatori[j];

                                            unsigned short option;
                                            int c;
                                            bool passa_turno = true;

                                            do{ 
                                                puts("1) AVANZA");
                                                puts("2) INDIETREGGIA");
                                                puts("3) STAMPA GIOCATORE");
                                                puts("4) STAMPA ZONA");
                                                puts("5) PRENDI TESORO");
                                                puts("6) PASSA");

                                                if(scanf("%hu", &option) != 1){
                                                    while((c = getchar()) != '\n' && c != EOF);     //pulizia buffer
                                                    option = 7;
                                                }

                                                switch(option){
                                                    case 1: 
                                                        avanza(giocatore_in_turno);
                                                        passa_turno = false;    //dopo ogni volta che avanza il giocatore, il turno passa automaticamente al successivo
                                                        break;
                                                    case 2:
                                                        indietreggia(giocatore_in_turno);
                                                        passa_turno = false;    //dopo ogni volta che indietreggia il giocatore, il turno passa automaticamente al successivo
                                                        break;
                                                    case 3:
                                                        stampa_giocatore(giocatore_in_turno);
                                                        break;
                                                    case 4:
                                                        stampa_zona(giocatore_in_turno);
                                                        break;
                                                    case 5: 
                                                        prendi_tesoro(giocatore_in_turno);
                                                        break;
                                                    case 6:
                                                        passa_turno = false;
                                                        break;
                                                    default:
                                                        puts("\nOpzione invalida, ritenta!\n");
                                                        break;

                                                }

                                            }while(passa_turno);

                                    }   
                            }
                        if(contatore == numero_giocatori){
                            contatore = 5;    //se tutti i giocatori hanno giocato al turno corrente, interrompo ciclo do-while e inizia un nuovo turno con numeri generati casualmente per stabilire l'ordine dei giocatori nel nuovo turno
                        }
                        }
                    }
                        if(numero_giocatori == 0){
                            puts("\nLa partita è terminata!\n");
                        }

                    }while(contatore == 5);
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
            puts("Errore: impossibile allocare memoria per la nuova zona.");
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
    int avanzare = 0;   //variabile controllo per verificare se il giocatore può effettivamente avanzare
    time_t t;
    srand((unsigned) time(&t));
    int possibilità_abitante = rand() % 3 + 1;      //genera numeri da 1 a 3

        if(giocatore_in_turno -> posizione -> zona_successiva != NULL){
            if(giocatore_in_turno -> posizione -> zona_successiva -> porta == 1 || giocatore_in_turno -> posizione -> zona_successiva -> porta == 2 ){
                puts("\nC'è una porta da aprire prima di poter avanzare!");
                int avanzare = apri_porta(giocatore_in_turno);
            }
        }
        else if(giocatore_in_turno -> posizione -> zona_successiva = pLast){
            possibilità_abitante = 2;   //se il giocatore si trova nell'ulitma zona la probabilità che apparirà un abitante delle segrete è 100%
        }
        
        if(possibilità_abitante == 2){      //33% probabilità di apparire abitante delle segrete
            puts("\nL'abitante delle segrete è apparso!\n");
            
            int option = 0;
            int c = 0;
            do{
                puts("1) COMBATTI");
                puts("2) SCAPPA");
                puts("3) GIOCA POTERE SPECIALE");
                puts("Risposta: ");

                scanf("%d", &option);

                switch(option){
                case 1:
                    int check = combatti(giocatore_in_turno);   //controllo per vedere se il giocatore può avanzare
                    if(check == 1){
                        avanzare = 1;
                    }
                    break;
                case 2:
                    int check2 = scappa(giocatore_in_turno);     //controllo per vedere se il giocatore può avanzare
                    if(check2 == 1){
                        avanzare = 1;
                    }
                    break;
                case 3:
                    int controllo_uso = gioca_potere_speciale(giocatore_in_turno);      //controlla se il giocatore in questione ha sufficienti poteri speciali da utilizzare in battaglia
                    if(controllo_uso == 0){
                        option = 0;     //reimposto variabile per dare la possibilità al giocatore di selezinare un'altra opzione
                    }
                    break;
                default:
                    puts("\nOpzione non valida, ritenta!\n");
                    while((c = getchar()) != '\n' && c != EOF);     //pulizia buffer
                    break;
            }

            }while(option != 1 && option != 2 && option != 3);

        }
        else{
            avanzare = 1;
        }

    if(avanzare == 1){
         if(giocatore_in_turno -> posizione -> zona_successiva != NULL && giocatore_in_turno -> posizione -> zona_successiva != pLast){
            giocatore_in_turno -> posizione = giocatore_in_turno -> posizione -> zona_successiva;
            printf("\n%s è avanzato nella zona successiva!\n", giocatore_in_turno -> nome_giocatore);
            giocatore_in_turno -> posizione -> tesoro_preso = 0;    //se il giocatore decide di tornare in una zona in cui già era passato ha sempre la stessa possibilità di raccogliere un tesoro diverso o uguale spawnato
        }
        else if(giocatore_in_turno -> posizione -> zona_successiva == pLast){
            printf("\n%s è arrivato nella zona finale!",  giocatore_in_turno -> nome_giocatore);
            printf("Hai vinto la partita!\n");
            numero_giocatori = 0;
        }
    }
       
}

void indietreggia(Giocatore* giocatore_in_turno){
    int indietreggiare = 0;
    time_t h;
    srand((unsigned) time(&h));
    int possibilità_abitante = rand() % 3 + 1;  //genera numeri da 1 a 3
        if(possibilità_abitante == 2){      //33% di apparire abitante delle segrete
            puts("\nDevi combattere l'abitante delle segrete per poter indiettreggiare!\n");

                int option = 0;
                int clear = 0;
                do{
                    puts("Cosa vuoi fare?\n");
                    puts("1) COMBATTI");
                    puts("2) SCAPPA");
                    puts("3) GIOCA POTERE SPECIALE");

                    scanf("%d", &option);

                    switch(option){
                        case 1:
                            int check = combatti(giocatore_in_turno);   //controllo per vedere se il giocatore può avanzare
                            if(check == 1){
                                indietreggiare = 1;
                            }
                            break;
                        case 2:
                            int check2 = scappa(giocatore_in_turno);    //controllo per vedere se il giocatore può avanzare
                            if(check2 == 1){
                                indietreggiare = 1;
                            }
                            break;
                        case 3:
                            int controllo_uso = gioca_potere_speciale(giocatore_in_turno);      //controlla se il giocatore in questione ha sufficienti poteri speciali da utilizzare in battaglia
                            if(controllo_uso == 0){
                                option = 0;     //reimposto variabile per dare la possibilità al giocatore di selezinare un'altra opzione
                            }
                            break;
                        default:
                            puts("\nOpzione non valida, Ritenta!");
                            while((clear = getchar()) != '\n' && clear != EOF);     //pulizia buffer
                    }
                }while(option != 1 && option != 2 && option != 3);
        } 
        else{
            indietreggiare = 1;
        }

        if(indietreggiare == 1){
            if(giocatore_in_turno -> posizione -> zona_precedente != NULL){
                giocatore_in_turno -> posizione = giocatore_in_turno -> posizione -> zona_precedente;
                printf("\n%s è tornato nella zona precendente!\n", giocatore_in_turno -> nome_giocatore);
                giocatore_in_turno -> posizione -> tesoro_preso = 0;    //se il giocatore decide di tornare in una zona in cui già era passato ha sempre la stessa possibilità di raccogliere un tesoro diverso o uguale spawnato

            }
            else{
                printf("\n%s sei nella prima zona, non puoi indiettreggiare ulteriormente!\n",  giocatore_in_turno -> nome_giocatore);
                puts("Scegli un'altra mossa!");
            }
        }
}

void stampa_giocatore(Giocatore* giocatore_in_turno){
    printf("\nDati del giocatore %s \n", giocatore_in_turno -> nome_giocatore);

    if(giocatore_in_turno -> classe == 0){
        puts("Classe giocatore: barbaro");
    }
    else if(giocatore_in_turno -> classe == 1){
        puts("Classe giocatore: nano");
    }
    else if(giocatore_in_turno -> classe == 2){
        puts("Classe giocatore: elfo");
    }
    else if(giocatore_in_turno -> classe == 3){
        puts("Classe giocatore: mago");
    }
    else{
        puts("ERRORE: nessuna classe identificata.");
    }

    if(giocatore_in_turno -> posizione != pFirst){
        Zona_segrete* scanner = giocatore_in_turno -> posizione -> zona_precedente -> zona_successiva;
        char* zona_giocatore = nomi_stanze(scanner);
        printf("Posizione attuale del giocatore: %s\n", zona_giocatore);
    }
    else if(giocatore_in_turno -> posizione == pFirst){
        Zona_segrete* scanner = giocatore_in_turno -> posizione -> zona_successiva -> zona_precedente;
        char* zona_giocatore = nomi_stanze(scanner);
        printf("Posizione attuale del giocatore: %s\n", zona_giocatore);
    }

    printf("Punti vita del giocatore: %d\n", giocatore_in_turno -> p_vita);
    printf("Dadi di attacco: %d\n", giocatore_in_turno -> dadi_attacco);
    printf("Dadi di difesa: %d\n", giocatore_in_turno -> dadi_difesa);
    printf("Punti mente: %d\n", giocatore_in_turno -> mente);
    printf("Potere speciale: %d\n\n", giocatore_in_turno -> potere_speciale);
}

void stampa_zona(Giocatore* giocatore_in_turno){
    if(giocatore_in_turno -> posizione != pFirst){
        Zona_segrete* scanner = giocatore_in_turno -> posizione -> zona_precedente -> zona_successiva;
        char* zona_giocatore_attuale = nomi_stanze(scanner);
        printf("\nValori della zona in cui si trova il giocatore: %s", giocatore_in_turno -> nome_giocatore);
        printf("Zona attuale: %s\n", zona_giocatore_attuale);
    }
    else if(giocatore_in_turno -> posizione == pFirst){
        Zona_segrete* scanner = giocatore_in_turno -> posizione -> zona_successiva -> zona_precedente;
        char* zona_giocatore_attuale = nomi_stanze(scanner);
        printf("\nValori della zona in cui si trova il giocatore: %s", giocatore_in_turno -> nome_giocatore);
        printf("Zona attuale: %s\n", zona_giocatore_attuale);
    }

    if((giocatore_in_turno -> posizione -> tesoro) == 1 || (giocatore_in_turno -> posizione -> tesoro) == 2 || (giocatore_in_turno -> posizione -> tesoro) == 3){
        puts("C'è un tesoro presente in questa zona!");
    }
    else if((giocatore_in_turno -> posizione -> tesoro) == 0){
        puts("Nessuno tesoro presente in questa zona!");
    }
   
    if((giocatore_in_turno -> posizione -> porta) == 1 || (giocatore_in_turno -> posizione -> porta) == 2){
        puts("C'è una porta normale o da scassinare in questa zona!\n");
    }
    else if((giocatore_in_turno -> posizione -> porta) == 0){
        puts("Nessuna porta presente in questa zona!\n");
    }
 }

 static int apri_porta(Giocatore* giocatore_in_turno){
    time_t h;
    srand((unsigned) time(&h));

    if(giocatore_in_turno -> posizione -> zona_successiva -> porta == 2){
        puts("La porta va scassinata!");
        int tiro_dado = rand() % 6 + 1;

            if(tiro_dado <= (giocatore_in_turno -> mente)){
                puts("La fortuna è dalla tua parte, puoi avanzare senza ripercussioni!");
                return 1;
            }
            else{
                int possibilità = rand() % 10 + 1;
                    if(possibilità == 1){
                        puts("Sei tornato alla prima zona!");
                        giocatore_in_turno -> posizione = pFirst;
                        return 0;
                    }
                    else if(possibilità >= 2 && possibilità <= 5){
                        puts("Devi combattere un abitante delle segrete!");
                        combatti(giocatore_in_turno);
                    }
                    else if(possibilità >= 6 && possibilità <= 10){
                        puts("Hai perso un punto vita!");
                        giocatore_in_turno -> p_vita = giocatore_in_turno -> p_vita - 1;
                    }
            }
    }

    if(giocatore_in_turno -> posizione -> zona_successiva -> porta == 1){
        puts("La porta è normale!");
        return 1;
    }

 }

 void prendi_tesoro(Giocatore* giocatore_in_turno){

        
        if(giocatore_in_turno -> posizione -> tesoro_preso == 1){
            puts("\nHai già preso il tesoro!");
            puts("Seleziona un'altra opzione!\n");
        }
        else if(giocatore_in_turno -> posizione -> tesoro == 0){
            puts("\nNessuno tesoro disponibile!\n");
        }
        else if(giocatore_in_turno -> posizione -> tesoro == 1){
            puts("\nHai trovato il veleno! Perdi due punti vita!\n");
            giocatore_in_turno -> p_vita = giocatore_in_turno -> p_vita - 2;
            giocatore_in_turno -> posizione -> tesoro_preso = 1;
                if(giocatore_in_turno -> p_vita == 0 || giocatore_in_turno -> p_vita >= 200){
                    printf("Il giocatore %s è stato ucciso!\n", giocatore_in_turno -> nome_giocatore);
                    free(giocatore_in_turno);
                    numero_giocatori--;
                }
        }
        else if(giocatore_in_turno -> posizione -> tesoro == 2){
            puts("\nHai trovato una guarigione! Hai guadagnato un punto vita!\n");
            giocatore_in_turno -> p_vita = giocatore_in_turno -> p_vita + 1;
            giocatore_in_turno -> posizione -> tesoro_preso = 1;
        }
        else if(giocatore_in_turno -> posizione -> tesoro == 3){
            puts("\nHai trova una doppia guarigione! Hai guadagnato due punti vita!\n");
            giocatore_in_turno -> p_vita = giocatore_in_turno -> p_vita + 2;
            giocatore_in_turno -> posizione -> tesoro_preso = 1;
        }
 }

//L'abitante delle segrete ha uguali punti vita, dadi di attaco, dadi di difesa rispetto al giocatore che sta combattendo per rendere l'incontro il più bilanciato possibile
 int combatti(Giocatore* giocatore_in_turno){
    //inizializzazione dell'abitante che il giocatore in questione dovrà affrontare
     Abitante* nuovo_abitante = (Abitante*) malloc(sizeof(Abitante)); 
     nuovo_abitante -> p_vita = giocatore_in_turno -> p_vita;
     nuovo_abitante -> dadi_attacco = giocatore_in_turno -> dadi_attacco;
     nuovo_abitante -> dadi_difesa = giocatore_in_turno -> dadi_difesa;

    time_t t;
    srand((unsigned) time(&t));
    int lancio_giocatore = rand() % 6 + 1;
    int lancio_abitante = rand() % 6 + 1;

        if(lancio_giocatore >= lancio_abitante){
            //puts("\nInizia il giocatore!\n");
                time_t k;
                srand((unsigned) time(&k));
                int contatore_teschi_giocatore = 0;     //variabili per tenere traccia di quante volte è stato lanciato un teschio,scudo
                int contatore_teschi_abitante = 0;
                int contatore_scudi_bianchi_giocatore = 0;
                int contatore_scudi_neri_abitante = 0;

                do{ 
                    //printf("Turno di %s\n", giocatore_in_turno -> nome_giocatore);
                    for(int i=0; i < giocatore_in_turno -> dadi_attacco; i++){
                        int dado_giocatore = rand() % 6 + 1;
                        int dado_abitante = rand() % 6 + 1;
                            if(dado_giocatore >= 1 && dado_giocatore <= 3){     // tira un numero compreso fra 1 e 3 conta come teschio 50% che accada
                                contatore_teschi_giocatore++;
                            }
                            if(dado_abitante == 6){
                                contatore_scudi_neri_abitante++;    //se l'abitante tira 6 si difende da un teschio, 1/6 prossibilità di difendersi
                            }
                    }

                    if(contatore_teschi_giocatore > contatore_scudi_neri_abitante){
                        //printf("Hai inflitto %d danni all'abitante delle segrete\n", contatore_teschi_giocatore - contatore_scudi_neri_abitante);
                        nuovo_abitante -> p_vita = (nuovo_abitante -> p_vita) - (contatore_teschi_giocatore - contatore_scudi_neri_abitante);   //aggiorno punti vita inflitti all'abitante
                        contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                        contatore_teschi_abitante = 0;
                        contatore_scudi_bianchi_giocatore = 0;
                        contatore_scudi_neri_abitante = 0;

                    }
                    else if(contatore_scudi_neri_abitante >= contatore_teschi_giocatore){
                        //puts("Attacco non andato a buon fine!");
                        //puts("Non sei riuscito ad infliggere danni all'abitante delle segrete!");
                        contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                        contatore_teschi_abitante = 0;
                        contatore_scudi_bianchi_giocatore = 0;
                        contatore_scudi_neri_abitante = 0;
                    }

                    //puts("Turno dell'abitante delle segrete!");
                    for(int i=0; i < giocatore_in_turno -> dadi_difesa; i++){
                        int dado_giocatore = rand() % 6 + 1;
                        int dado_abitante = rand() % 6 + 1;
                            if(dado_abitante >= 1 && dado_abitante <= 3){
                                contatore_teschi_abitante++;
                            }
                            if(dado_giocatore == 5){
                                contatore_scudi_bianchi_giocatore++;
                            }
                    }

                    if(contatore_teschi_abitante > contatore_scudi_bianchi_giocatore){
                        //printf("L'abitante ti ha inflitto %d danni\n\n", contatore_teschi_abitante - contatore_scudi_bianchi_giocatore);
                        giocatore_in_turno -> p_vita = (giocatore_in_turno -> p_vita) - (contatore_teschi_abitante - contatore_scudi_bianchi_giocatore);
                        contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                        contatore_teschi_abitante = 0;
                        contatore_scudi_bianchi_giocatore = 0;
                        contatore_scudi_neri_abitante = 0;
                    }
                    else if(contatore_scudi_bianchi_giocatore >= contatore_teschi_abitante){
                        //puts("Sei riuscito a difenderti impeccabilmente! Non perdi nessun punto vita!");
                        contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                        contatore_teschi_abitante = 0;
                        contatore_scudi_bianchi_giocatore = 0;
                        contatore_scudi_neri_abitante = 0;
                    }
                }while(giocatore_in_turno -> p_vita > 0 || nuovo_abitante -> p_vita > 0);


        }
        else{
            //puts("\nInizia l'abitante delle segrete!");
                time_t k;
                srand((unsigned) time(&k));
                int contatore_teschi_giocatore = 0;     //variabili per tenere traccia di quante volte è stato lanciato un teschio,scudo
                int contatore_teschi_abitante = 0;
                int contatore_scudi_bianchi_giocatore = 0;
                int contatore_scudi_neri_abitante = 0;

                //printf("Turno dell'abitante delle segrete!\n");
                do{ 
                    for(int i=0; i < giocatore_in_turno -> dadi_difesa; i++){
                        int dado_giocatore = rand() % 6 + 1;
                        int dado_abitante = rand() % 6 + 1;
                            if(dado_abitante >= 1 && dado_abitante <= 3){     // tira un numero compreso fra 1 e 3 conta come teschio 50% che accada
                                contatore_teschi_abitante++;
                            }
                            if(dado_giocatore == 5){
                                contatore_scudi_bianchi_giocatore++;    //se l'abitante tira 6 si difende da un teschio, 1/6 prossibilità di difendersi
                            }
                    }

                    if(contatore_teschi_abitante > contatore_scudi_bianchi_giocatore){
                        //printf("L'abitante ti ha inflitto %d danni\n\n", contatore_teschi_abitante - contatore_scudi_bianchi_giocatore);
                        giocatore_in_turno -> p_vita = (giocatore_in_turno -> p_vita) - (contatore_teschi_abitante - contatore_scudi_bianchi_giocatore);   //aggiorno punti vita inflitti all'abitante
                        contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                        contatore_teschi_abitante = 0;
                        contatore_scudi_bianchi_giocatore = 0;
                        contatore_scudi_neri_abitante = 0;

                    }
                    else if(contatore_scudi_bianchi_giocatore >= contatore_teschi_abitante){
                        //puts("Sei riuscito a difenderti impeccabilmente! Non perdi nessun punto vita!\n");
                        contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                        contatore_teschi_abitante = 0;
                        contatore_scudi_bianchi_giocatore = 0;
                        contatore_scudi_neri_abitante = 0;
                    }

                    //printf("Turno di %s\n", giocatore_in_turno -> nome_giocatore);
                    for(int i=0; i < giocatore_in_turno -> dadi_attacco; i++){
                        int dado_giocatore = rand() % 6 + 1;
                        int dado_abitante = rand() % 6 + 1;
                            if(dado_giocatore >= 1 && dado_giocatore <= 3){
                                contatore_teschi_giocatore++;
                            }
                            if(dado_abitante == 6){
                                contatore_scudi_neri_abitante++;
                            }
                    }

                    if(contatore_teschi_giocatore > contatore_scudi_neri_abitante){
                        //printf("Hai inflitto %d danni\n", contatore_teschi_giocatore - contatore_scudi_neri_abitante);
                        nuovo_abitante -> p_vita = (nuovo_abitante -> p_vita) - (contatore_teschi_giocatore - contatore_scudi_neri_abitante);
                        contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                        contatore_teschi_abitante = 0;
                        contatore_scudi_bianchi_giocatore = 0;
                        contatore_scudi_neri_abitante = 0;
                    }
                    else if(contatore_scudi_neri_abitante >= contatore_teschi_giocatore){
                        //puts("Attacco non andato a buon fine!");
                        //puts("Non sei riuscito ad infliggere danni all'abitante delle segrete!");
                        contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                        contatore_teschi_abitante = 0;
                        contatore_scudi_bianchi_giocatore = 0;
                        contatore_scudi_neri_abitante = 0;
                    }

                }while(giocatore_in_turno -> p_vita > 0 || nuovo_abitante -> p_vita > 0);

        }

        if(nuovo_abitante -> p_vita <= 0){
            printf("%s è riuscito a sconfiggere l'abitante e può avanzare/indiettreggiare nella zona desiderata!\n\n", giocatore_in_turno ->nome_giocatore);
            free(nuovo_abitante);
            return 1;
        }
        else if(giocatore_in_turno -> p_vita <= 0){
            printf("%s è stato sconfitto dall'abitante!\n\n", giocatore_in_turno -> nome_giocatore);
            free(giocatore_in_turno);
            free(nuovo_abitante);
            numero_giocatori--;
            return 0;
        }
 }

 int scappa(Giocatore* giocatore_in_turno){
    time_t h;
    srand((unsigned) time(&h));
    int possibilità_scappare = rand() % 6 + 1;
        if(possibilità_scappare <= giocatore_in_turno -> mente){
            puts("\nHai schivato l'abitante, ma non ti muovi!\n");
            return 0;
        }
        else{
            puts("\nNon sei riuscito a scappare dall'abitante!");
            puts("Ti difenderai dall'attacco con il numero di dadi dimezzati (arrotondati per difetto)!");

                //inizializzazione dell'abitante che il giocatore in questione dovrà affrontare
                Abitante* nuovo_abitante = (Abitante*) malloc(sizeof(Abitante)); 
                nuovo_abitante -> p_vita = giocatore_in_turno -> p_vita;
                nuovo_abitante -> dadi_attacco = giocatore_in_turno -> dadi_attacco;
                nuovo_abitante -> dadi_difesa = giocatore_in_turno -> dadi_difesa;
            
                time_t t;
                srand((unsigned) time(&t));
                int lancio_giocatore = rand() % 6 + 1;
                int lancio_abitante = rand() % 6 + 1;
            
                    if(lancio_giocatore >= lancio_abitante){
                        //puts("Inizia il giocatore!\n");
                            time_t k;
                            srand((unsigned) time(&k));
                            int contatore_teschi_giocatore = 0;     //variabili per tenere traccia di quante volte è stato lanciato un teschio,scudo
                            int contatore_teschi_abitante = 0;
                            int contatore_scudi_bianchi_giocatore = 0;
                            int contatore_scudi_neri_abitante = 0;
            
                            do{ 
                                //printf("Turno di %s\n", giocatore_in_turno -> nome_giocatore);
                                for(int i=0; i < floor(giocatore_in_turno -> dadi_attacco / 2); i++){
                                    int dado_giocatore = rand() % 6 + 1;
                                        if(dado_giocatore >= 1 && dado_giocatore <= 3){     // tira un numero compreso fra 1 e 3 conta come teschio 50% che accada
                                            contatore_teschi_giocatore++;
                                        }
                                }
                                for(int i=0; i < giocatore_in_turno -> dadi_attacco; i++){
                                    int dado_abitante = rand() % 6 + 1;
                                    if(dado_abitante == 6){
                                        contatore_scudi_neri_abitante++;    //se l'abitante tira 6 si difende da un teschio, 1/6 prossibilità di difendersi con scudo
                                    }
                                }
            
                                if(contatore_teschi_giocatore > contatore_scudi_neri_abitante){
                                    //printf("Hai inflitto %d danni all'abitante delle segrete\n", contatore_teschi_giocatore - contatore_scudi_neri_abitante);
                                    nuovo_abitante -> p_vita = (nuovo_abitante -> p_vita) - (contatore_teschi_giocatore - contatore_scudi_neri_abitante);   //aggiorno punti vita inflitti all'abitante
                                    contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                                    contatore_teschi_abitante = 0;
                                    contatore_scudi_bianchi_giocatore = 0;
                                    contatore_scudi_neri_abitante = 0;
            
                                }
                                else if(contatore_scudi_neri_abitante >= contatore_teschi_giocatore){
                                    //puts("\nAttacco non andato a buon fine!");
                                    //puts("Non sei riuscito ad infliggere danni all'abitante delle segrete!");
                                    contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                                    contatore_teschi_abitante = 0;
                                    contatore_scudi_bianchi_giocatore = 0;
                                    contatore_scudi_neri_abitante = 0;
                                }
            
                                //puts("Turno dell'abitante delle segrete!");
                                for(int i=0; i < giocatore_in_turno -> dadi_difesa; i++){
                                    int dado_abitante = rand() % 6 + 1;
                                        if(dado_abitante >= 1 && dado_abitante <= 3){
                                            contatore_teschi_abitante++;
                                        }
                                }      
                                for(int i=0; i < floor(giocatore_in_turno -> dadi_difesa / 2); i++){
                                    int dado_giocatore = rand() % 6 + 1;
                                         if(dado_giocatore == 5){
                                            contatore_scudi_bianchi_giocatore++;
                                        }
                                }
                                    
                                
            
                                if(contatore_teschi_abitante > contatore_scudi_bianchi_giocatore){
                                    //printf("L'abitante ti ha inflitto %d danni\n", contatore_teschi_abitante - contatore_scudi_bianchi_giocatore);
                                    giocatore_in_turno -> p_vita = (giocatore_in_turno -> p_vita) - (contatore_teschi_abitante - contatore_scudi_bianchi_giocatore);
                                    contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                                    contatore_teschi_abitante = 0;
                                    contatore_scudi_bianchi_giocatore = 0;
                                    contatore_scudi_neri_abitante = 0;
                                }
                                else if(contatore_scudi_bianchi_giocatore >= contatore_teschi_abitante){
                                    //puts("Sei riuscito a difenderti impeccabilmente! Non perdi nessun punto vita!");
                                    contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                                    contatore_teschi_abitante = 0;
                                    contatore_scudi_bianchi_giocatore = 0;
                                    contatore_scudi_neri_abitante = 0;
                                }
                            }while(giocatore_in_turno -> p_vita > 0 && nuovo_abitante -> p_vita > 0);
            
            
                    }
                    else{
                        //puts("Inizia l'abitante delle segrete!\n");
                            time_t k;
                            srand((unsigned) time(&k));
                            int contatore_teschi_giocatore = 0;     //variabili per tenere traccia di quante volte è stato lanciato un teschio,scudo
                            int contatore_teschi_abitante = 0;
                            int contatore_scudi_bianchi_giocatore = 0;
                            int contatore_scudi_neri_abitante = 0;
            
                            //printf("Turno dell'abitante delle segrete!\n");
                            do{ 
                                for(int i=0; i < giocatore_in_turno -> dadi_difesa; i++){
                                    int dado_abitante = rand() % 6 + 1;
                                        if(dado_abitante >= 1 && dado_abitante <= 3){     // tira un numero compreso fra 1 e 3 conta come teschio 50% che accada
                                            contatore_teschi_abitante++;
                                        }
                                       
                                }
                                for(int i=0; i < floor(giocatore_in_turno -> dadi_difesa / 2); i++){
                                    int dado_giocatore = rand() % 6 + 1;
                                     if(dado_giocatore == 5){
                                        contatore_scudi_bianchi_giocatore++;    //se l'abitante tira 6 si difende da un teschio, 1/6 prossibilità di difendersi
                                    }
                                }
            
                                if(contatore_teschi_abitante > contatore_scudi_bianchi_giocatore){
                                    //printf("L'abitante ti ha inflitto %d danni\n", contatore_teschi_abitante - contatore_scudi_bianchi_giocatore);
                                    giocatore_in_turno -> p_vita = (giocatore_in_turno -> p_vita) - (contatore_teschi_abitante - contatore_scudi_bianchi_giocatore);   //aggiorno punti vita inflitti all'abitante
                                    contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                                    contatore_teschi_abitante = 0;
                                    contatore_scudi_bianchi_giocatore = 0;
                                    contatore_scudi_neri_abitante = 0;
            
                                }
                                else if(contatore_scudi_bianchi_giocatore >= contatore_teschi_abitante){
                                    //puts("Sei riuscito a difenderti impeccabilmente! Non perdi nessun punto vita!\n");
                                    contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                                    contatore_teschi_abitante = 0;
                                    contatore_scudi_bianchi_giocatore = 0;
                                    contatore_scudi_neri_abitante = 0;
                                }
            
                                //printf("Turno di %s\n", giocatore_in_turno -> nome_giocatore);
                                for(int i=0; i < giocatore_in_turno -> dadi_attacco; i++){
                                    int dado_abitante = rand() % 6 + 1;
                                        if(dado_abitante == 6){
                                            contatore_scudi_neri_abitante++;
                                        }
                                }
                                for(int i=0; i < floor(giocatore_in_turno -> dadi_attacco / 2); i++){
                                    int dado_giocatore = rand() % 6 + 1;
                                        if(dado_giocatore >= 1 && dado_giocatore <= 3){
                                            contatore_teschi_giocatore++;
                                        }
                                }
            
                                if(contatore_teschi_giocatore > contatore_scudi_neri_abitante){
                                    //printf("Hai inflitto %d danni\n", contatore_teschi_giocatore - contatore_scudi_neri_abitante);
                                    nuovo_abitante -> p_vita = (nuovo_abitante -> p_vita) - (contatore_teschi_giocatore - contatore_scudi_neri_abitante);
                                    contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                                    contatore_teschi_abitante = 0;
                                    contatore_scudi_bianchi_giocatore = 0;
                                    contatore_scudi_neri_abitante = 0;
                                }
                                else if(contatore_scudi_neri_abitante >= contatore_teschi_giocatore){
                                    //puts("Attacco non andato a buon fine!\n");
                                    //puts("Non sei riuscito ad infliggere danni all'abitante delle segrete!");
                                    contatore_teschi_giocatore = 0;     //reimposto i contatori a 0 prima che rinizi il turno successivo
                                    contatore_teschi_abitante = 0;
                                    contatore_scudi_bianchi_giocatore = 0;
                                    contatore_scudi_neri_abitante = 0;
                                }

                            }while(giocatore_in_turno -> p_vita > 0 && nuovo_abitante -> p_vita > 0);
            
                    }
            
                    if(nuovo_abitante -> p_vita <= 0){
                        printf("%s è riuscito a sconfiggere l'abitante e può avanzare/indiettreggiare nella zona desiderata!\n\n", giocatore_in_turno -> nome_giocatore);
                        free(nuovo_abitante);
                        return 1;
                    }
                    else if(giocatore_in_turno -> p_vita <= 0){
                        printf("%s è stato sconfitto dall'abitante!\n\n", giocatore_in_turno -> nome_giocatore);
                        free(giocatore_in_turno);
                        free(nuovo_abitante);
                        numero_giocatori--;
                        return 0;
                    }
            }
}

int gioca_potere_speciale(Giocatore* giocatore_in_turno){
    if(giocatore_in_turno -> potere_speciale >= 1){
        puts("\nHai ucciso l'abitante delle segrete! Puoi procedere!\n");
        giocatore_in_turno -> potere_speciale = giocatore_in_turno -> potere_speciale - 1;
    }
    else{
        puts("\nNon hai poteri speciali sufficienti per poter usare quest'opzione!\n");
        return 0;
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

 