
#define _CRT_SECURE_NO_WARNINGS

#include "film.h"

static const char* IME_DATOTEKE = "filmovi.txt";
static const char* BACKUP_DATOTEKA = "filmovi_backup.txt";

void prikaziIzbornik() {
    printf("\n===== VIDEOTEKA =====\n");
    printf("1. Dodaj film\n");
    printf("2. Prikazi sve filmove\n");
    printf("3. Azuriraj film\n");
    printf("4. Obrisi film\n");
    printf("5. Sortiraj filmove\n");
    printf("6. Pretrazi film (binary search)\n");
    printf("7. Ispis rekurzivno\n");
    printf("8. Backup filmova\n");
    printf("9. Izlaz\n");
    printf("Odaberi opciju: ");
}

void dodajFilm(Cvor** glava) {
    if (!glava) return;
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor));
    if (!novi) {
        perror("Greska pri alokaciji memorije");
        return;
    }

    printf("Unesi ID: ");
    scanf("%d", &(*novi).film.id);
    printf("Unesi naziv filma: ");
    scanf(" %99[^\n]", (*novi).film.naziv);
    printf("Unesi zanr: ");
    scanf(" %49[^\n]", (*novi).film.zanr);
    printf("Unesi trajanje (u minutama): ");
    scanf("%f", &(*novi).film.trajanje);

    (*novi).sljedeci = *glava;
    *glava = novi;
    spremiFilmove(*glava);
}

void prikaziFilmove(const Cvor* glava) {
    printf("\n--- Popis filmova ---\n");
    while (glava) {
        PRINT_LINE;
        printf("ID: %d\nNaziv: %s\nZanr: %s\nTrajanje: %.2f min\n",
            (*glava).film.id, (*glava).film.naziv, (*glava).film.zanr, (*glava).film.trajanje);
        glava = (*glava).sljedeci;
    }
}
void ispisiRekurzivno(const Cvor* glava) {
    static int pozicija = 1;
    if (!glava) {
        pozicija = 1;
        return;
    }

    printf("[Rekurzija %d] ", pozicija++);
    PRINT_LINE;
    printf("ID: %d\nNaziv: %s\nZanr: %s\nTrajanje: %.2f min\n",
        (*glava).film.id, (*glava).film.naziv,
        (*glava).film.zanr, (*glava).film.trajanje);

    ispisiRekurzivno((*glava).sljedeci);
}





void azurirajFilm(Cvor* glava) {
    int id;
    printf("Unesi ID filma za azuriranje: ");
    scanf("%d", &id);

    while (glava) {
        if ((*glava).film.id == id) {
            printf("Unesi novi naziv: ");
            scanf(" %99[^\n]", (*glava).film.naziv);
            printf("Unesi novi zanr: ");
            scanf(" %49[^\n]", (*glava).film.zanr);
            printf("Unesi novo trajanje: ");
            scanf("%f", &(*glava).film.trajanje);
            spremiFilmove(glava);
            return;
        }
        glava = (*glava).sljedeci;
    }
    printf("Film nije pronaden.\n");
}

void obrisiFilm(Cvor** glava) {
    if (!glava) return;

    int id;
    printf("Unesi ID za brisanje: ");
    scanf("%d", &id);

    Cvor* trenutni = *glava;
    Cvor* prethodni = NULL;

    while (trenutni) {
        if ((*trenutni).film.id == id) {
            if (!prethodni) *glava = (*trenutni).sljedeci;
            else (*prethodni).sljedeci = (*trenutni).sljedeci;

            free(trenutni);
            spremiFilmove(*glava);
            printf("Film obrisan.\n");
            return;
        }
        prethodni = trenutni;
        trenutni = (*trenutni).sljedeci;
    }
    printf("Film nije pronaden.\n");
}

void ucitajFilmove(Cvor** glava) {                                                  
                                           
    if (!glava) return;                 

    FILE* dat = fopen(IME_DATOTEKE, "r");                        
    if (!dat) {                                                
        if (errno != ENOENT) perror("Ne mogu otvoriti datoteku");  
        return;                                                 
    }

    rewind(dat);                                         

    int id;
    char naziv[100], zanr[50];
    float trajanje;
    char linija[256];                             
   

    while (fgets(linija, sizeof(linija), dat)) {       
        
        if (strncmp(linija, "------------------------------", 10) == 0) {               
            
            if (fgets(linija, sizeof(linija), dat) &&                     
                sscanf(linija, "ID: %d", &id) == 1 &&                    
                fgets(linija, sizeof(linija), dat) &&                  
                sscanf(linija, "Naziv filma: %99[^\n]", naziv) == 1 &&     
                fgets(linija, sizeof(linija), dat) &&                     
                sscanf(linija, "Zanr filma: %49[^\n]", zanr) == 1 &&     
                fgets(linija, sizeof(linija), dat) &&                     
                sscanf(linija, "Trajanje filma: %f", &trajanje) == 1) {   

                Cvor* novi = (Cvor*)malloc(sizeof(Cvor));         
                if (!novi) {                                    
                    perror("Greska alokacije");           
                    fclose(dat);                          
                    return;                                  
                }
                (*novi).film.id = id;                         
                strcpy((*novi).film.naziv, naziv);          
                strcpy((*novi).film.zanr, zanr);          
                (*novi).film.trajanje = trajanje;          
                (*novi).sljedeci = *glava;                
                *glava = novi;                           
            }
        }
    }
    fclose(dat);                                         
}

