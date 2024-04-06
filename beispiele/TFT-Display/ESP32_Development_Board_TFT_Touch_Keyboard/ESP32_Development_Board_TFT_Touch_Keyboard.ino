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

uint8_t WHITE = lcd.color332(255, 255, 255);
uint8_t DARK_GREY = lcd.color332(103, 108, 112);
uint8_t DARKER_GREY = lcd.color332(136, 136, 136);
uint8_t LIGHT_GREY = lcd.color332(221, 221, 221);
uint8_t BLUE = lcd.color332(145, 176, 201);
uint8_t BK_GREY = lcd.color332(227, 230, 232);
uint8_t RED = lcd.color332(238, 65, 65);
uint8_t GREEN = lcd.color332(86, 217, 86);

struct Button {
  int coord_x;              //X Koordinate der Schaltfläche
  int coord_y;              //Y Koordinate der Schaltfläche
  String caption1;          //Text normal
  String caption2;          //Text umgeschaltet
};

struct InputField {
  uint8_t backgroundcolor; //Hintergrundfarbe
  uint8_t bordercolor;     //Rahmenfarbe
  uint8_t textcolor;       //Textfarbe
  int coord_x;             //X Koordinate
  int coord_y;             //Y Koordinate
  int width;               //Breite
  int height;              //Höhe
};

struct ButtonFunction {
  uint8_t backgroundcolor; //Hintergrundfarbe
  uint8_t textcolor;       //Textfarbe
  int coord_x;             //X Koordinate der Schaltfläche
  int coord_y;             //Y Koordinate der Schaltfläche
  int width;               //Breite
  int height;              //Höhe
  String caption;          //Text    
};

ButtonFunction closeBtn = {RED, WHITE, 253, 38, 20, 20, "X"};
ButtonFunction backBtn = {LIGHT_GREY, WHITE, 205, 38, 40, 20, "BACK"};

ButtonFunction shiftBtn = {LIGHT_GREY, WHITE, 38, 158, 47, 20, "SHIFT"};
ButtonFunction symbolsBtn = {LIGHT_GREY, WHITE, 89, 158, 20, 20, "#"};
ButtonFunction spaceBtn = {LIGHT_GREY, WHITE, 113, 158, 133, 20, "Space"};
ButtonFunction okBtn = {GREEN, DARK_GREY, 250, 158, 25, 20, "OK"};

InputField inputField = {WHITE, DARK_GREY, DARK_GREY, 38, 35, 160, 25};

const int BTN_ROUNDEDGE = 2;
const int BTN_WIDTH = 20;
const int BTN_HEIGHT = 20;
const int BTN_MARGIN_HORIZONTAL = 3;
const int BTN_MARGIN_VERTIKAL = 3;
const int DEFAULT_START_X = 41;
const int DEFAULT_START_Y = 65;

int start_X = DEFAULT_START_X;
int start_Y = DEFAULT_START_Y;

//Wieviele Schaltflächen sollen pro Zeile angezeigt werden?
int rowButtonCount[4] = {10, 10, 9, 7};

//Maximale Anzahl der Schaltflächen pro Zeile
const int NUM_BUTTONS = 10;
//Anzahl der Zeilen
const int NUM_ROWS = 4;
Button keyboard[NUM_ROWS][NUM_BUTTONS] = {
  { { 0, 0, "1", "!" }, { 0, 0, "2", "\"" }, { 0, 0, "3", ":" }, { 0, 0, "4", "$" }, { 0, 0, "5", "%" }, { 0, 0, "6", "&" }, { 0, 0, "7", "/" }, { 0, 0, "8", "(" }, { 0, 0, "9", ")" }, { 0, 0, "0", "=" } },
  { { 0, 0, "Q", "q" }, { 0, 0, "W", "w" }, { 0, 0, "E", "e" }, { 0, 0, "R", "r" }, { 0, 0, "T", "t" }, { 0, 0, "Z", "z" }, { 0, 0, "U", "u" }, { 0, 0, "I", "i" }, { 0, 0, "O", "o" }, { 0, 0, "P", "p" } },
  { { 0, 0, "A", "a" }, { 0, 0, "S", "s" }, { 0, 0, "D", "d" }, { 0, 0, "F", "f" }, { 0, 0, "G", "g" }, { 0, 0, "H", "h" }, { 0, 0, "J", "j" }, { 0, 0, "K", "k" }, { 0, 0, "L", "l" } },
  { { 0, 0, "Y", "y" }, { 0, 0, "X", "x" }, { 0, 0, "C", "c" }, { 0, 0, "V", "v" }, { 0, 0, "B", "b" }, { 0, 0, "N", "n" }, { 0, 0, "M", "m" }}
};

