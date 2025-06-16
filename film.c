

#define _CRT_SECURE_NO_WARNINGS  

#include "film.h"                

static const char* IME_DATOTEKE = "filmovi.txt";        //Definira ime glavne datoteke za spremanje filmova
static const char* BACKUP_DATOTEKA = "filmovi_backup.txt";   //Definira ime backup datoteke

void prikaziIzbornik() {                  //Ispisuje tekst izbornika korisniku
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

void dodajFilm(Cvor** glava) {    //Dinamički alocira novi čvor, unosi podatke, dodaje ga na početak liste i sprema listu u datoteku
    if (!glava) return;          //Provjerava je li pokazivač na glavu NULL, ako jest, izlazi iz funkcije
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor));      //Alocira memoriju za novi čvor liste
    if (!novi) {                               //Provjerava je li alokacija memorije bila uspješna
        perror("Greska pri alokaciji memorije");     //Ispisuje poruku o grešci ako nije
        return;                                  //Izlazi iz funkcije zbog greške
    }

    printf("Unesi ID: ");                 //Traži od korisnika unos ID-a filma
    scanf("%d", &(*novi).film.id);        //Sprema uneseni ID u strukturu film unutar novog čvora
    printf("Unesi naziv filma: ");         
    scanf(" %99[^\n]", (*novi).film.naziv);  
    printf("Unesi zanr: ");                    
    scanf(" %49[^\n]", (*novi).film.zanr);    
    printf("Unesi trajanje (u minutama): ");    
    scanf("%f", &(*novi).film.trajanje);      

    (*novi).sljedeci = *glava;    //Povezuje novi čvor na početak liste
    *glava = novi;              //Postavlja novi čvor kao glavu liste
    spremiFilmove(*glava);       //Sprema ažuriranu listu filmova u datoteku
}

void prikaziFilmove(const Cvor* glava) {   //Definira funkciju za prikaz svih filmova u listi
    printf("\n--- Popis filmova ---\n");   //Ispisuje naslov popisa filmova
    while (glava) {                       
        PRINT_LINE;                    
        printf("ID: %d\nNaziv: %s\nZanr: %s\nTrajanje: %.2f min\n", // Ispisuje podatke o filmu
            (*glava).film.id, (*glava).film.naziv, (*glava).film.zanr, (*glava).film.trajanje);
        glava = (*glava).sljedeci;           //Prelazi na sljedeći čvor u listi
    }
}
void ispisiRekurzivno(const Cvor* glava) {  //Rekurzivno ispisuje filmove, koristi statičku varijablu za brojanje poziva
    static int pozicija = 1;                //Deklarira i inicijalizira statičku varijablu za brojanje poziva
    if (!glava) {                         //Provjerava je li trenutni čvor NULL (kraj liste)
        pozicija = 1;                   //Resetira brojač pri završetku rekurzije
        return;                       //Izlaz iz funkcije
    }

    printf("[Rekurzija %d] ", pozicija++);       //Ispisuje broj rekurzije i povećava brojač
    PRINT_LINE;
    printf("ID: %d\nNaziv: %s\nZanr: %s\nTrajanje: %.2f min\n",
        (*glava).film.id, (*glava).film.naziv,        
        (*glava).film.zanr, (*glava).film.trajanje);                                              

    ispisiRekurzivno((*glava).sljedeci);        //Rekurzivno poziva funkciju za sljedeći čvor
}





void azurirajFilm(Cvor* glava) { //Traži film po ID-u i omogućuje ažuriranje njegovih podataka, zatim sprema promjene
    int id;                                     
    printf("Unesi ID filma za azuriranje: ");  
    scanf("%d", &id);                        

    while (glava) {                             
        if ((*glava).film.id == id) {      //Provjerava je li pronađen film s traženim ID-om
            printf("Unesi novi naziv: ");                 //Traži novi naziv filma
            scanf(" %99[^\n]", (*glava).film.naziv);    //Sprema novi naziv
            printf("Unesi novi zanr: ");                  
            scanf(" %49[^\n]", (*glava).film.zanr);     
            printf("Unesi novo trajanje: ");              
            scanf("%f", &(*glava).film.trajanje);       
            spremiFilmove(glava);                        //Sprema promjene u datoteku
            return;                                   //Izlazi iz funkcije nakon ažuriranja
        }
        glava = (*glava).sljedeci;                  //Prelazi na sljedeći čvor
    }
    printf("Film nije pronaden.\n");             //Ispisuje poruku ako film nije pronađen
}

