# Pochylnia

Firmware dla Arduino Uno, który odczytuje położenie pochylni z trzech czujników (górny, środkowy, dolny), steruje wyjściem blokady (zwory) i wyświetla komunikaty z komputera na LCD 16x2 po I2C. Komunikacja z komputerem odbywa się przez port szeregowy.

## Funkcje

- Odczyt trzech czujników położenia: **G** (góra), **S** (środek), **D** (dół).
- Wysyłanie do komputera litery aktualnego położenia po każdej zmianie.
- Sterowanie wyjściem zwory komendami `O` (włącz) i `C` (wyłącz).
- Automatyczne wyłączenie zwory, gdy pochylnia dojedzie na dół.
- Wyświetlanie na LCD 16x2 (I2C) dowolnego tekstu wysłanego z komputera.

## Wymagania

- Arduino Uno
- Wyświetlacz LCD 16x2 z konwerterem I2C (adres `0x27`, jeśli Twój ma inny, np. `0x3F`, zmień go w `src/main.cpp`)
- Trzy czujniki lub krańcówki zwierające wejście do GND
- Element wykonawczy zwory (np. przekaźnik lub elektromagnes przez odpowiedni driver)
- [PlatformIO](https://platformio.org/) (CLI lub wtyczka do VS Code)

## Podłączenie

| Element                | Pin Arduino Uno |
|------------------------|-----------------|
| Czujnik górny (G)      | D10             |
| Czujnik środkowy (S)   | D9              |
| Czujnik dolny (D)      | D8              |
| Wyjście zwory          | D7              |
| LCD I2C – SDA          | A4              |
| LCD I2C – SCL          | A5              |
| LCD I2C – VCC / GND    | 5V / GND        |

Wejścia czujników używają wewnętrznego rezystora podciągającego (`INPUT_PULLUP`), więc czujnik ma zwierać pin do GND. Stan aktywny to `LOW`.

## Budowanie i wgrywanie

```bash
git clone https://github.com/bytewarden-dev/Pochylnia.git
cd Pochylnia

pio run                  # kompilacja
pio run -t upload        # wgranie na płytkę
pio device monitor -b 9600
```

Konfiguracja (`platformio.ini`): platforma `atmelavr`, płytka `uno`, framework `arduino`. Biblioteki: `Wire` i `marcoschwartz/LiquidCrystal_I2C`.

## Protokół szeregowy

Parametry: **9600 baud**, komendy kończone znakiem nowej linii (`\n`).

### Z Arduino do komputera

| Dane     | Znaczenie                                                        |
|----------|------------------------------------------------------------------|
| `init`   | Wysyłane raz po starcie.                                         |
| `G`      | Pochylnia w położeniu górnym.                                    |
| `S`      | Pochylnia w położeniu środkowym.                                 |
| `D`      | Pochylnia w położeniu dolnym.                                    |
| `a`      | Sygnał aktywności, wysyłany w każdym obiegu pętli (co ok. 150 ms). |

Litera położenia jest wysyłana tylko wtedy, gdy się zmieni. Przy kilku aktywnych czujnikach naraz priorytet ma G, potem S, potem D.

### Z komputera do Arduino

| Komenda | Działanie                                                              |
|---------|------------------------------------------------------------------------|
| `O`     | Ustawia wyjście zwory (D7) w stan wysoki. Zwolnienie elektro magnesu.  |
| `C`     | Ustawia wyjście zwory (D7) w stan niski. Włączenie elektro magnesu.    |

Każda odebrana linia (także `O` i `C`) jest dodatkowo wyświetlana na LCD: pierwsze 16 znaków w górnym wierszu, kolejne 16 w dolnym. Polskie znaki diakrytyczne nie są obsługiwane przez wyświetlacz HD44780.

Przykład (Linux/macOS):

```bash
stty -F /dev/ttyACM0 9600
printf 'O\n' > /dev/ttyACM0      # włącz zworę
printf 'Witaj swiecie\n' > /dev/ttyACM0
```

## Struktura projektu

```
.
├── platformio.ini   # konfiguracja PlatformIO
├── src/
│   └── main.cpp     # cały kod programu
├── include/         # nagłówki (puste)
├── lib/             # lokalne biblioteki (puste)
└── test/            # testy (puste)
```

## Uwagi

- Gdy aktywny jest czujnik dolny (D), zwora jest wyłączana automatycznie w każdym obiegu pętli. Komenda `O` odebrana w tym czasie może zostać szybko nadpisana.
- Jeśli ekran jest pusty, wyreguluj kontrast potencjometrem na module I2C i sprawdź adres skanerem I2C.

## Licencja

Repozytorium nie zawiera pliku licencji. Dodaj go, jeśli projekt ma być współdzielony.
