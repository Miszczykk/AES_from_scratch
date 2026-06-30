# PL - AES od zera

> [!IMPORTANT]
> Zaimplementowany algorytm służy jedynie w celach edukacyjnych. NIE UŻYWAĆ w projektach komercyjnych.

> Advanced Encryption Standard (AES), znany również pod oryginalną nazwą Rijndael to specyfikacja szyfrowania elektronicznych danych opracowana przez Narodowy Instytut Norm i Technologii w 2001 roku.

Jest to bardzo prosta implementacja AES napisana w celach przedstawienia podstawowego algorytmu szyfrującego. Poniższy opis dotyczy implementacji wykorzystującej **klucz 128-bitowy**, który charakteryzuje się wykonaniem **10 rund** szyfrowania. Wszystkie wizualizacje będą opierać się na tekście *"Embedded Systems"* oraz kluczu *"Strong Password!"*

## Algorytm generowania kluczy

W pierwszym etapie należy wygenerować 11 kluczy, w którym będą wykorzystane w fazie początkowej oraz w każdej rundzie szyfrowania/odszyfrowywania danych. Pierwszy klucz jest generowany za pomocą funkcji createMatrix(tekst klucza, długość tekstu) natomiast kolejne są obliczane z użyciek sekwencji wywołań copyMatrix(aktualny klucz, poprzedni klucz), rotWord(aktualny klucz), subWord(aktualny klucz), rCon(aktualny klucz, poprzedni klucz) oraz keySchedule(aktualny klucz, poprzedni klucz). Wszystkie funkcje zostaną szerzej opisane później.

```c
 unsigned char (*keys[NUMBER_OF_ROUNDS + 1])[4];
    keys[0] = createMatrix(secret_key, BUFFER_LENGTH);

    for (int round = 1; round <= NUMBER_OF_ROUNDS; round++) {
        keys[round] = malloc(SIZE_MATRIX * SIZE_MATRIX * sizeof(unsigned char));
        if (keys[round] == NULL) {
            for (int i = 0; i < round; i++) free(keys[i]);
            free(encrypted_text);
            return NULL;
        }

        copyMatrix(keys[round], keys[round-1]);
        rotWord(keys[round]);
        subWord(keys[round]);
        rCon(keys[round], round-1);
        keySchedule(keys[round], keys[round-1]);
    }
```

### createMatrix(tekst klucza, długość tekstu)

Długość tekstu klucza oczywiście powinna wynosić w naszym przypadku 16 bajtów (czyli 16 znaków), lecz ta implementacja przepuszcza przypadki tekstów krótszych. A sam tekst klucza powinien być oczywiście prywatny. Funkcja ma za zadanie przekonwertować tekst klucza na macierz 4 x 4 w którym każdy znak jest w formacie heksadecymalnym i zwrócić tą macierz. Jeżeli długość klucza nie jest wystarczająca aby wypełnić całą macierz 4 x 4 to wypełnia resztę znakiem Null (czyli w formacie heksadecymalnym 00).

```c
unsigned char (*createMatrix(const char* buffer, const size_t bytes_read))[SIZE_MATRIX] {
    //row x col
    unsigned char (*matrix)[SIZE_MATRIX] = malloc(SIZE_MATRIX * SIZE_MATRIX * sizeof(unsigned char));

    if (matrix == NULL)
        return NULL;

    int position = 0;

    for (int col = 0; col < SIZE_MATRIX; col++) {
        for (int row = 0; row < SIZE_MATRIX; row++) {
            if (position < bytes_read) {
                matrix[row][col] = buffer[position];
                position++;
            }else {
                matrix[row][col] = 0x00;
            }
        }
    }
    return matrix;
}
```

```math
\text{Strong Password!} \Longrightarrow

\begin{bmatrix}
S & n & a & o \\
t & g & s & r \\
r &  & s & d \\
o & P & w & !
\end{bmatrix}

\Longrightarrow

\begin{bmatrix}
53 & 6E & 61 & 6F \\
74 & 67 & 73 & 72 \\
72 & 20 & 73 & 64 \\
6F & 50 & 77 & 21
\end{bmatrix}
```