void spremiFilmove(const Cvor* glava) {     
    FILE* dat = fopen(IME_DATOTEKE, "w");    
    if (!dat) {                            
        perror("Ne mogu spremiti");         
        return;                           
    } 

    rewind(dat);                           

    while (glava) {                                        
        fprintf(dat, "------------------------------\n");
        fprintf(dat, "ID: %d\n", (*glava).film.id);            
        fprintf(dat, "Naziv filma: %s\n", (*glava).film.naziv);  
        fprintf(dat, "Zanr filma: %s\n", (*glava).film.zanr);   
        fprintf(dat, "Trajanje filma: %.2f min\n", (*glava).film.trajanje);   
        glava = (*glava).sljedeci;                            
                                                                           
    }
    fclose(dat);                                
}

int compareFilmID(const void* a, const void* b) {
    Film* fa = *(Film**)a;
    Film* fb = *(Film**)b;
    return (*fa).id - (*fb).id;
}

void pretraziFilmBsearch(Cvor* glava) {
    int count = 0;
    Cvor* temp = glava;
    while (temp) {
        count++;
        temp = (*temp).sljedeci;
    }

    if (count == 0) {
        printf("Nema filmova.\n");
        return;
    }

    Film** niz = malloc(count * sizeof(Film*));
    temp = glava;
    for (int i = 0; i < count; i++) {
        niz[i] = &(*temp).film;
        temp = (*temp).sljedeci;
    }

    qsort(niz, count, sizeof(Film*), compareFilmID);

    int trazeni;
    printf("Unesi ID za pretragu: ");
    scanf("%d", &trazeni);

    Film key = { .id = trazeni };
    Film* keyPtr = &key;

    Film** pronadjen = bsearch(&keyPtr, niz, count, sizeof(Film*), compareFilmID);
    if (pronadjen)
        printf("Pronaden: %s (%s) %.2f min\n",
            (*(*pronadjen)).naziv, (*(*pronadjen)).zanr, (*(*pronadjen)).trajanje);
    else
        printf("Film nije pronaden.\n");

    free(niz);
}

int usporediFilmoveNaziv(const void* a, const void* b) {
    Film* f1 = *(Film**)a;
    Film* f2 = *(Film**)b;
    return strcmp((*f1).naziv, (*f2).naziv);
}

int usporediFilmoveTrajanje(const void* a, const void* b) {
    Film* f1 = *(Film**)a;
    Film* f2 = *(Film**)b;
    return ((*f1).trajanje > (*f2).trajanje) - ((*f1).trajanje < (*f2).trajanje);
}

void sortirajFilmove(Cvor** glava) {
    if (!glava || !*glava) return;

    int count = 0;
    Cvor* temp = *glava;
    while (temp) {
        count++;
        temp = (*temp).sljedeci;
    }

    Film** niz = malloc(count * sizeof(Film*));
    temp = *glava;
    for (int i = 0; i < count; i++) {
        niz[i] = &(*temp).film;
        temp = (*temp).sljedeci;
    }

    int nacin;
    printf("Sortirati po: 0 - Naziv, 1 - Trajanje: ");
    scanf("%d", &nacin);

    if (nacin == 0)
        qsort(niz, count, sizeof(Film*), usporediFilmoveNaziv);
    else
        qsort(niz, count, sizeof(Film*), usporediFilmoveTrajanje);

    for (int i = 0; i < count; i++) {
        printf("ID: %d | %s | %s | %.2f\n", (*niz[i]).id, (*niz[i]).naziv, (*niz[i]).zanr, (*niz[i]).trajanje);
    }

    free(niz);
}


void backupFilmova() {
    if (rename(IME_DATOTEKE, BACKUP_DATOTEKA) == 0)
        printf("Backup uspjesan (%s -> %s).\n", IME_DATOTEKE, BACKUP_DATOTEKA);
    else
        perror("Greska kod backup-a");
}

void oslobodiMemoriju(Cvor** glava) {
    while (*glava) {
        Cvor* temp = *glava;
        *glava = (*(*glava)).sljedeci;
        free(temp);
    }
}

