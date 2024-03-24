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

//Eigenschaften eines Buttons definieren
struct Button {
  bool isPressed; //ist dieser gedrückt?
  int coord_x; //X Koordinate
  int coord_y; //Y Koordinate
  int width; //Breite
  int height; //Höhe
  uint8_t backgroundcolor; //Hintergrundfarbe
  uint8_t textcolor; //Textfarbe
  String caption; //Text auf dem Button
};

//definieren von zwei Buttons
Button btn1 = { false, 20, 20, 50, 25, lcd.color332(255, 0, 0), lcd.color332(255, 255, 255), "Test" };
Button btn2 = { false, 20, 60, 100, 25, lcd.color332(0, 255, 0), lcd.color332(0, 0, 0), "Hallo Welt!" };

//die beiden Buttons in ein Array aufnehmen
//das macht es später einfacher diese abzuarbeiten
const int NUM_BUTTONS = 2;
Button buttons[2] = { btn1, btn2 };

//Offset für den Schatten des Buttons
const int SHADOW_OFFSET = 2;

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
  //zeichnen der Oberfläche
  drawBackground();
}

//Zeichnen der Oberfläche
//In dieser Funktion können noch weitere
//GUI Elemente generiert werden.
void drawBackground() {
  //Hintergrundfarbe in Schwarz
  lcd.fillScreen(TFT_BLACK);
  //Schleife über das Array mit 
  //den zuvor erstellten Buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    //zeichnen des Buttons
    drawButton(buttons[i]);
  }
}

//Zeichnen eines Buttons
void drawButton(Button button) {
  //Wenn der Button gedrückt ist, 
  //dann soll der Schatten nach oben links zeigen
  if (button.isPressed) {
    lcd.fillRoundRect(button.coord_x - SHADOW_OFFSET, button.coord_y - SHADOW_OFFSET, button.width, button.height, 10, lcd.color332(176, 190, 197));
  } else {
    //Wenn der Button nicht gedrückt ist,
    //dann soll der Schatten nach unten rechts zeigen
    lcd.fillRoundRect(button.coord_x + SHADOW_OFFSET, button.coord_y + SHADOW_OFFSET, button.width, button.height, 10, lcd.color332(176, 190, 197));
  }

  //Zeichnen eines Rechtecks mit abgerundeten Ecken mit den Eigenschaften des Buttons
  lcd.fillRoundRect(button.coord_x, button.coord_y, button.width, button.height, 10, button.backgroundcolor);

  //Berechnen der Position für den Text.
  int textLenght = (button.caption.length() * 3);
  int newX = (button.width / 2) - (textLenght / 2) + 5;
  //setzen der Textfarbe und der Hintergrundfarbe
  lcd.setTextColor(button.textcolor, button.backgroundcolor);
  //positionieren des Cursors
  lcd.setCursor(button.coord_x + (newX / 2), button.coord_y + 8);
  //schreiben des Textes an die gesetzte Position
  lcd.print(button.caption);
}

//entfernen eines Buttons
void removeButton(Button button) {
  //ein einfaches schwarzes Rechteck an die Position des Buttons zeichnen
  lcd.fillRect(button.coord_x - 5, button.coord_y - 5, button.width + 10, button.height + 10, lcd.color332(0, 0, 0));
}

//Prüfen ob die angeklickte Position auf dem Display innerhalb der Abgrenzung
//des Buttons ist.
bool checkCoordButton(Button button, int x, int y) {
  return (x >= button.coord_x && x <= (button.coord_x + button.width) && y >= button.coord_y && y <= (button.coord_y + button.height));
}

//Einen angeklickten Button togglen.
//D.h. es wird kurz der schatten nach oben links
//verschoben und danach wieder nach unten rechts.
void toggleButton(Button button) {
  //Button entfernen
  removeButton(button);
  //setzen das der Button gedrückt ist
  button.isPressed = true;
  //zeichnen des Buttons
  drawButton(button);
  //eine kleine Pause von 85 Millisekunden
  delay(85);
  //Button entfernen
  removeButton(button);
  //setzen das der Button nicht gedrückt ist
  button.isPressed = false;
  //zeichnen des Buttons
  drawButton(button);
}

void loop() {
  //Wenn auf das Display geklickt wird, dann
  //liefert die Funktion getTouch eine 1 ansonsten 0
  //zusätzlich werden die Koordinaten in die Felder
  //x & y gespeichert.
  if (lcd.getTouch(&x, &y) == 1) {

    //Wenn die Koordinaten außerhalb der Range sind,
    //dann soll die Funktion hier verlassen werden.
    if (x > MAX_X || y > MAX_Y) {
      return;
    }

    //Schleife über das Array mit den Buttons
    for (int i = 0; i < NUM_BUTTONS; i++) {
      //Button aus dem Array mit dem Wert aus
      //der Laufvariable i lesen.
      Button button = buttons[i];
      //Wenn die geklickte Position auf dem
      //Display innerhalb der Abgrenzung eines
      //Buttons ist, dann...
      if (checkCoordButton(button, x, y)) {
        //Togglen des Buttons
        toggleButton(button);
      }
    }

  }
  //eine Pause von 50 Millisekunden.
  delay(50);
}