### copyMatrix(aktualny klucz, poprzedni klucz)

Zadaniem tej funkcji jest wypełnić macierz klucza aktualnego wartościami macierzy klucza poprzedniego

```c
void copyMatrix(unsigned char copy_matrix[SIZE_MATRIX][SIZE_MATRIX], unsigned char original_matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (copy_matrix == NULL || original_matrix == NULL)
        return;

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            copy_matrix[row][col] = original_matrix[row][col];
        }
    }
}
```
> [!NOTE]
> Ilustracja tej funkcji oraz następnych będzie pokazywana jedynie dla klucza drugiego.

```math
\text{klucz drugi} =
\begin{bmatrix}
53 & 6E & 61 & 6F \\
74 & 67 & 73 & 72 \\
72 & 20 & 73 & 64 \\
6F & 50 & 77 & 21
\end{bmatrix}
```

### rotWord(aktualny klucz)

Zadaniem tej funkcji jest przesunąć ostatnią kolumnę z prawej stronie o jedną pozycję w górę. Funkcja ta korzysta z funkcji zamian w której na początku zamienia się wartości od wiersza 2 do wiersza ostatniego (czyli 4), następnie zamienia się wartości od wiersza 1 do wiersza ostatniego, w ten sposób mam całą kolumnę przesuniętą o jedną pozycję w górę.

```c
static void reverseCol(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], int top, int bottom) {
    while (top < bottom) {
        const unsigned char temp = matrix[top][LAST_COL];
        matrix[top][LAST_COL] = matrix[bottom][LAST_COL];
        matrix[bottom][LAST_COL] = temp;

        top++;
        bottom--;
    }
}

void rotWord(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    reverseCol(matrix, 1, SIZE_MATRIX-1);
    reverseCol(matrix, 0, SIZE_MATRIX-1);
}
```

```math
\begin{bmatrix}
53 & 6E & 61 & 6F \\
74 & 67 & 73 & 72 \\
72 & 20 & 73 & 64 \\
6F & 50 & 77 & 21
\end{bmatrix}

\Longrightarrow

\begin{bmatrix}
53 & 6E & 61 & 6F \\
74 & 67 & 73 & 21 \\
72 & 20 & 73 & 64 \\
6F & 50 & 77 & 72
\end{bmatrix}

\Longrightarrow

\begin{bmatrix}
53 & 6E & 61 & 72 \\
74 & 67 & 73 & 64 \\
72 & 20 & 73 & 21 \\
6F & 50 & 77 & 6F
\end{bmatrix}
```

### subWord(aktualny klucz)

Zadaniem tej funkcji jest zamiana każdego bajtu z ostatniej kolumny na inny bajt według tabeli S-box.

```c
void subWord(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    for (int row = 0; row < SIZE_MATRIX; row++) {
        matrix[row][SIZE_MATRIX - 1] = s_box[matrix[row][SIZE_MATRIX - 1]];
    }
}
```

```math
\begin{bmatrix}
53 & 6E & 61 & 72 \\
74 & 67 & 73 & 64 \\
72 & 20 & 73 & 21 \\
6F & 50 & 77 & 6F
\end{bmatrix}

\Longrightarrow

\begin{bmatrix}
53 & 6E & 61 & 40 \\
74 & 67 & 73 & 43 \\
72 & 20 & 73 & FD \\
6F & 50 & 77 & A8
\end{bmatrix}
```

### rCon(aktualny klucz, poprzedni klucz)

Ta funkcja wykonuje operacje XOR między ostatnią kolumną z macierzą 1x4 wypełnioną potęgą dwójki danej rundy - 1 (czyli w przypadku rundy pierwszej to wykona 2^0 = 1)

```c
static unsigned char powerOfTwo[MAX_ROUND] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

void rCon(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], const int round) {
    if (matrix == NULL || round >= MAX_ROUND || round < 0)
        return;

    matrix[0][SIZE_MATRIX-1] = matrix[0][SIZE_MATRIX-1] ^ powerOfTwo[round];
}
```

