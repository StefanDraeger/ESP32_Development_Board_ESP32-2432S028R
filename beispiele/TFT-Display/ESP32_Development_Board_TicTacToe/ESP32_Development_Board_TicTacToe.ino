/*
  Einbinden der Bibliothek zum kommunizieren
  mit dem Display.
*/
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h"

//Größe der Zeichenfläche definieren
#define MAX_X 319
#define MAX_Y 239

//Felder für die ermittelte Position
//bei einem klick
uint16_t x = 0, y = 0;


//Instanz des Displays
static LGFX lcd;

//definieren eines Feldes
struct Field {
  int32_t x;        //X Koordinate
  int32_t y;        //Y Koordinate
  bool isSelected;  //bereits belegt?
  char symbol;      //Symbol O oder X, default -
};

//Spielfeld, 3x3 Felder groß
Field playground[3][3] = {
  { { 0, 0, false, '-' }, { 0, 0, false, '-' }, { 0, 0, false, '-' } },
  { { 0, 0, false, '-' }, { 0, 0, false, '-' }, { 0, 0, false, '-' } },
  { { 0, 0, false, '-' }, { 0, 0, false, '-' }, { 0, 0, false, '-' } }
};

//Feld für den aktuellen Spieler
bool currentPlayerX = true;
//Feld ob das Spiel beendet ist
bool gameOver = false;

//Abmaße einer Spielfläche
const int32_t DIMENSION = 50;
//Abstände zwischen den Feldern
const int32_t PADDING = 10;

void setup(void) {
  //beginn der seriellen Kommunikation mit
  //115200 Baud
  Serial.begin(115200);

  //beginn der Kommunikation mit dem Display
  lcd.init();
  //drehen des Displays
  lcd.setRotation(1);
  //füllen des Displays mit der Farbe Schwarz
  lcd.fillScreen(TFT_BLACK);
  //eine kleine Pause von 100ms.
  delay(100);
  //zeichnen des Hintergrundes inkl. dem befüllen
  //der Spielfläche mit den Koordinaten der Felder
  drawBackground();
}

void loop() {
  //Wenn ein Touchaktion ausgeführt / erkannt wurde,
  //dann liefert die Funktion getTouch den Wert 1 und
  //befüllt die übergebenen Parametern mit den Koordinaten
  if (lcd.getTouch(&x, &y) == 1) {

    //Wenn die ermittelte X oder Y Positon außerhalb der Range ist,
    //dann soll die Funktion hier verlassen werden.
    if (x > MAX_X || y > MAX_Y) {
      return;
    }

    //Wenn das Spiel nicht beendet ist, dann...
    if (!gameOver) {
      //umkehren des Wertes des Feldes currentPlayerX
      //aus true wird false und andersherum
      currentPlayerX = !currentPlayerX;

      //Offset für das Symbol der Spieler
      int32_t offset = ((DIMENSION / 2) / 2) + 3;

      //For-Schleife über die Zeilen
      for (int row = 1; row < 4; row++) {
        //For-Schleife über die Spalten
        for (int col = 1; col < 4; col++) {
          //entnehmen eines Feldes aus dem Spielfeld
          Field& field = playground[row - 1][col - 1];
          //Wenn das Feld nicht belegt ist und die geklickte Koordinate
          //innerhalb dieses Feldes ist, dann...
          if (field.isSelected == false && checkCoord(field, x, y)) {
            //Feld als belegt markieren
            field.isSelected = true;
            //setzen der Schritgröße auf 4
            lcd.setFont(&fonts::Font4);
            //Schriftfarbe setzen,
            //Schriftfarbe - schwarz,
            //Hintergrund - grau
            lcd.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
            //Symbol setzen
            char symbol = currentPlayerX ? 'X' : 'O';
            //platzieren des Cursors an die X/Y Koordinate mit offset
            lcd.setCursor(field.x + offset, field.y + offset);
            //schreiben des Symbols
            lcd.print(String(symbol));
            //schreiben des Symbols auf das Feld
            field.symbol = symbol;
            //eine Pause von 250ms.
            delay(250);

            //prüfen ob das Spiel beendet ist,
            //zunächst wird geprüft ob der aktuelle Spieler gewonnen hat
            if (checkWin(symbol)) {
              //Anzeigen der Meldung das der Spieler gewonnen hat.
              showMessage("Spieler " + String(symbol) + " hat gewonnen!", 20);
              //Feld gameOver auf true setzen und damit wird im
              //nächsten Schritt der Else-Zweig betreten.
              gameOver = true;
              //eine Pause von 2 Sekunden.
              delay(2000);
            } else {
              //Wenn der aktuelle Spieler nicht gewonnen hat, dann wird geprüft
              //ob ggf. ein unentschieden besteht.
              //Im ersten Schritt wird geprüft ob noch Felder mit einem - existieren,
              //(alternativ könnte man auch das Feld isSelected wählen)
              bool playgroundContainsDash = false;
              for (int row = 1; row < 4; row++) {
                for (int col = 1; col < 4; col++) {
                  Field& field = playground[row - 1][col - 1];
                  if (field.symbol == '-') {
                    playgroundContainsDash = true;
                  }
                }
              }
              //Wenn alle Felder belegt sind, dann...
              if (!playgroundContainsDash) {
                //prüfen ob der Spieler O oder X gewonnen hat.
                //(Hier sollte eigentlich immer False als ergebnis kommen.)
                if (!checkWin('O') || !checkWin('X')) {
                  //Anzeigen der Meldung "Unentschieden"
                  showMessage("Unentschieden!", 55);
                  //das Spiel beenden
                  gameOver = true;
                }
              }
            }
          }
        }
      }
    } else {
      //Wenn das Spiel beendet ist, und der Spieler auf das Display klickt,
      //dann soll ein neues gestartet werden.
      gameOver = false;
      //das Array zurücksetzen
      initPlayground();
      //zeichnen der Spielfläche
      drawBackground();
    }
  }
  //eine Pause von 50ms.
  delay(50);
}

