//Bibliothek zum ansteuern des TFT Displays
#include <TFT_eSPI.h>
#include <SPI.h>
//Datei mit den definierten Schriftarten
#include "Free_Fonts.h"

//eine instanz vom TFT Display Objekt erstellen
TFT_eSPI tft = TFT_eSPI();

void setup(void) {
  //beginn der kommunikation mit dem TFT Display
  tft.init();
  //drehen des Displays
  tft.setRotation(3);
  //Hintergrundfarbe auf Schwarz setzen
  tft.fillScreen(TFT_BLACK);

  //Textfarbe Magenta mit Hintergrundfarbe Schwarz
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  //Schriftart FSB9 setzen
  tft.setFreeFont(FSB9);
  //den Cursor an die Position x=10 & y=45 setzen
  tft.setCursor(10, 45);
  //den Text "Hallo Welt!" an die gesetzte Cursorposition
  //schreiben
  tft.print("Hallo Welt!");

  //Weitere Texte mit unterschiedlichen Farben, Positionen und
  //Schriftgrößen auf dem TFT-Display anzeigen
  tft.setTextColor(TFT_VIOLET, TFT_BLACK);
  tft.setFreeFont(FSB12);
  tft.setCursor(10, 65);
  tft.print("Hallo Welt!");

  tft.setTextColor(TFT_PINK, TFT_BLACK);
  tft.setFreeFont(FSB18);
  tft.setCursor(10, 95);
  tft.print("Hallo Welt!");

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(FSB24);
  tft.setCursor(10, 130);
  tft.print("Hallo Welt!");
}

void loop() {
}