//Anzahl der Schaltflächen pro Zeile
const int NUM_BTN_SYMBOL = 5;
//Anzahl der Zeilen
const int NUM_BTN_SYMBOL_ROWS = 4;

Button symbols[NUM_BTN_SYMBOL_ROWS][NUM_BTN_SYMBOL] = {
  {{ 0, 0, "+", "" }, { 0, 0, "-", "" }, { 0, 0, "/", "" }, { 0, 0, "#", "" }, { 0, 0, "\'", "" }},
  {{ 0, 0, "_", "" }, { 0, 0, ".", "" }, { 0, 0, ":", "" }, { 0, 0, ",", "" }, { 0, 0, ";", "" }},
  {{ 0, 0, "<", "" }, { 0, 0, ">", "" }, { 0, 0, "|", "" }, { 0, 0, "?", "" }, { 0, 0, "!", "" }},
  {{ 0, 0, "{", "" }, { 0, 0, "}", "" }, { 0, 0, "[", "" }, { 0, 0, "]", "" }, { 0, 0, "~", "" }}
};

bool isShiftActive = false;
bool isSymbolActive = false;

String inputText = "";

bool keyboardIsVisible = false;

void setup(void) {
  //beginn der seriellen Kommunikation mit
  //115200 Baud
  Serial.begin(115200);

  //beginn der Kommunikation mit dem Display
  lcd.init();
  //drehen des Displays
  lcd.setRotation(1);
  //füllen des Displays mit der Farbe Schwarz
  lcd.fillScreen(BK_GREY);
  //setzen der Schriftart
  lcd.setFont(&fonts::Font2);
  //eine kleine Pause von 100ms.
  delay(100);
  showKeyboard();
}

const long CLICK_DELAY = 350;
long lastClick = 0;

void loop() {
  long currentMillis = millis();
  if (lcd.getTouch(&x, &y) == 1 && (lastClick + CLICK_DELAY < currentMillis)) {
    lastClick = currentMillis;

    if (x > MAX_X || y > MAX_Y) {
      return;
    }

    //Nur wenn die Bildschirmtastatur sichtbar ist, soll diese behandelt werden.
    if(keyboardIsVisible){
      //Aufrufen der Funktion mit den geklickten Koordinaten.
      handleKeyboard(x, y);
    }
    
  }
  //eine Pause von 50 Millisekunden.
  delay(50);
}

