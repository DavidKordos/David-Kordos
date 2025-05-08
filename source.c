#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicija strukture za Film
typedef struct {
    int id;
    char naziv[100];
    char zanr[50];
    int godina;
    int trajanje;
} Film;

// Funkcija za unos filma
void unosFilma(FILE* file) {
    Film film;

    // Unos podataka
    printf("Unesite naziv filma: ");
    fgets(film.naziv, sizeof(film.naziv), stdin);
    film.naziv[strcspn(film.naziv, "\n")] = '\0';  // Uklanja znak novog reda na kraju naziva

    printf("Unesite zanr filma: ");
    fgets(film.zanr, sizeof(film.zanr), stdin);
    film.zanr[strcspn(film.zanr, "\n")] = '\0';  // Uklanja znak novog reda na kraju žanra

    printf("Unesite godinu izlaska filma: ");
    scanf("%d", &film.godina);

    printf("Unesite trajanje filma u minutama: ");
    scanf("%d", &film.trajanje);

    // Generiraj ID
    film.id = rand();  // Možete koristiti bilo koju funkciju za generiranje ID-a

    // Pohrani film u datoteku
    fseek(file, 0, SEEK_END);  // Pomakni pokazivač na kraj datoteke
    fprintf(file, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);

    getchar();  // Čisti buffer za kasniji unos
}

// Funkcija za ispis svih filmova
void ispisFilmove(FILE* file) {
    Film film;
    fseek(file, 0, SEEK_SET);  // Pozicionira pokazivač na početak datoteke

    printf("Filmove u videoteci:\n");
    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n", &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {
        printf("ID: %d, Naziv: %s, Zanr: %s, Godina: %d, Trajanje: %d minuta\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);
    }
}

// Funkcija za ažuriranje podataka o filmu
void azurirajFilm(FILE* file) {
    int id, novaGodina, novoTrajanje;
    char noviNaziv[100], noviZanr[50];
    int found = 0;

    printf("Unesite ID filma koji želite ažurirati: ");
    scanf("%d", &id);
    getchar();  // Čisti buffer

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Ne mogu otvoriti privremenu datoteku!\n");
        return;
    }

    fseek(file, 0, SEEK_SET);  // Pozicionira pokazivač na početak datoteke

    Film film;
    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n", &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {
        if (film.id == id) {
            found = 1;
            printf("Unesite novi naziv filma: ");
            fgets(noviNaziv, sizeof(noviNaziv), stdin);
            noviNaziv[strcspn(noviNaziv, "\n")] = '\0';  // Ukloni novi red

            printf("Unesite novi zanr filma: ");
            fgets(noviZanr, sizeof(noviZanr), stdin);
            noviZanr[strcspn(noviZanr, "\n")] = '\0';  // Ukloni novi red

            printf("Unesite novu godinu izlaska: ");
            scanf("%d", &novaGodina);

            printf("Unesite novo trajanje filma: ");
            scanf("%d", &novoTrajanje);

            // Ažurirani podaci
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, noviNaziv, noviZanr, novaGodina, novoTrajanje);
        }
        else {
            // Kopiraj nepromijenjene filmove u privremenu datoteku
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);
        }
    }

    if (!found) {
        printf("Film s ID-em %d nije pronađen!\n", id);
    }

    fclose(file);
    fclose(tempFile);

    remove("films.txt");
    rename("temp.txt", "films.txt");

    file = fopen("films.txt", "r+");  // Ponovno otvori datoteku za kasniji rad
}

// Funkcija za brisanje filma
void obrisiFilm(FILE* file) {
    int id;
    int found = 0;

    printf("Unesite ID filma koji želite obrisati: ");
    scanf("%d", &id);
    getchar();  // Čisti buffer

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Ne mogu otvoriti privremenu datoteku!\n");
        return;
    }

    fseek(file, 0, SEEK_SET);  // Pozicionira pokazivač na početak datoteke

    Film film;
    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n", &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {
        if (film.id != id) {
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);
        }
        else {
            found = 1;
        }
    }

    if (!found) {
        printf("Film s ID-em %d nije pronađen!\n", id);
    }

    fclose(file);
    fclose(tempFile);

    remove("films.txt");
    rename("temp.txt", "films.txt");

    file = fopen("films.txt", "r+");  // Ponovno otvori datoteku za kasniji rad
}

// Glavni izbornik
int main() {
    FILE* file = fopen("films.txt", "r+");
    if (file == NULL) {
        file = fopen("films.txt", "w+");  // Ako datoteka ne postoji, kreiraj je
        if (file == NULL) {
            printf("Ne mogu otvoriti datoteku!\n");
            return 1;
        }
    }

    int izbor;
    do {
        printf("\n1. Unesi film\n2. Ispis svih filmova\n3. Ažuriraj film\n4. Obriši film\n5. Izlaz\n");
        printf("Odaberite opciju: ");
        scanf("%d", &izbor);
        getchar();  // Čisti buffer nakon unosa

        switch (izbor) {
        case 1:
            unosFilma(file);
            break;
        case 2:
            ispisFilmove(file);
            break;
        case 3:
            azurirajFilm(file);
            break;
        case 4:
            obrisiFilm(file);
            break;
        case 5:
            printf("Izlaz...\n");
            break;
        default:
            printf("Nevažeća opcija!\n");
        }
    } while (izbor != 5);

    fclose(file);
    return 0;
}
