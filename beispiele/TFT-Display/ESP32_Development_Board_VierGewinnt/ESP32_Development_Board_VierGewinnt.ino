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


//Feld für die Daten eines Pfeiles
struct Arrow {
  int col;
  int x;  //X Koordinate
  int y;  //Y Koordinate
};

//die beiden Spieler erhalten jeweils die Farbe Gelb&Rot
//im Array jedoch werden diese durch die beiden nachfolgenden
//Symbole dargestellt.
const char PLAYER1_CHAR = 'X';
const char PLAYER2_CHAR = '0';
//Der Strich ist der default Wert
const char DASH = '-';

int triangleWidth = 25;   // Breite der Dreiecke
int startX = 50;          // Start-X-Koordinate
int startY = 45;          // Start-Y-Koordinate
int spacing = 10;          // Abstand zwischen den Dreiecken
int circleRadius = 15;    // Radius der Kreise
int circleSpacing = 20;   // Abstand zwischen den Kreisen (korrigiert)
int circleYspacing = 5;  // Abstand zwischen den Kreisen untereinander

//Anzahl der Spalten
const int COLS = 7;
//Anzahl der Zeilen
const int ROWS = 5;

//Array für die Pfeile
//das Array wird beim zeichnen befüllt
Arrow arrowFields[COLS] = {};

//Das Array mit dem Spielfeld
//initial sind alle Felder leer
//bzw. mit einem Strich markiert
char playground[ROWS][COLS] = {
  {DASH, DASH, DASH, DASH, DASH, DASH, DASH},
  {DASH, DASH, DASH, DASH, DASH, DASH, DASH},
  {DASH, DASH, DASH, DASH, DASH, DASH, DASH},
  {DASH, DASH, DASH, DASH, DASH, DASH, DASH},
  {DASH, DASH, DASH, DASH, DASH, DASH, DASH}
};

//Feld zum speichern ob das Spiel beendet ist
bool gameOver = false;

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

  //zeichnen der Spielfläche
  drawPlayground();
}

//Feld zum speichern welcher Spieler am zug ist
//beginnen tut der Spieler 1 / Gelb
bool currentPlayer1 = false;

//Funktion zum initialisieren der Spielfläche
void initPlayground() {
  //Schleife über die Spalten und Zeilen
  for (int col = 0; col < COLS; col++) {
    for (int  row = 0; row < ROWS; row++) {
      //Zuweisen eines Striches an die Position im Array
      playground[row][col] = DASH;
    }
  }
}

//Funktion zum zeichnen der Spielfläche
void drawPlayground() {
  //leeren des Displays
  lcd.clear();

  //Hintergrundfarbe Schwarz
  lcd.fillScreen(TFT_BLACK);
  //Schriftgröße 4
  lcd.setFont(&fonts::Font4);
  //Schriftfarbe Gelb,
  //Hintergrundfarbe Schwarz
  lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  //Text an die Position x & y schreiben
  lcd.drawString("Vier gewinnt!", 105, 5);
  //ein blaues rechteck zeichnen
  lcd.fillRect(40, 35, 255, 205, TFT_BLUE);

  // For-Schleife zum Zeichnen von sieben Dreiecken
  for (int i = 0; i < 7; i++) {
    // Koordinaten für die Eckpunkte des Dreiecks berechnen
    int  x1 = startX + (i * (triangleWidth + spacing));
    int  y1 = startY;
    int  x2 = x1 + triangleWidth / 2;
    int  y2 = startY + (triangleWidth / 2);
    int  x3 = x1 + triangleWidth;
    int  y3 = startY;

    // fillTriangle-Funktion aufrufen, um das Dreieck zu zeichnen
    lcd.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN);

    Arrow arrowField = {i, x1, y1};
    arrowFields[i] = arrowField;
  }
  //zeichnen des Arrays
  drawHoles();
  //anzeigen der Daten auf dem Display
  lcd.display();
}

//Funktion zum zeichnen der Daten des Arrays
void drawHoles() {
  //ein gefülltes, blaues rechteck über die Spielfläche zeichnen
  //damit werden alle zuvor gezeichneten Daten überschrieben
  lcd.fillRect(40, 60, 255, 185, TFT_BLUE);
  //Schleife über das Array
  for (int col = 0; col < COLS; col++) {
    Arrow arrowField = arrowFields[col];
    for (int  row = 0; row < ROWS; row++) {
      //Berechnen der X & Y Koordinate für das Loch
      int circleY = (startY + triangleWidth + circleSpacing + (2 * circleRadius + circleYspacing) * row) - 10;  // y-Koordinate des Kreises
      int circleX = arrowField.x + triangleWidth / 2;                                                                        // x-Koordinate des Kreises
      lcd.fillCircle(circleX, circleY, circleRadius, TFT_BLACK);

      //Wenn an der Koordinate im Array kein Strich ist, dann ist dort ein Spielstein abgelegt
      if (playground[row][col] != DASH) {
        //den Spielstein zeichnen
        drawStone(circleX, circleY, playground[row][col] == PLAYER1_CHAR);
      }
    }
  }
}

//Zeichnet ein Spielstein an die Position X & Y
//der boolsche Parameter player1 gibt an ob Gelb oder Rot am zug ist
void drawStone(int x, int y, bool player1) {
  //initial ist Gelb gesetzt
  uint8_t colorRingOuter1 = lcd.color332(137, 121, 5);;
  uint8_t colorRingOuter2 = lcd.color332(238, 226, 146);
  uint8_t colorRingInner = lcd.color332(243, 212, 0);
  //bei Spieler 2 soll ein Stein in der Farbe Rot gezeichnet werden.
  if (!player1) {
    colorRingOuter1 = lcd.color332(106, 5, 13);
    colorRingOuter2 = lcd.color332(239, 109, 119);
    colorRingInner = lcd.color332(255, 0, 0);
  }

  //zeichnen des Spielsteins
  lcd.fillCircle(x, y, 15, colorRingOuter1);
  lcd.fillCircle(x, y, 12, colorRingOuter2);
  lcd.fillCircle(x, y, 8, colorRingInner);
}