//Zeigt die Bildschirmtastatur an, wird ebenso verwendet um 
//die Bildschirmtastatur neu zu zeichnen.
void showKeyboard() {
  //Die Bildschirmtastatur ist sichtbar!
  keyboardIsVisible = true;
  //Koordinaten der Bildschirmtastatur
  start_X = DEFAULT_START_X;
  start_Y = DEFAULT_START_Y;

  //Hintergrundfarbe
  lcd.fillScreen(BK_GREY);
  //Rahmen zeichnen
  lcd.drawRoundRect(30, 30, 250, 155, 10, DARK_GREY);
  
  //Zeichnen der Funktionstasten
  drawFunctionButton(closeBtn);
  drawFunctionButton(backBtn);

  //Wenn die Symbol-Taste aktiviert ist, dann soll
  //die Shift-Taste nicht gezeichnet werden
  if (!isSymbolActive) {
    drawFunctionButton(shiftBtn);
  }

  drawFunctionButton(symbolsBtn);
  drawFunctionButton(spaceBtn);
  drawFunctionButton(okBtn);

  //Zeichnen des Eingabefeldes
  drawInputField(inputField);

  //Wenn nicht die Symbol-Taste aktiviert ist,
  //dann soll die normale Tastatur gezeichnet werden.
  if (!isSymbolActive) {
    //Schleifen über die Arrays
    for (int row = 0; row < NUM_ROWS; row++) {
      start_X = DEFAULT_START_X;
      //Berechnen der X Koordinate für Zeilen größer als 1
      if (row > 0) {
        start_Y = DEFAULT_START_Y + (row * (BTN_HEIGHT + BTN_MARGIN_VERTIKAL));
      }
      //Schleife über die Schaltflächen im Array, zusätzlich wird hier aus dem Array
      //rowButtonCount die maximale Anzahl entnommen
      for (int btnIndex = 0; btnIndex < rowButtonCount[row]; btnIndex++) {
        int offset_X = 0;
        //Berechnen eines Offsets 
        if (rowButtonCount[row] < 10 && btnIndex == 0) {
          //Abhängig von der Anzahl der Schaltflächen soll die Zeile eingerückt werden.
          switch (rowButtonCount[row]) {
            case 9: offset_X = BTN_WIDTH / 2; break;
            case 7: offset_X = BTN_WIDTH * 1.5 + BTN_MARGIN_HORIZONTAL; break;
          }
        }
        //Zeichnen der Bildschirmtastatur
        drawButton(row, btnIndex, keyboard[row][btnIndex], offset_X);
      }
    }
  } else { //Wenn die Symbol-Taste aktiviert wurde, dann sollen die Sonderzeichen gezeichnet werden
    //Die Sonderzeichen sind als Block von 5x4 abgelegt.
    for (int row = 0; row < NUM_BTN_SYMBOL_ROWS; row++) {
      start_X = DEFAULT_START_X;
      if (row > 0) {
        start_Y = DEFAULT_START_Y + (row * (BTN_HEIGHT + BTN_MARGIN_VERTIKAL));
      }
      for (int btnIndex = 0; btnIndex < NUM_BTN_SYMBOL; btnIndex++) {
        int offset_X = 0;
        if (btnIndex == 0) {
          offset_X = 60;
        }
        drawButton(row, btnIndex, symbols[row][btnIndex], offset_X);
      }
    }
  }
}

//Aktionen an der Bildschirmtastatur behandeln
//Als Parameter werden die geklickten Koordinaten am Display erwartet.
void handleKeyboard(int x, int y) {
  //Felder ob die komplette Bildschirmtastatur neugezeichnet werden soll, oder
  //nur das Eingabefeld.
  bool redrawKeyboard = false;
  bool redrawInputField = false;
  //prüfen ob die Symbold-Taste aktiviert ist.
  //Hintergrund: Es wird geprüft ob sich an einer X/Y Koordinate eine Schaltfläche
  //befindet. Da die Bildschirmtastatur jedoch zweischichtig ist, kommt es hier zu
  //Überlagerungen von Schaltflächen.
  if (!isSymbolActive) {
    for (int row = 0; row < NUM_ROWS; row++) {
      for (int btnIndex = 0; btnIndex < NUM_BUTTONS; btnIndex++) {
        Button button = keyboard[row][btnIndex];
        if (checkCoordButton(button, x, y)) {
          redrawInputField = true;
          addSignToInputField(isShiftActive ? button.caption2 : button.caption1);
        }
      }
    }
  } else {
    for (int row = 0; row < NUM_BTN_SYMBOL_ROWS; row++) {
      for (int btnIndex = 0; btnIndex < NUM_BTN_SYMBOL; btnIndex++) {
        Button button = symbols[row][btnIndex];
        if (checkCoordButton(button, x, y)) {
          addSignToInputField(button.caption1);
          redrawInputField = true;
        }
      }
    }
  }

  //Prüfen der Funktionstasten
  if (checkCoordButton(closeBtn, x, y)) {
    //Hier wird die Schaltfläche X behandelt.
  } else if (checkCoordButton(shiftBtn, x, y)) {
    //Wenn die Shift-Taste betätigt wurde dann soll der
    //boolsche Wert umgekert werden
    isShiftActive = !isShiftActive;
    //Auf jedenfall soll dann auch die Symboltaste als deaktiviert gelten
    isSymbolActive = false;
    //neuzeichnen der Bildschirmtastatur, entweder werden nun 
    //die kleinen Buchstaben oder die großen angezeigt.
    redrawKeyboard = true;
  } else if (checkCoordButton(symbolsBtn, x, y)) {
    isSymbolActive = !isSymbolActive;
    isShiftActive = false;
    redrawKeyboard = true;
  } else if (checkCoordButton(spaceBtn, x, y)) {
    //Ein Leerzeichen dem Eingabefeld hinzufügen.
    addSignToInputField(" ");
  } else if (checkCoordButton(okBtn, x, y)) {
    //Hier wird die Schaltfläche OK behandelt.
  } else if (checkCoordButton(backBtn, x, y)) {
    //Taste zum löschen des letzten Zeichens im Eingabefeld
    //Wird jedoch nur ausgehührt wenn die Länge des Feldes größer 0 ist.
    if (inputText.length() > 0) {
      inputText = inputText.substring(0, inputText.length() - 1);
      //nur das Eingabefeld neuzeichnen
      redrawInputField = true;
    }
  }

  if (redrawKeyboard) {
    showKeyboard();
  } else if (redrawInputField) {
    drawInputField(inputField);
  }
}