void obrisiFilm(Cvor** glava) {         //Briše film po ID-u iz liste i sprema promjene
    if (!glava) return;           //Provjerava je li pokazivač na glavu NULL, ako jest, izlazi iz funkcije

    int id;                                       
    printf("Unesi ID za brisanje: ");            
    scanf("%d", &id);                          
     
    Cvor* trenutni = *glava;                  //Postavlja pokazivač na trenutni čvor na glavu liste
    Cvor* prethodni = NULL;                //Inicijalizira pokazivač na prethodni čvor kao NULL
     
    while (trenutni) {                                    
        if ((*trenutni).film.id == id) {                  //Provjerava je li pronađen film s traženim ID-om
            if (!prethodni) *glava = (*trenutni).sljedeci;        //Ako je prvi čvor, ažurira glavu liste
            else (*prethodni).sljedeci = (*trenutni).sljedeci;   //Inače preskače trenutni čvor u listi

            free(trenutni);                            //Oslobađa memoriju za obrisani čvor
            spremiFilmove(*glava);                   //Sprema ažuriranu listu u datoteku
            printf("Film obrisan.\n");             //Ispisuje poruku o uspješnom brisanju
            return;                              //Izlazi iz funkcije nakon brisanja
        }
        prethodni = trenutni;                     //Sprema trenutni čvor kao prethodni
        trenutni = (*trenutni).sljedeci;        //Prelazi na sljedeći čvor
    }
    printf("Film nije pronaden.\n");           //Ispisuje poruku ako film nije pronađen
}


void ucitajFilmove(Cvor** glava) {               //Definira funkciju za učitavanje filmova iz datoteke u povezanu listu                                         
                                           

    if (!glava) return;                    //Provjerava je li pokazivač na glavu NULL; ako jest, izlazi iz funkcije

    FILE* dat = fopen(IME_DATOTEKE, "r");                             //Otvara datoteku za čitanje
    if (!dat) {                                                     //Provjerava je li otvaranje datoteke bilo uspješno
        if (errno != ENOENT) perror("Ne mogu otvoriti datoteku");   //Ispisuje grešku ako nije zbog nepostojanja datoteke
        return;                                                     //Izlazi iz funkcije ako nije moguće otvoriti datoteku
    }

    rewind(dat);                                         //Vraća pokazivač na početak datoteke

    int id;
    char naziv[100], zanr[50];
    float trajanje;
    char linija[256];                               //Deklarira privremeni niz za čitanje linija iz datoteke
   

    while (fgets(linija, sizeof(linija), dat)) {          //Čita liniju po liniju iz datoteke dok ne dođe do kraja
        
        if (strncmp(linija, "------------------------------", 10) == 0) {                //Provjerava je li linija crta koja označava početak zapisa o filmu
            
            if (fgets(linija, sizeof(linija), dat) &&                       // Čita liniju s ID-om
                sscanf(linija, "ID: %d", &id) == 1 &&                     // Parsira ID iz linije
                fgets(linija, sizeof(linija), dat) &&                    
                sscanf(linija, "Naziv filma: %99[^\n]", naziv) == 1 &&    
                fgets(linija, sizeof(linija), dat) &&                     
                sscanf(linija, "Zanr filma: %49[^\n]", zanr) == 1 &&  
                fgets(linija, sizeof(linija), dat) &&                  
                sscanf(linija, "Trajanje filma: %f", &trajanje) == 1) {   

                Cvor* novi = (Cvor*)malloc(sizeof(Cvor));          // Dinamički alocira novi čvor za film
                if (!novi) {                                    // Provjerava je li alokacija memorije bila uspješna
                    perror("Greska alokacije");               // Ispisuje poruku o grešci ako nije uspješna
                    fclose(dat);                             // Zatvara datoteku zbog greške
                    return;                                  // Izlazi iz funkcije zbog greške
                }
                (*novi).film.id = id;                          // Sprema ID u strukturu film novog čvora
                strcpy((*novi).film.naziv, naziv);           // Sprema naziv filma u strukturu film
                strcpy((*novi).film.zanr, zanr);           // Sprema žanr filma u strukturu film
                (*novi).film.trajanje = trajanje;           // Sprema trajanje filma u strukturu film
                (*novi).sljedeci = *glava;                  // Povezuje novi čvor na početak liste
                *glava = novi;                            // Postavlja novi čvor kao glavu liste
            }
        }
    }
    fclose(dat);                                         // Zatvara datoteku nakon učitavanja svih filmova
}