```math
\begin{bmatrix}
40 \\
43 \\
FD \\
A8
\end{bmatrix}

\oplus

\begin{bmatrix}
01 \\
00 \\
00 \\
00
\end{bmatrix}

=

\begin{bmatrix}
41 \\
43 \\
FD \\
A8
\end{bmatrix}

\Longrightarrow

\begin{bmatrix}
53 & 6E & 61 & 41 \\
74 & 67 & 73 & 43 \\
72 & 20 & 73 & FD \\
6F & 50 & 77 & A8
\end{bmatrix}
```

### keySchedule(aktualny klucz, poprzedni klucz)

Zadaniem tej funkcji jest wygenerowanie nowych kolumn poprzez mieszanie:
* pierwsza kolumna = czwarta kolumna XOR stara pierwsza kolumna
* druga kolumna = pierwsza kolumna XOR stara druga kolumna
* trzecia kolumna = druga kolumna XOR stara trzecia kolumna
* czwarta kolumna = trzecia kolumna XOR stara czwarta kolumna

```c
void keySchedule(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], unsigned char prev_matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    for (int col = 0; col < SIZE_MATRIX; col++) {
        for (int row = 0; row < SIZE_MATRIX; row++) {
            matrix[row][col] = matrix[row][(col + 3) % SIZE_MATRIX] ^ prev_matrix[row][col];
        }
    }
}
```

```math
\begin{bmatrix}
41 & 53 & 6E & 61 \\
43 & 74 & 67 & 73 \\
FD & 72 & 20 & 73 \\
A8 & 6F & 50 & 77
\end{bmatrix}

\oplus

\begin{bmatrix}
53 & 6E & 61 & 41 \\
74 & 67 & 73 & 43 \\
72 & 20 & 73 & FD \\
6F & 50 & 77 & A8
\end{bmatrix}

=

\begin{bmatrix}
41 \oplus 53 & 53 \oplus 6E & 6E \oplus 61 & 61 \oplus 41 \\
43 \oplus 74 & 74 \oplus 67 & 67 \oplus 73 & 73 \oplus 43 \\
FD \oplus 72 & 72 \oplus 20 & 20 \oplus 73 & 73 \oplus FD \\
A8 \oplus 6F & 6F \oplus 50 & 50 \oplus 77 & 77 \oplus A8
\end{bmatrix}

=

\begin{bmatrix}
12 & 3D & 0F & 20 \\
37 & 13 & 14 & 30 \\
8F & 52 & 53 & 8E \\
C7 & 3F & 27 & DF
\end{bmatrix}
```

## Algorytm szyfrowania

Proces szyfrowania składa się z:
* **rundy początkowej**, w której 16 bajtowy tekst przechodzi przez funkcje createMatrix(tekst, długość tekstu), addRoundKey(macierz, klucz pierwszy)
* **9 rund**, w której macierz z początkowej rundy przechodzi przez funkcje subBytes(macierz), shiftRows(macierz), mixColumns(macierz), addRoundKey(macierz, klucz danej rundy) 9 razy
* **Ostatniej rundy**, w której przetworzona macierz po 9 rundach przechodzi przez funkcje subBytes(macierz), shiftRows(macierz), addRoundKey(macierz, jedenasty klucz). *W skrócie ta runda różni się poprzedniej brakiem funkcji mixColumns*.

> [!NOTE]
> Funkcje createMatrix(tekst, długość tekstu) opisywałem wcześniej przy tłumaczeniu algorytmu generowania kluczy, więc nie będę jej znowu tłumaczył.

