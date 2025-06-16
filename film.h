

#ifndef FILM_H 
#define FILM_H

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h>  

#define PRINT_LINE printf("------------------------------\n")//Makro za ispis linije radi preglednosti ispisa

typedef enum {  //Nabrajanje opcija izbornika koje se koriste u main.c
    DODAJ_FILM = 1,
    PRIKAZI_FILMOVE,
    AZURIRAJ_FILM,
    OBRISI_FILM,
    SORTIRAJ_FILMOVE,
    PRETRAZI_FILM,
    REKURZIVNI_ISPIS,
    BACKUP_FILMOVA,
    IZLAZ,
} IzbornikOpcije;

typedef struct {  //Struktura koja opisuje jedan film (ID, naziv, žanr, trajanje)
    int id;
    char naziv[100];
    char zanr[50];
    float trajanje;
} Film;

typedef struct cvor { //Struktura čvora povezane liste, sadrži podatke o filmu i pokazivač na sljedeći čvor
    Film film;
    struct cvor* sljedeci;
} Cvor;

extern Cvor* glava;

void prikaziIzbornik();
void dodajFilm(Cvor** glava);
void prikaziFilmove(const Cvor* glava);
void ispisiRekurzivno(const Cvor* glava);
void azurirajFilm(Cvor* glava);
void obrisiFilm(Cvor** glava);
void ucitajFilmove(Cvor** glava);
void spremiFilmove(const Cvor* glava);
void sortirajFilmove(Cvor** glava);
void pretraziFilm(const Cvor* glava);
void pretraziFilmBsearch(Cvor* glava);
void oslobodiMemoriju(Cvor** glava);
void backupFilmova();

#endif 