//Zeichnen einer Funktionstaste
void drawFunctionButton(ButtonFunction &button) {
  //Wenn die Shift-Taste oder die Symbol-Taste betätigt wurde, dann soll die jeweilige
  //Taste mit dunkelgrauem hintergrund dargestellt werden.
  if ((button.caption == "SHIFT" && isShiftActive) || (button.caption == "#" && isSymbolActive)) {
    lcd.fillRoundRect(button.coord_x, button.coord_y, button.width, button.height, BTN_ROUNDEDGE, DARKER_GREY);
    lcd.setTextColor(button.textcolor, DARKER_GREY);
  } else {
    lcd.fillRoundRect(button.coord_x, button.coord_y, button.width, button.height, BTN_ROUNDEDGE, button.backgroundcolor);
    lcd.setTextColor(button.textcolor, button.backgroundcolor);
  }

  //Die Leertaste ist deutlich länger und damit der Text zentriert angezeigt wird, muss hier eine If-Bedingung erfolgen.
  if (button.caption == "Space") {
    lcd.setCursor(button.coord_x + 50, button.coord_y + 3);
  } else {
    lcd.setCursor(button.coord_x + 6, button.coord_y + 3);
  }

  lcd.print(button.caption);
}

void drawInputField(InputField inputField) {
  lcd.drawRect(inputField.coord_x, inputField.coord_y, inputField.width, inputField.height, inputField.bordercolor);
  lcd.fillRect(inputField.coord_x + 1, inputField.coord_y + 1, inputField.width - 2, inputField.height - 2, BK_GREY);
  lcd.setTextColor(inputField.textcolor, inputField.backgroundcolor);
  lcd.setCursor(inputField.coord_x + 4, inputField.coord_y + 5);

  int maxTextLength = 15;
  int textLength = inputText.length();
  if (textLength > maxTextLength) {
    lcd.print(inputText.substring(textLength - maxTextLength, textLength));
  } else {
    lcd.print(inputText);
  }
}

//Zeichnen eines Buttons
void drawButton(int row, int btnIndex, Button &button, int offset_X) {
  
  int x = start_X + offset_X;
  //Für den ersten Button die Berechnung der neuen X Koordinate überspringen
  if (btnIndex > 0) {
    x = start_X + (BTN_WIDTH + BTN_MARGIN_HORIZONTAL) + offset_X;
  }

  // Y Koordinate vom durchlauf zuvor verwenden
  int y = start_Y;

  lcd.fillRoundRect(x, y, BTN_WIDTH, BTN_HEIGHT, 2, LIGHT_GREY);

  lcd.setTextColor(WHITE, LIGHT_GREY);
  lcd.setCursor(x + 7, y + 2);

  lcd.print(isShiftActive ? button.caption2 : button.caption1);

  //Speichern der Koordinaten am Button
  button.coord_x = x;
  button.coord_y = y;
  //Speichern der Koordinaten an den Feldern für den nächsten durchlauf
  start_X = x;
  start_Y = y;
}

bool checkCoordButton(Button button, int x, int y) {
  return checkCoordButton(button.coord_x, button.coord_y, BTN_WIDTH, BTN_HEIGHT, x, y);
}

bool checkCoordButton(ButtonFunction button, int x, int y) {
  return checkCoordButton(button.coord_x, button.coord_y, button.width, button.height, x, y);
}

bool checkCoordButton(int button_x, int button_y, int button_width, int button_height, int x, int y) {
  return (x >= button_x && x <= (button_x + button_width) && y >= button_y && y <= (button_y + button_height));
}

void addSignToInputField(String sign) {
  inputText = inputText + sign;
}