Proces ten jest wykonywany tak długo aż zaszyfruje cały tekst.
```c
for (int length_string = 0; length_string < text_length; length_string+=BUFFER_LENGTH) {
        const size_t remaining_bytes = text_length - length_string;
        const size_t bytes_to_copy = remaining_bytes < BUFFER_LENGTH ? remaining_bytes : BUFFER_LENGTH;

        memset(buffer, 0, BUFFER_LENGTH + 1);
        memcpy(buffer, text + length_string, bytes_to_copy);

        unsigned char (*matrix)[4] = createMatrix(buffer, BUFFER_LENGTH);
        if (matrix == NULL)
            break;

        addRoundKey(matrix, keys[0]);
        for (int round = 1; round < NUMBER_OF_ROUNDS; round++) {
            subBytes(matrix);
            shiftRows(matrix);
            mixColumns(matrix);
            addRoundKey(matrix, keys[round]);
        }

        subBytes(matrix);
        shiftRows(matrix);
        addRoundKey(matrix, keys[10]);

        char* chunk_string = printMatrix(matrix);
        if (chunk_string != NULL) {
            memcpy(encrypted_text + current_offset, chunk_string, BUFFER_LENGTH);
            current_offset += BUFFER_LENGTH;

            free(chunk_string);
        }

        free(matrix);
    }
    encrypted_text[total_encrypted_length] = '\0';

    for (int round = 0; round <= NUMBER_OF_ROUNDS; round++) {
        free(keys[round]);
        keys[round] = NULL;
    }
```

### addRoundKey(macierz, klucz)

Zadaniem tej funkcji jest wykonać operację XOR między macierzą tekstu a macierzą klucza.

```c
void addRoundKey(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], unsigned char key[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL || key == NULL)
        return;

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            matrix[row][col] = matrix[row][col] ^ key[row][col];
        }
    }
}
```

```math
\begin{bmatrix}
45 & 64 & 20 & 74 \\
6D & 64 & 53 & 65 \\
62 & 65 & 79 & 6D \\
65 & 64 & 73 & 73
\end{bmatrix}

\oplus

\begin{bmatrix}
53 & 6E & 61 & 6F \\
74 & 67 & 73 & 72 \\
72 & 20 & 73 & 64 \\
6F & 50 & 77 & 21
\end{bmatrix}

=

\begin{bmatrix}
16 & 0A & 41 & 1B \\
19 & 03 & 20 & 17 \\
10 & 45 & 0A & 09 \\
0A & 34 & 04 & 52
\end{bmatrix}
```

### subBytes(macierz)

Zadaniem tej funkcji jest zamiana każdego bajtu na inny bajt według tabeli S-box.

```c
void subBytes(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]){
    if (matrix == NULL)
        return;

    for (int col = 0; col < SIZE_MATRIX; col++) {
        for (int row = 0; row < SIZE_MATRIX; row++) {
            matrix[row][col] = s_box[matrix[row][col]];
        }
    }
}
```

```math
\begin{bmatrix}
16 & 0A & 41 & 1B \\
19 & 03 & 20 & 17 \\
10 & 45 & 0A & 09 \\
0A & 34 & 04 & 52
\end{bmatrix}

\Longrightarrow

\begin{bmatrix}
47 & 67 & 83 & AF \\
D4 & 7B & B7 & F0 \\
CA & 6E & 67 & 01 \\
67 & 18 & F2 & 00
\end{bmatrix}
```

### shiftRows(macierz)

Zadaniem tej funkcji jest cykliczne przesunięcie bajtów w wierszach w lewo. Pierwszy wiersz pozostaje bez zmian, drugi wiersz przesuwany jest o jedną pozycję, trzeci o dwie, a czwarty o trzy pozycje. (w tej implementacji wykorzystuję również funkcję zamiany miejscami).

```c
static void reverse(unsigned char row[SIZE_MATRIX], int left, int right) {
    while (left < right) {
        const unsigned char temp = row[left];
        row[left] = row[right];
        row[right] = temp;

        left++;
        right--;
    }
}

void shiftRows(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    for (int row = 1; row < SIZE_MATRIX; row++) {
        reverse(matrix[row], 0, row-1);
        reverse(matrix[row], row, SIZE_MATRIX-1);
        reverse(matrix[row], 0, SIZE_MATRIX-1);
    }
}
```

