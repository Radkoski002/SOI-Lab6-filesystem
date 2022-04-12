# SOI - Laboratorium 6 - system plików
Zadanie polegało na napisaniu prostego systemu plików dla systemu minix. System ma pojemność 256 kB i jest podzielony na 1 super-blok, 15 iwęzłów i 48 bloków danych.

### Lista poleceń:
- ```create``` - tworzy system plików jeżeli nie istnieje
- ```copy *file_name*``` - kopiuje plik do systemu plików
- ```remove *file_name*```- usuwa plik z systemu plików
- ```load *file_name*```- kopiuje plik z sytemu plików
- ```clear``` - czyści cały system plików
- ```show``` - pokazuje jakie pliki znajdują się w systemie, ile miejsca zajmują i jak są rozmieszczone

Do kompilacji można użyć polecenia ``` make ```.