

#define _CRT_SECURE_NO_WARNINGS

#include "film.h"

static const char* IME_DATOTEKE = "filmovi.txt";

// Funkcija za prikaz izbornika
void prikaziIzbornik() {
    printf("\n===== VIDEOTEKA =====\n");
    printf("1. Dodaj film\n");
    printf("2. Prikazi sve filmove\n");
    printf("3. Azuriraj film\n");
    printf("4. Obrisi film\n");
    printf("5. Sortiraj filmove\n");
    printf("6. Pretrazi film\n");
    printf("7. Izlaz\n");
    printf("Odaberi opciju: ");
}

// Funkcija za dodavanje filma
void dodajFilm(Cvor** glava) {
    if (!glava) return;
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor));
    if (!novi) {
        perror("Greska pri alokaciji memorije");
        return;
    }

    printf("Unesi ID: ");
    scanf("%d", &((*novi).film.id));
    printf("Unesi naziv filma: ");
    scanf(" %99[^\"]", (*novi).film.naziv);
    printf("Unesi zanr: ");
    scanf(" %49[^\"]", (*novi).film.zanr);
    printf("Unesi trajanje (u minutama): ");
    scanf("%f", &((*novi).film.trajanje));

    (*novi).sljedeci = *glava;
    *glava = novi;
    spremiFilmove(*glava);
}

// Funkcija za prikaz svih filmova
void prikaziFilmove(const Cvor* glava) {
    printf("\n--- Popis filmova ---\n");
    while (glava) {
        printf("ID: %d | Naziv: %s | Zanr: %s | Trajanje: %.2f min\n",
            (*glava).film.id, (*glava).film.naziv, (*glava).film.zanr, (*glava).film.trajanje);
        glava = (*glava).sljedeci;
    }
}

// Funkcija za auriranje filma
void azurirajFilm(Cvor* glava) {
    int trazeniId;
    printf("Unesi ID filma za azuriranje: ");
    scanf("%d", &trazeniId);

    while (glava) {
        if ((*glava).film.id == trazeniId) {
            printf("Unesi novi naziv: ");
            scanf(" %99[^\"]", (*glava).film.naziv);
            printf("Unesi novi zanr: ");
            scanf(" %49[^\"]", (*glava).film.zanr);
            printf("Unesi novo trajanje: ");
            scanf("%f", &((*glava).film.trajanje));
            spremiFilmove(glava);
            return;
        }
        glava = (*glava).sljedeci;
    }
    printf("Film s ID %d nije pronağen.\n", trazeniId);
}

// Funkcija za brisanje filma
void obrisiFilm(Cvor** glava) {
    if (!glava) return;

    int trazeniId;
    printf("Unesi ID filma za brisanje: ");
    scanf("%d", &trazeniId);

    Cvor* trenutni = *glava;
    Cvor* prethodni = NULL;

    while (trenutni) {
        if ((*trenutni).film.id == trazeniId) {
            if (prethodni == NULL) {
                *glava = (*trenutni).sljedeci;
            }
            else {
                (*prethodni).sljedeci = (*trenutni).sljedeci;
            }
            free(trenutni);
            spremiFilmove(*glava);
            printf("Film obrisan.\n");
            return;
        }
        prethodni = trenutni;
        trenutni = (*trenutni).sljedeci;
    }
    printf("Film nije pronağen.\n");
}

// Uèitavanje filmova iz datoteke
void ucitajFilmove(Cvor** glava) {
    if (!glava) return;

    FILE* dat = fopen(IME_DATOTEKE, "r");
    if (!dat) {
        if (errno != ENOENT) perror("Ne mogu otvoriti datoteku");
        return;
    }

    Film f;
    while (fscanf(dat, "%d,%99[^,],%49[^,],%f\n",
        &f.id, f.naziv, f.zanr, &f.trajanje) == 4) {
        Cvor* novi = (Cvor*)malloc(sizeof(Cvor));
        if (!novi) {
            perror("Greska pri alokaciji memorije");
            fclose(dat);
            return;
        }
        (*novi).film = f;
        (*novi).sljedeci = *glava;
        *glava = novi;
    }
    fclose(dat);
}

// Spremanje filmova u datoteku
void spremiFilmove(const Cvor* glava) {
    FILE* dat = fopen(IME_DATOTEKE, "w");
    if (!dat) {
        perror("Ne mogu spremiti filmove");
        return;
    }

    while (glava) {
        fprintf(dat, "%d,%s,%s,%.2f\n",
            (*glava).film.id, (*glava).film.naziv, (*glava).film.zanr, (*glava).film.trajanje);
        glava = (*glava).sljedeci;
    }
    fclose(dat);
}

// Funkcija za sortiranje
int usporediFilmoveNaziv(const void* a, const void* b) {
    const Film* f1 = *(const Film**)a;
    const Film* f2 = *(const Film**)b;
    return strcmp((*f1).naziv, (*f2).naziv);
}

int usporediFilmoveTrajanje(const void* a, const void* b) {
    const Film* f1 = *(const Film**)a;
    const Film* f2 = *(const Film**)b;
    return ((*f1).trajanje > (*f2).trajanje) - ((*f1).trajanje < (*f2).trajanje);
}

void sortirajFilmove(Cvor** glava) {
    if (!glava || !*glava) return;

    int broj = 0;
    Cvor* temp = *glava;
    while (temp) {
        broj++;
        temp = (*temp).sljedeci;
    }

    Film** niz = (Film**)malloc(broj * sizeof(Film*));
    temp = *glava;
    for (int i = 0; i < broj; i++) {
        niz[i] = &((*temp).film);
        temp = (*temp).sljedeci;
    }

    int nacin;
    printf("Sortirati po: 0 - Naziv, 1 - Trajanje: ");
    scanf("%d", &nacin);

    if (nacin == 0)
        qsort(niz, broj, sizeof(Film*), usporediFilmoveNaziv);
    else
        qsort(niz, broj, sizeof(Film*), usporediFilmoveTrajanje);

    for (int i = 0; i < broj; i++)
        printf("ID: %d | %s | %s | %.2f\n", (*niz[i]).id, (*niz[i]).naziv, (*niz[i]).zanr, (*niz[i]).trajanje);

    free(niz);
}

// Funkcija za pretraivanje
void pretraziFilm(const Cvor* glava) {
    int trazeniId;
    printf("Unesi ID filma za pretragu: ");
    scanf("%d", &trazeniId);

    while (glava) {
        if ((*glava).film.id == trazeniId) {
            printf("Pronağen: %s (%s) %.2f min\n", (*glava).film.naziv, (*glava).film.zanr, (*glava).film.trajanje);
            return;
        }
        glava = (*glava).sljedeci;
    }
    printf("Film nije pronağen.\n");
}

// Oslobodi svu zauzetu memoriju
void oslobodiMemoriju(Cvor** glava) {
    if (!glava) return;

    while (*glava) {
        Cvor* temp = *glava;
        *glava = (*(*glava)).sljedeci;
        free(temp);
    }
}


