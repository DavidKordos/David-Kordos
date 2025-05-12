#define _CRT_SECURE_NO_WARNINGS // Omogućuje korištenje funkcija poput scanf i fopen bez upozorenja u Visual Studiju
#include <stdio.h>              // Uključuje standardnu biblioteku za ulaz/izlaz
#include <stdlib.h>             // Uključuje funkcije poput rand(), malloc() itd.
#include <string.h>             // Uključuje funkcije za obradu stringova

// Struktura koja predstavlja jedan film
typedef struct {
    int id;                 // Jedinstveni identifikator filma
    char naziv[100];        // Naziv filma
    char zanr[50];          // Žanr filma
    int godina;             // Godina izlaska
    int trajanje;           // Trajanje filma u minutama
} Film;

// Funkcija za unos novog filma u datoteku
void unosFilma(FILE* file) {
    Film film; // Novi film

    // Unos naziva filma
    printf("Unesite naziv filma: ");
    fgets(film.naziv, sizeof(film.naziv), stdin);
    film.naziv[strcspn(film.naziv, "\n")] = '\0'; // Uklanja novi red s kraja stringa

    // Unos žanra filma
    printf("Unesite zanr filma: ");
    fgets(film.zanr, sizeof(film.zanr), stdin);
    film.zanr[strcspn(film.zanr, "\n")] = '\0';

    // Unos godine izlaska i trajanja
    printf("Unesite godinu izlaska filma: ");
    scanf("%d", &film.godina);

    printf("Unesite trajanje filma u minutama: ");
    scanf("%d", &film.trajanje);

    // Generiranje nasumičnog ID-a
    film.id = rand();

    // Pozicioniraj na kraj datoteke i upiši novi film
    fseek(file, 0, SEEK_END);
    fprintf(file, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);

    getchar(); // Čisti buffer od viška znakova (npr. Enter)
}

// Funkcija za ispis svih filmova iz datoteke
void ispisFilmove(FILE* file) {
    Film film;

    // Postavi čitanje na početak datoteke
    fseek(file, 0, SEEK_SET);

    printf("Filmove u videoteci:\n");

    // Čitanje i ispis svakog filma sve dok ne dođemo do kraja datoteke
    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n", &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {
        printf("ID: %d, Naziv: %s, Zanr: %s, Godina: %d, Trajanje: %d minuta\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);
    }
}

// Funkcija za ažuriranje filma po ID-u
int azurirajFilm() {
    int id, novaGodina, novoTrajanje;
    char noviNaziv[100], noviZanr[50];
    int found = 0; // Oznaka je li film pronađen

    // Unos ID-a filma koji se ažurira
    printf("Unesite ID filma koji zelite azurirati: ");
    scanf("%d", &id);
    getchar();

    // Otvaranje postojeće datoteke za čitanje
    FILE* file = fopen("films.txt", "r");
    if (file == NULL) {
        perror("Greska pri otvaranju datoteke");
        return 0;
    }

    // Otvaranje privremene datoteke za pisanje izmjena
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Greska pri otvaranju privremene datoteke");
        fclose(file);
        return 0;
    }

    Film film;
    // Čitanje svakog filma i upis u privremenu datoteku, ažuriraj ako ID odgovara
    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n", &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {
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

            // Upis ažuriranih podataka
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, noviNaziv, noviZanr, novaGodina, novoTrajanje);
        }
        else {
            // Ako nije taj ID, samo kopiraj podatke
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);
        }
    }

    // Zatvori obje datoteke
    fclose(file);
    fclose(tempFile);

    // Ako nije pronađen film za ažuriranje, izbriši privremenu datoteku
    if (!found) {
        printf("Film s ID-em %d nije pronaden!\n", id);
        remove("temp.txt");
        return 0;
    }

    // Zamijeni staru datoteku novom (ažuriranom)
    remove("films.txt");
    rename("temp.txt", "films.txt");
    return 1;
}

// Funkcija za brisanje filma po ID-u
int obrisiFilm() {
    int id, found = 0;

    // Unos ID-a filma za brisanje
    printf("Unesite ID filma koji zelite obrisati: ");
    scanf("%d", &id);
    getchar();

    // Otvori datoteku za čitanje
    FILE* file = fopen("films.txt", "r");
    if (file == NULL) {
        perror("Greska pri otvaranju datoteke");
        return 0;
    }

    // Otvori privremenu datoteku za upis
    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Greska pri otvaranju privremene datoteke");
        fclose(file);
        return 0;
    }

    Film film;
    // Kopiraj sve filmove osim onog s traženim ID-em
    while (fscanf(file, "%d;%99[^;];%49[^;];%d;%d\n", &film.id, film.naziv, film.zanr, &film.godina, &film.trajanje) != EOF) {
        if (film.id != id) {
            fprintf(tempFile, "%d;%s;%s;%d;%d\n", film.id, film.naziv, film.zanr, film.godina, film.trajanje);
        }
        else {
            found = 1; // Pronađen film za brisanje
        }
    }

    // Zatvori datoteke
    fclose(file);
    fclose(tempFile);

    // Ako nije pronađen, izbriši temp datoteku
    if (!found) {
        printf("Film s ID-em %d nije pronaden!\n", id);
        remove("temp.txt");
        return 0;
    }

    // Zamijeni originalnu datoteku ažuriranom
    remove("films.txt");
    rename("temp.txt", "films.txt");
    return 1;
}

// Glavna funkcija programa
int main() {
    // Pokušaj otvoriti datoteku za čitanje i pisanje
    FILE* file = fopen("films.txt", "r+");
    if (file == NULL) {
        // Ako ne postoji, kreiraj novu
        file = fopen("films.txt", "w+");
        if (file == NULL) {
            perror("Ne mogu otvoriti datoteku");
            return 1;
        }
    }

    int izbor;
    do {
        // Prikaz izbornika
        printf("\n1. Unesi film\n2. Ispis svih filmova\n3. Azuriraj film\n4. Obrisi film\n5. Izlaz\n");
        printf("Odaberite opciju: ");
        scanf("%d", &izbor);
        getchar(); // Očisti buffer

        // Obrada korisničkog izbora
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
                file = fopen("films.txt", "r+"); // Ponovno otvori datoteku nakon izmjene
            }
            break;
        case 4:
            if (obrisiFilm()) {
                fclose(file);
                file = fopen("films.txt", "r+"); // Ponovno otvori datoteku nakon brisanja
            }
            break;
        case 5:
            printf("Izlaz...\n"); // Kraj programa
            break;
        default:
            printf("Nevazeca opcija!\n"); // Upozorenje za nepoznatu opciju
        }
    } while (izbor != 5); // Ponavljaj dok se ne odabere izlaz

    fclose(file); // Zatvori datoteku
    return 0;
}