```math
\begin{bmatrix}
47 & 67 & 83 & AF \\
D4 & 7B & B7 & F0 \\
CA & 6E & 67 & 01 \\
67 & 18 & F2 & 00
\end{bmatrix}

\Longrightarrow

\begin{bmatrix}
47 & 67 & 83 & AF \\
7B & B7 & F0 & D4 \\
67 & 01 & CA & 6E \\
00 & 67 & 18 & F2
\end{bmatrix}
```

### mixColumns(macierz)

Zadaniem tej funkcji jest transformowanie kolumn macierzy przez mnożenie w ciele Galois przez stałą macierz.

```c
static unsigned char state_matrix[SIZE_MATRIX][SIZE_MATRIX] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02},
};

static unsigned char galoisFieldMultiplication(unsigned char a, unsigned char b) {
    unsigned char result = 0;

    for (int bit = 0; bit < 8; bit++) {
        if ((b & 1) != 0)
            result ^= a;

        unsigned char high_bit_set = (a & 0x80);
        a <<= 1;

        if (high_bit_set != 0)
            a ^= 0x1B;

        b >>= 1;
    }

    return result;
}

void mixColumns(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    unsigned char new_matrix[SIZE_MATRIX][SIZE_MATRIX] = {0};

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            for (int index = 0; index < SIZE_MATRIX; index++) {
                new_matrix[row][col] ^= galoisFieldMultiplication(state_matrix[row][index], matrix[index][col]);
            }
        }
    }

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            matrix[row][col] = new_matrix[row][col];
        }
    }
}
```

```math
\text{Stała macierz}

\Longrightarrow

\begin{bmatrix}
02 & 03 & 01 & 01 \\
01 & 02 & 03 & 01 \\
01 & 01 & 02 & 03 \\
03 & 01 & 01 & 02
\end{bmatrix}
```

```math
\begin{bmatrix}
02 & 03 & 01 & 01 \\
01 & 02 & 03 & 01 \\
01 & 01 & 02 & 03 \\
03 & 01 & 01 & 02
\end{bmatrix}

\begin{bmatrix}
47 & 67 & 83 & AF \\
7B & B7 & F0 & D4 \\
67 & 01 & CA & 6E \\
00 & 67 & 18 & F2
\end{bmatrix}

=

\begin{bmatrix}
64 & 6A & C4 & BE \\
18 & 76 & 25 & 5C \\
F2 & 7B & D4 & AA \\
D7 & D1 & 94 & AF
\end{bmatrix}
```

## Algorytm odszyfrowywania

Jest to proces odwrotny od procesu szyfrowania danych. Do odszyfrowywania danych nadal potrzebne są klucze, a cały proces odszyfrowywania składa się z:
* **rundy początkowej**, w której 16 bajtowy tekst przechodzi przez funkcje createMatrix(tekst, długość tekstu), addRoundKey(macierz, klucz jedenasty)
* **9 rund**, w której macierz z początkowej rundy przechodzi przez funkcje inverseShiftRows(macierz), inverseSubBytes(macierz), addRoundKey(macierz, klucz danej rundy), inverseMixColumns(macierz) 9 razy *(klucze są wybierane w kolejności odwrotnej, czyli zaczynamy od klucza dziesiątego a kończymy na kluczu drugim)*
* **Ostatniej rundy**, w której przetworzona macierz po 9 rundach przechodzi przez funkcje inverseShiftRows(macierz), inverseSubBytes(macierz), addRoundKey(macierz, pierwszy klucz).

> [!NOTE]
> Wszystkie funkcje są operacjami odwrotnymi do tych, które znajdywały się przy algorytmie szyfrowania.

## Źródła
* [Wikipedia AES - en](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)
* [Wikipedia AES - pl](https://pl.wikipedia.org/wiki/Advanced_Encryption_Standard)
* [Wikipedia Rijndael S-box](https://en.wikipedia.org/wiki/Rijndael_S-box)
* [Wikipedia ASCII](https://pl.wikipedia.org/wiki/ASCII)
* [Wikipedia RCon](https://en.wikipedia.org/wiki/AES_key_schedule)