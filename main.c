

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h> 
#include <stdlib.h> 
#include "film.h"


Cvor* glava = NULL;                   //Definira globalni pokazivač na prvi element povezane liste filmova

int main() {
    ucitajFilmove(&glava);          //Poziva funkciju za učitavanje filmova iz datoteke i inicijalizaciju liste

    int izbor;
    do {           
        prikaziIzbornik();
        if (scanf("%d", &izbor) != 1) {               //Učitava korisnikov odabir u izborniku
            fprintf(stderr, "\nNeispravan unos.\n");
            while (getchar() != '\n');  //Prikazuje izbornik i omogućuje korisniku izbor opcije sve dok ne odabere izlaz
            continue;
        }

        switch ((IzbornikOpcije)izbor) {   //Prebacuje izvršavanje na funkciju ovisno o izboru korisnika
        case DODAJ_FILM:
            dodajFilm(&glava); 
            break;
        case PRIKAZI_FILMOVE:
            prikaziFilmove(glava); 
            break;
        case AZURIRAJ_FILM:
            azurirajFilm(glava); 
            break;
        case OBRISI_FILM:
            obrisiFilm(&glava); 
            break;
        case SORTIRAJ_FILMOVE:
            sortirajFilmove(&glava); 
            break;
        case PRETRAZI_FILM:
            pretraziFilmBsearch(glava); 
            break;
        case REKURZIVNI_ISPIS:
            ispisiRekurzivno(glava); 
            break;
        case BACKUP_FILMOVA:
            backupFilmova(glava); 
            break;
        case IZLAZ:
            spremiFilmove(glava); 
            oslobodiMemoriju(&glava);    //Oslobađa svu dinamički alociranu memoriju prije izlaska
            printf("\nZatvaranje aplikacije...\n");
            break;
        default:
            printf("\nNepostojeca opcija. Pokusaj ponovno.\n");
        }
    } while (izbor != IZLAZ);

    return 0;
}



