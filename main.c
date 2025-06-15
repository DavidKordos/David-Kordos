
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "film.h"


Cvor* glava = NULL;

int main() {
    ucitajFilmove(&glava);

    int izbor;
    do {
        prikaziIzbornik();
        if (scanf("%d", &izbor) != 1) {
            fprintf(stderr, "\nNeispravan unos.\n");
            while (getchar() != '\n');
            continue;
        }

        switch ((IzbornikOpcije)izbor) {
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
            oslobodiMemoriju(&glava);
            printf("\nZatvaranje aplikacije...\n");
            break;
        default:
            printf("\nNepostojeca opcija. Pokusaj ponovno.\n");
        }
    } while (izbor != IZLAZ);

    return 0;
}


