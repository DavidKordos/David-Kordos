main.c
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "film.h"

int main() {

    srand((unsigned)time(NULL));  // Inicijalizacija generatora slučajnih brojeva
    FILE* file = fopen("films.txt", "r+");
    if (!file) {
        file = fopen("films.txt", "w+");  // Ako file ne postoji, kreiraj ga
        if (!file) {
            perror("Ne mogu otvoriti datoteku");
            return 1;
        }
    }

    int izbor;
    do {
        printf("\n1. Unesi film\n2. Ispis svih filmova\n3. Azuriraj film\n4. Obrisi film\n5. Izlaz\n");
        printf("Odaberite opciju: ");
        scanf("%d", &izbor);
        getchar();

        switch (izbor) {
        case 1:
            unosFilma(file);
            break;
        case 2:
            ispisFilmove(file);
            break;
        case 3:
            if (azurirajFilm()) {
                fclose(file);
                file = fopen("films.txt", "r+");  // Ponovno otvori datoteku nakon ažuriranja
            }
            break;
        case 4:
            if (obrisiFilm()) {
                fclose(file);
                file = fopen("films.txt", "r+");  // Ponovno otvori datoteku nakon brisanja
            }
            break;
        case 5:
            printf("Izlaz...\n");
            break;
        default:
            printf("Nevazeca opcija!\n");
        }
    } while (izbor != 5);

    fclose(file);
    return 0;
}

film.c
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "film.h"

void unosFilma(FILE* file) {
    Film film;

    printf("Unesite naziv filma: ");
    fgets(film.naziv, sizeof(film.naziv), stdin);
    film.naziv[strcspn(film.naziv, "\n")] = '\0';

    printf("Unesite zanr filma: ");
    fgets(film.zanr, sizeof(film.zanr), stdin);
    film.zanr[strcspn(film.zanr, "\n")] = '\0';

    printf("Unesite godinu izlaska filma: ");
    scanf("%d", &film.godina);

    printf("Unesite trajanje filma u minutama: ");
    scanf("%d", &film.trajanje);

    film.id = rand();

    fseek(file, 0, SEEK_END);  // pomakni se na kraj datoteke
    fprintf(file, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);

    getchar();  // Konzumiraj newline slovo
}

void ispisFilmove(FILE* file) {
    Film film;
    fseek(file, 0, SEEK_SET);

    printf("Filmove u videoteci:\n");

    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n",
        &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {
        printf("ID: %d, Naziv: %s, Zanr: %s, Godina: %d, Trajanje: %d minuta\n",
            film.id, film.naziv, film.zanr, film.godina, film.trajanje);
    }
}

int azurirajFilm() {
    int id, novaGodina, novoTrajanje;
    char noviNaziv[100], noviZanr[50];
    int found = 0;

    printf("Unesite ID filma koji zelite azurirati: ");
    scanf("%d", &id);
    getchar();

    // Otvoriti datoteku za čitanje i pisanje, bez brisanja sadržaja
    FILE* file = fopen("films.txt", "r+");
    if (!file) {
        perror("Greska pri otvaranju datoteke");
        return 0;
    }

    // Otvoriti privremenu datoteku za pisanje
    FILE* tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        perror("Greska pri otvaranju privremene datoteke");
        fclose(file);
        return 0;
    }

    Film film;
    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n",
        &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {

        if (film.id == id) {
            found = 1;
            printf("Unesite novi naziv filma: ");
            fgets(noviNaziv, sizeof(noviNaziv), stdin);
            noviNaziv[strcspn(noviNaziv, "\n")] = '\0';

            printf("Unesite novi zanr filma: ");
            fgets(noviZanr, sizeof(noviZanr), stdin);
            noviZanr[strcspn(noviZanr, "\n")] = '\0';

            printf("Unesite novu godinu izlaska: ");
            scanf("%d", &novaGodina);
            printf("Unesite novo trajanje filma: ");
            scanf("%d", &novoTrajanje);
            getchar();

            // Zapisati ažurirani film u privremenu datoteku
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, noviNaziv, noviZanr, novaGodina, novoTrajanje);
        }
        else {
            // Ako film nije pronađen, samo ga upisujemo u privremenu datoteku
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("Film s ID-em %d nije pronaden!\n", id);
        remove("temp.txt");  // Brisanje privremene datoteke ako film nije pronađen
        return 0;
    }

    // Obrišemo originalnu datoteku i preimenujemo privremenu u originalnu
    remove("films.txt");
    rename("temp.txt", "films.txt");
    return 1;
}

int obrisiFilm() {
    int id, found = 0;
    printf("Unesite ID filma koji zelite obrisati: ");
    scanf("%d", &id);
    getchar();

    // Otvori originalnu datoteku za čitanje i pisanje (bez brisanja sadržaja)
    FILE* file = fopen("films.txt", "r+");
    if (!file) {
        perror("Greska pri otvaranju datoteke");
        return 0;
    }

    // Otvori privremenu datoteku za pisanje
    FILE* tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        perror("Greska pri otvaranju privremene datoteke");
        fclose(file);
        return 0;
    }

    Film film;
    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n",
        &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {
        if (film.id != id) {
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);
        }
        else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("Film s ID-em %d nije pronaden!\n", id);
        remove("temp.txt");
        return 0;
    }

    remove("films.txt");
    rename("temp.txt", "films.txt");
    return 1;
}

film.h
#ifndef FILM_H
#define FILM_H

#include <stdio.h>

// Struktura koja predstavlja jedan film
typedef struct {
    int id;
    char naziv[100];
    char zanr[50];
    int godina;
    int trajanje;
} Film;

// Deklaracije funkcija
void unosFilma(FILE* file);
void ispisFilmove(FILE* file);
int azurirajFilm();
int obrisiFilm();

#endif // FILM_H


