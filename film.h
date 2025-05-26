
#ifndef FILM_H
#define FILM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Enum za izbornik
typedef enum {
    DODAJ_FILM = 1,
    PRIKAZI_FILMOVE,
    AZURIRAJ_FILM,
    OBRISI_FILM,
    SORTIRAJ_FILMOVE,
    PRETRAZI_FILM,
    IZLAZ
} IzbornikOpcije;

// Struktura za film
typedef struct {
    int id;
    char naziv[100];
    char zanr[50];
    float trajanje;
} Film;

// Èvor povezane liste
typedef struct cvor {
    Film film;
    struct cvor* sljedeci;
} Cvor;

// Funkcije
void prikaziIzbornik();
void dodajFilm(Cvor** glava);
void prikaziFilmove(const Cvor* glava);
void azurirajFilm(Cvor* glava);
void obrisiFilm(Cvor** glava);
void ucitajFilmove(Cvor** glava);
void spremiFilmove(const Cvor* glava);
void sortirajFilmove(Cvor** glava);
void pretraziFilm(const Cvor* glava);
void oslobodiMemoriju(Cvor** glava);

#endif // FILM_H
