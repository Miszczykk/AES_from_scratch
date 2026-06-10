# PL - AES od zera
> *Advanced Encryption Standard* (AES) to specyfikacja symetrycznego szyfrowania elektronicznych danych. Poniższy opis dotyczy implementacji wykorzystującej klucz 128-bitowy, który charakteryzuje się wykonaniem 10 rund szyfrowania.

Algorytm przetwarza dane w blokach i składa się z następujących etapów:

1. Pobranie 16 bajtów (128 bitów) tekstu jawnego
2. Konwersja każdego bajtu na system szesnastkowy
3. Ułożenie pobranych bajtów w macierzy o wymiarach 4 × 4 (macierz stanu)
4. Dodanie Klucza
5. 9 rund:

    1. Zamiana Bajtów
    2. Przesunięcie Wierszy
    3. Mieszanie Kolumn
    4. Dodanie Klucza
6. Ostatnia runda:

    1. Zamiana Bajtów
    2. Przesunięcie Wierszy
    3. Dodanie Klucza


Aby algorytm był bezpieczny, w każdej rundzie należy używać innego klucza. Proces generowania nowych kluczy na bazie początkowego składa się z następujących etapów:

1. RotWord
2. SubWord
3. RCon
4. Wygenerowanie nowych kolumn poprzez mieszanie:
   1. pierwsza kolumna = czwarta kolumna XOR pierwsza kolumna
   2. druga kolumna = pierwsza kolumna XOR druga kolumna
   3. trzecia kolumna = druga kolumna XOR trzecia kolumna
   4. czwarta kolumna = trzecia kolumna XOR czwarta kolumna

## Terminologia
* **Tekst jawny** - oryginalne dane przed rozpoczęciem procesu szyfrowania, dzielone na bloki po 16 bajtów
* **XOR** - logiczna operacja bitowa 
* **Rijndael S-box** - tabela wyszukiwania
* **Dodanie klucza** - operacja XOR na obecnej macierzy stanu i kluczu wygenerowanym dla danej rundy
* **Zamiana bajtów** - zamiana każdego bajtu na inny z wykorzystaniem tabeli podstawień (Rijndael S-box)
* **Przesunięcie wierszy** - cykliczne przesunięcie wierszy macierzy w lewo (pierwszy wiersz pozostaje bez zmian, drugi o jedną pozycję, trzeci o dwie, czwarty o trzy)
* **Mieszanie kolumn** - matematyczna transformacja kolumn polegająca na mnożeniu przez stałą macierz
* **RotWord** - przesunięcie bajtów w skrajnej prawej kolumnie w górę o jedną pozycję.
* **SubWord** - zamiana każdego bajtu z przetworzonej kolumny na inny z wykorzystaniem tabeli S-box.
* **RCon** - operacja XOR pierwszego bajtu kolumny z wartością stałą przypisaną do danej rundy

## Źródła
* [Wikipedia AES - en](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)
* [Wikipedia AES - pl](https://pl.wikipedia.org/wiki/Advanced_Encryption_Standard)
* [Wikipedia Rijndael S-box](https://en.wikipedia.org/wiki/Rijndael_S-box)
* [Wikipedia ASCII](https://pl.wikipedia.org/wiki/ASCII)
* [Wikipedia RCon](https://en.wikipedia.org/wiki/AES_key_schedule)
