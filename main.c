#include<stdio.h>
#include<stdlib.h>
#include "gamelib.h"

int main(void){
    unsigned short option = 0;
    int c = 0;
    system("clear");    //pulizia schermo

    puts("------------------BENVENUTO A SCALOGNA QUEST!!!------------------");
    do{
        puts("1) IMPOSTA GIOCO");
        puts("2) GIOCA");
        puts("3) TERMINA GIOCO");
        printf("Risposta: ");

        scanf("%hu", &option);
        while((c = getchar()) != '\n' && c != EOF);     //pulizia buffer

        switch(option){
            case 1:
                system("clear");
                imposta_gioco();
                break;
            case 2:
                system("clear");
                gioca();
                break;
            case 3:
                system("clear");
                termina_gioco();
                break;
            default:
                puts("\nOpzione invalida, Ritenta!\n");
                break;
        }
    }while(option != 3);  
}  