//Funktion zum zeichnen der Spielfläche
void drawBackground() {
  // füllen des Displays mit der Farbe Schwarz
  lcd.fillScreen(TFT_BLACK);
  //Schriftgröße 4
  lcd.setFont(&fonts::Font4);
  //Schriftfarbe setzen
  //Schriftfarbe - grau,
  //Hintergrundfarbe - schwarz
  lcd.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  //Text an der Koordinate schreiben
  lcd.drawString("Tic-Tac-Toe", 75, 25);

  //Zeichnen der Felder
  for (int row = 1; row < 4; row++) {
    for (int col = 1; col < 4; col++) {
      drawFilledRectangle(row, col);
    }
  }
}

//Zeichnet ein gefülltet Rechteck
void drawFilledRectangle(int32_t row, int32_t col) {
  //berechnen der Position der Felder aus den Daten
  //Zeile & Spalte
  int32_t x = (DIMENSION + PADDING) * col;
  int32_t y = (DIMENSION + PADDING) * row;

  //Zeichnen des gefüllten Rechtecks
  lcd.fillRect(x, y, DIMENSION, DIMENSION, TFT_LIGHTGREY);

  //Speichern der Koordinaten an dem Feld
  Field& field = playground[row - 1][col - 1];
  field.x = x;
  field.y = y;
}

//Funktion zum prüfen ob die geklickte Koordinate innerhalb des Feldes liegt
bool checkCoord(Field field, int x, int y) {
  return (x >= field.x && x <= (field.x + DIMENSION) && y >= field.y && y <= (field.y + DIMENSION));
}

//Funktion zum prüfen ob der Spieler mit dem Symbol "player" gewonnen hat.
bool checkWin(char player) {
  // Überprüfen auf horizontale Gewinnmöglichkeiten
  for (int row = 0; row < 4; row++) {
    if (playground[row][0].symbol == player && playground[row][1].symbol == player && playground[row][2].symbol == player)
      return true;
  }

  // Überprüfen auf vertikale Gewinnmöglichkeiten
  for (int col = 0; col < 4; col++) {
    if (playground[0][col].symbol == player && playground[1][col].symbol == player && playground[2][col].symbol == player)
      return true;
  }

  // Überprüfen auf diagonale Gewinnmöglichkeiten
  if (playground[0][0].symbol == player && playground[1][1].symbol == player && playground[2][2].symbol == player)
    return true;
  if (playground[0][2].symbol == player && playground[1][1].symbol == player && playground[2][0].symbol == player)
    return true;

  // Wenn keine Gewinnmöglichkeiten gefunden wurden
  return false;
}

//Funktion zum initialisieren des Arrays.
//Diese Funktion wird aufgerufen wenn
//das Spiel beendet ist und neugestartet wird.
void initPlayground() {
  for (int row = 1; row < 4; row++) {
    for (int col = 1; col < 4; col++) {
      Field& field = playground[row - 1][col - 1];
      field.x = 0;
      field.y = 0;
      field.isSelected = false;
      field.symbol = '-';
    }
  }
}

//Funktion zum anzeigen einer Meldung auf dem Display.
//Da die Texte unterschiedlich lang sind, wird zusätzlich 
//noch die X Koordinate als Parameter übergeben.
void showMessage(String msg, int32_t x) {
  lcd.setFont(&fonts::Font4);
  lcd.setTextColor(TFT_BLACK, TFT_ORANGE);
  lcd.setCursor(x, 120);
  lcd.print(msg);
}