void spremiFilmove(const Cvor* glava) {      //Definira funkciju za spremanje liste filmova u datoteku
    FILE* dat = fopen(IME_DATOTEKE, "w");       //Otvara datoteku za pisanje
    if (!dat) {                               // Provjerava je li otvaranje datoteke bilo uspješno
        perror("Ne mogu spremiti");         // Ispisuje grešku ako nije moguće otvoriti datoteku
        return;                           // Izlazi iz funkcije zbog greške
    } 

    rewind(dat);                           //Vraća pokazivač na početak datoteke

    while (glava) {                                        
        fprintf(dat, "------------------------------\n");
        fprintf(dat, "ID: %d\n", (*glava).film.id);         //Ispis ID-a u datoteku
        fprintf(dat, "Naziv filma: %s\n", (*glava).film.naziv);  
        fprintf(dat, "Zanr filma: %s\n", (*glava).film.zanr);    
        fprintf(dat, "Trajanje filma: %.2f min\n", (*glava).film.trajanje);   
        glava = (*glava).sljedeci;              //Prelazi na sljedeći čvor
                                                                           
    }
    fclose(dat);                                  //Zatvara datoteku nakon spremanja svih filmova
}

int compareFilmID(const void* a, const void* b) { //Definira funkciju za usporedbu filmova po ID-u za sortiranje i pretragu
    Film* fa = *(Film**)a;               //Dohvaća prvi pokazivač na strukturu Film
    Film* fb = *(Film**)b;               //Dohvaća drugi pokazivač na strukturu Film
    return (*fa).id - (*fb).id;         //Vraća razliku između ID-eva filmova
}

void pretraziFilmBsearch(Cvor* glava) {            //Definira funkciju za binarnu pretragu filma po ID-u
    int count = 0;                                  //Inicijalizira brojač filmova
    Cvor* temp = glava;                           //Postavlja privremeni pokazivač na glavu liste
    while (temp) {                              
        count++;                                //Povećava brojač za svaki čvor
        temp = (*temp).sljedeci;              //Prelazi na sljedeći čvor
    }

    if (count == 0) {                       //Provjerava ima li filmova u listi
        printf("Nema filmova.\n");        //Ispisuje poruku ako nema filmova
        return;                             //Izlazi iz funkcije
    }

    Film** niz = malloc(count * sizeof(Film*));          //Alocira memoriju za niz pokazivača na filmove
    temp = glava;                                      //Ponovno postavlja temp na glavu liste
    for (int i = 0; i < count; i++) {                //Petlja prolazi kroz sve filmove
        niz[i] = &(*temp).film;                    //Sprema adresu filma u niz
        temp = (*temp).sljedeci;                 //Prelazi na sljedeći čvor
    }

    qsort(niz, count, sizeof(Film*), compareFilmID);      //Sortira niz filmova po ID-u
     
    int trazeni;                                          //varijabla za trazeni ID
    printf("Unesi ID za pretragu: ");                   
    scanf("%d", &trazeni);                            

    Film key = { .id = trazeni };                   //Kreira privremenu strukturu s traženim ID-om
    Film* keyPtr = &key;                          //Sprema adresu privremene strukture

    Film** pronadjen = bsearch(&keyPtr, niz, count, sizeof(Film*), compareFilmID);    //Binarno pretražuje niz po ID-u
    if (pronadjen)                                                                  //Provjerava je li pronađen film
        printf("Pronaden: %s (%s) %.2f min\n",                                   //Ispisuje podatke o pronađenom filmu
            (*(*pronadjen)).naziv, (*(*pronadjen)).zanr, (*(*pronadjen)).trajanje);    //Ispisuje naziv, žanr i trajanje
    else
        printf("Film nije pronaden.\n");                           //Ispisuje poruku ako film nije pronađen

    free(niz);                                                   //Oslobađa memoriju za niz pokazivača
}

