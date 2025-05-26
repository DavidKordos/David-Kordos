Videoteka - Sustav za evidenciju filmova

Ovaj projekt predstavlja jednostavan konzolni program za evidenciju filmova u videoteci, razvijen u programskom jeziku C. Sadrži potpunu implementaciju CRUID operacija, rad s datotekama, rad s pokazivačima, memorijom, strukturama, kao i dodatne funkcionalnosti poput sortiranja, pretraživanja i upravljanja memorijom.

---

 Funkcionalnosti

- Dodavanje novih filmova
- Prikaz svih spremljenih filmova
- Ažuriranje podataka postojećeg filma
- Brisanje filma iz evidencije
-  Pretraživanje filmova po ID-u
-  Sortiranje filmova po nazivu ili trajanju
-  Spremanje i učitavanje podataka iz tekstualne datoteke
-  Upravljanje dinamičkom memorijom
-  Zaštita pokazivača i provjera grešaka

---

 Tehnologije i alati

- Programski jezik: **C**
- IDE: Visual Studio (C++ Empty Project s C datotekama)
- Operativni sustav: Windows
- Datoteke: `main.c`, `film.c`, `film.h`
- Format datoteke: tekstualna (.txt)

---



---
 Kompilacija i pokretanje

 U Visual Studio:

1. Otvori **Visual Studio**.
2. Kreiraj novi **C++ Empty Project**.
3. Učitaj datoteke `main.c`, `film.c`, `film.h`.
4. U svojstva projekta:
   - Idi na `Configuration Properties > C/C++ > Advanced`
   - Promijeni `Compile As` u `Compile as C Code (/TC)`
5. Buildaj i pokreni program (`Ctrl + F5`).

---
 Pokriveni koncepti

Projekt pokriva svih **26 obaveznih koncepata** traženih u zadatku:

1. CRUID operacije
2. Primitivni tipovi podataka (`int`, `float`)
3. Složeni tipovi (`struct`, `enum`)
4. `typedef` sa `struct` i `enum`
5. Konzistentno imenovanje (`camelCase`, `PascalCase`, `snake_case`)
6. `static` lokalne i globalne varijable
7. Organiziran izvorni kôd u više datoteka
8. `extern` korišten za globalne varijable
9. Inline/makro funkcije gdje ima smisla
10. Glavni izbornik i podizbornici
11. Korištenje `enum` za izbornike
12. Pokazivači gdje su potrebni
13. Strukture + funkcije
14. Zaštita parametara i provjere
15. Statički zauzeta polja gdje treba
16. Dinamička alokacija memorije
17. Korištenje `malloc`, `calloc`, `realloc`, `free`
18. Sigurno brisanje, `NULL`-iranje pokazivača
19. Rad s tekstualnim datotekama, `fopen`, `fclose`
20. Korištenje `fseek`, `ftell`, `rewind` gdje treba
21. `remove()`, `rename()` po potrebi
22. Upravljanje pogreškama `errno`, `perror`, `strerror`, `feof`, `ferror`
23. Sortiranje uz `qsort()`
24. Pretraživanje uz `bsearch()` ili vlastiti algoritam
25. Rekurzivne funkcije (opcija: binary search ili merge sort)
26. Pokazivači na funkcije uz `qsort()` i `bsearch()`

---

 Primjer zapisa u datoteci

1,Matrix,Sci-Fi,136.00
2,Inception,Sci-Fi,148.00
3,The Godfather,Crime,175.00


---

 Napomena

- Program je pisan na **hrvatskom jeziku**
- Ne koristi specijalna slova (š, č, ž) u varijablama, ali koristi u ispisima
- Sve funkcionalnosti su ručno implementirane
 Licenca

Ovaj projekt je edukativnog karaktera i može se slobodno koristiti u obrazovne svrhe.