// Funktion, die prüft, ob ein Spieler gewonnen hat
bool hasPlayerWon(char player) {
  // Überprüfen von horizontalen Linien
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col <= COLS - 4; col++) {
      if (playground[row][col] == player &&
          playground[row][col + 1] == player &&
          playground[row][col + 2] == player &&
          playground[row][col + 3] == player) {
        return true;
      }
    }
  }

  // Überprüfen von vertikalen Linien
  for (int col = 0; col < COLS; col++) {
    for (int row = 0; row <= ROWS - 4; row++) {
      if (playground[row][col] == player &&
          playground[row + 1][col] == player &&
          playground[row + 2][col] == player &&
          playground[row + 3][col] == player) {
        return true;
      }
    }
  }

  // Überprüfen von diagonalen Linien (von links oben nach rechts unten)
  for (int row = 0; row <= ROWS - 4; row++) {
    for (int col = 0; col <= COLS - 4; col++) {
      if (playground[row][col] == player &&
          playground[row + 1][col + 1] == player &&
          playground[row + 2][col + 2] == player &&
          playground[row + 3][col + 3] == player) {
        return true;
      }
    }
  }

  // Überprüfen von diagonalen Linien (von rechts oben nach links unten)
  for (int row = 0; row <= ROWS - 4; row++) {
    for (int col = 3; col < COLS; col++) {
      if (playground[row][col] == player &&
          playground[row + 1][col - 1] == player &&
          playground[row + 2][col - 2] == player &&
          playground[row + 3][col - 3] == player) {
        return true;
      }
    }
  }

  // Falls kein Spieler gewonnen hat, return false
  return false;
}

bool isGameDraw() {
  // Überprüfen, ob noch leere Felder vorhanden sind
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      if (playground[row][col] == '-') {
        // Es gibt noch mindestens ein leeres Feld, das Spiel ist nicht unentschieden
        return false;
      }
    }
  }

  // Das Spielfeld ist vollständig belegt
  // Überprüfen, ob einer der Spieler gewonnen hat
  return !hasPlayerWon(PLAYER1_CHAR) && !hasPlayerWon(PLAYER2_CHAR);
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

    //Wenn das Spiel beendet ist, dann...
    if (gameOver) {
      //initialisiern der Spielfläche
      initPlayground();
      drawPlayground();
      //Feld wieder auf false setzen
      gameOver = false;
    } else {
      //Wenn das Spiel noch läuft, dann...

      //Variable zum speichern ob ein Pfeil geklickt wurde
      bool playerChooseArrow = false;
      //Variable zum speichern ob ein leeres Feld in der Spalte gefunden wurde
      bool foundEmptyPlace = false;
      //Schleife über alle Pfeile
      for (int arrow = 0; arrow < COLS; arrow++) {
        Arrow arrowField = arrowFields[arrow];
        //prüfen ob der Pfeil an der Position aus dem Array geklickt wurde
        if (checkCoord(arrowField, x, y)) {
          Serial.println(arrow);
          //Variable auf true setzen
          playerChooseArrow = true;
          for (int row = ROWS - 1; row >= 0; row--) {
            //prüfen ob das Feld leer / mit einem Strich belegt ist
            if (playground[row][arrow] == DASH) {
              //Umkehren des Wertes für den Spieler
              currentPlayer1 = !currentPlayer1;
              //bei Spieler 1 soll ein X und bei Spieler 2 ein O an die Stelle im
              //Array gespeichert werden
              playground[row][arrow] = currentPlayer1 ? PLAYER1_CHAR : PLAYER2_CHAR;
              //Es wurde ein leeres Feld gefunden und somit Variable auf true setzen
              foundEmptyPlace = true;
              //Schleife abbrechen
              break;
            }
          }
        }
      }

      //Wenn ein leeres Feld gefunden wurde,
      //und ein Pfeil gewählt wurde, dann...
      if (foundEmptyPlace && playerChooseArrow) {


        //zeichnen des Arrays
        drawHoles();

        //prüfen ob der aktuelle Spieler gewonnen hat
        char player = currentPlayer1 ? PLAYER1_CHAR : PLAYER2_CHAR;
        if (hasPlayerWon(player)) {
          String playerColor = currentPlayer1 ? "Gelb" : "Rot";
          displayMessage("Der Spieler " + playerColor + " hat gewonnen!", 0);
          gameOver = true;
        } else if (isGameDraw()) {
          displayMessage("Das Spiel ist unentschieden!", 40);
          gameOver = true;
        }
      }
      //eine kleine Pause von 300 ms.
      //Damit wird dem Spieler die möglichkeit gegeben
      //den Stift vom Display zu nehmen.
      delay(300);
    }
  }
  //eine Pause von 50ms.
  delay(50);
}

void displayMessage(String msg, int offset) {
  lcd.fillRect(15, 120, 295, 30, TFT_RED);
  lcd.setCursor(20 + offset, 125);
  lcd.setTextSize(0.85);
  lcd.setTextColor(TFT_GREEN, TFT_RED);
  lcd.print(msg);
}

//Funktion zum prüfen ob die geklickte Koordinate innerhalb des Feldes liegt
bool checkCoord(Arrow arrowField, int x, int y) {
  return (x >= arrowField.x && x <= (arrowField.x + triangleWidth) && y >= arrowField.y && y <= (arrowField.y + triangleWidth));
}