int usporediFilmoveNaziv(const void* a, const void* b) {   //Definira funkciju za usporedbu filmova po nazivu za sortiranje
    Film* f1 = *(Film**)a;                          //Dohvaća prvi pokazivač na film
    Film* f2 = *(Film**)b;                           //Dohvaća drugi pokazivač na film
    return strcmp((*f1).naziv, (*f2).naziv);       //Vraća rezultat usporedbe naziva filmova
}

int usporediFilmoveTrajanje(const void* a, const void* b) { //Definira funkciju za usporedbu filmova po trajanju za sortiranje
    Film* f1 = *(Film**)a;                          //Dohvaća prvi pokazivač na film
    Film* f2 = *(Film**)b;                        //Dohvaća drugi pokazivač na film
    return ((*f1).trajanje > (*f2).trajanje) - ((*f1).trajanje < (*f2).trajanje);   //Vraća rezultat usporedbe trajanja filmova
}

void sortirajFilmove(Cvor** glava) {     //Definira funkciju za sortiranje filmova po nazivu ili trajanju
    if (!glava || !*glava) return;              //Provjerava je li lista prazna ili pokazivač NULL 
        
    int count = 0;                             //Inicijalizira brojač filmova
    Cvor* temp = *glava;                        //Postavlja privremeni pokazivač na glavu liste
    while (temp) {                            
        count++;                                //Povećava brojač za svaki čvor
        temp = (*temp).sljedeci;              //Prelazi na sljedeći čvor
    }

    Film** niz = malloc(count * sizeof(Film*));          //Alocira memoriju za niz pokazivača na filmove
    temp = *glava;                                     //Ponovno postavlja temp na glavu liste
    for (int i = 0; i < count; i++) {                //Petlja prolazi kroz sve filmove
        niz[i] = &(*temp).film;                    //Sprema adresu filma u niz
        temp = (*temp).sljedeci;                 //Prelazi na sljedeći čvor
    }

    int nacin;                                                 //Deklarira varijablu za način sortiranja
    printf("Sortirati po: 0 - Naziv, 1 - Trajanje: ");       //Traži od korisnika izbor načina sortiranja
    scanf("%d", &nacin);                                   //Sprema izbor korisnika
                                                 
    if (nacin == 0)                                                   //Provjerava je li odabrano sortiranje po nazivu
        qsort(niz, count, sizeof(Film*), usporediFilmoveNaziv);       //Sortira po nazivu    
    else
        qsort(niz, count, sizeof(Film*), usporediFilmoveTrajanje);    // Sortira po trajanju

    for (int i = 0; i < count; i++) {    //Petlja prolazi kroz sortirani niz
        printf("ID: %d | %s | %s | %.2f\n", (*niz[i]).id, (*niz[i]).naziv, (*niz[i]).zanr, (*niz[i]).trajanje);     //Ispisuje podatke o svakom filmu
    }

    free(niz);                    //Oslobađa memoriju za niz pokazivača
}


void backupFilmova() {                                      //Definira funkciju za backup datoteke filmova
    if (rename(IME_DATOTEKE, BACKUP_DATOTEKA) == 0)       //Pokušava preimenovati glavnu datoteku u backup
        printf("Backup uspjesan (%s -> %s).\n", IME_DATOTEKE, BACKUP_DATOTEKA);  //Ispisuje poruku o uspješnom backupu
    else
        perror("Greska kod backup-a");                    //Ispisuje poruku o grešci ako backup nije uspio
}

void oslobodiMemoriju(Cvor** glava) {          //Definira funkciju za oslobađanje memorije povezane liste
    while (*glava) {                                  
        Cvor* temp = *glava;                        //Sprema trenutni čvor u privremenu varijablu
        *glava = (*(*glava)).sljedeci;            //Pomiče glavu na sljedeći čvor
        free(temp);                             //Oslobađa memoriju za trenutni čvor
    } 
}



