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
}

void loop() {
  //Wenn ein Touchaktion ausgeführt / erkannt wurde,
  //dann liefert die Funktion getTouch den Wert 1 und
  //befüllt die übergebenen Parametern mit den Koordinaten
  if (lcd.getTouch(&x, &y) == 1) {

    //befüllen des Displays mit der Farbe schwarz
    lcd.fillScreen(TFT_BLACK);

    //Wenn die ermittelte X oder Y Positon außerhalb der Range ist,
    //dann soll die Funktion hier verlassen werden.
    if (x > MAX_X || y > MAX_Y) {
      return;
    }

    //ein Kreuz an der X & Y Koordinate Zeichnen
    lcd.drawLine(x - 20, y, x + 20, y, 0xFFFF00U);
    lcd.drawLine(x, y - 20, x, y + 20, 0xFFFF00U);

    //setzen der Schriftart & Schriftgröße
    lcd.setFont(&fonts::Font2);
    //setzen des Cursors abhängig von der X/Y Koordinate
    setCursorPosition(0);
    //schreiben der X Position
    lcd.printf("x:%d", x);
    //setzen des Cursors abhängig von der X/Y Koordinate
    //mit einem Offset von 20 Pixel
    setCursorPosition(20);
    //schreiben der X Position
    lcd.printf("y:%d", y);
    //Zusätzlich werden die ermittelten Koordinaten auf der
    //Konsole ausgegeben.
    Serial.printf("x:%d,y:%d", x, y);
  }
  //eine Pause von 50ms.
  delay(50);
}

void setCursorPosition(int offset) {
  if (x < 280 && y < 200) {
    lcd.setCursor(x + 5, y + offset);
  } else {
    if (x > 280 && y > 200) {  //|| y > 200
      lcd.setCursor(x - 35, y - 40 + offset);
    } else if ((x < 20 && y > 200) || (x > 20 && y > 200)) {
      lcd.setCursor(x + 5, y - 40 + offset);
    } else if (x > 280) {
      lcd.setCursor(x - 35, y + offset);
    }
  }
}