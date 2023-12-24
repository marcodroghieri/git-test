#include<stdio.h>
#include "gamelib.h"

int main(){
    puts("------------------BENVENUTO A SCALOGNA QUEST!!!------------------");
    int option;
    do{
        puts("1) IMPOSTA GIOCO");
        puts("2) GIOCA");
        puts("3) TERMINA GIOCO");
        printf("Risposta: ");

        if(scanf("%d", &option) != 1){
            puts("\nOpzione invalida, seleziona una tra le seguenti:\n");
            option = 0;     //reimposto valore di option per far fallire appositamente la condizione del while
        }
        
        while(getchar() != '\n');   //in caso di fallimento della funzione scanf, il buffer di input viene ripulito
        printf("\n");
        }while(option != 1 && option != 2 && option != 3);  //non dà mess opzione invalido in caso di numero non accettato ma solo per le lettere funziona
            switch(option){
                case 1:
                    imposta_gioco();
                    break;
                case 2:
                    //gioca();
                    break;
                case 3:
                    //termina_gioco();
                    break;
            }


}