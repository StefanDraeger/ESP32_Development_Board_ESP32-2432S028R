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
  uint8_t backgroundcolor;  //Hintergrundfarbe
  uint8_t textcolor;        //Textfarbe
  int coord_x;
  int coord_y;
  String caption1;          //Text normal
  String caption2;          //Text umgeschaltet
};

struct InputField {
  uint8_t backgroundcolor;
  uint8_t bordercolor;
  uint8_t textcolor;
  int coord_x;
  int coord_y;
  int width;
  int height;
};

struct ButtonFunction {
  uint8_t backgroundcolor;
  uint8_t textcolor;
  int coord_x;
  int coord_y;
  int width;
  int height;
  int roundEdge;
  String caption;
};

ButtonFunction closeBtn = {RED, WHITE, 253, 38, 20, 20, 5, "X"};
ButtonFunction backBtn = {LIGHT_GREY, WHITE, 205, 38, 40, 20, 5, "BACK"};

ButtonFunction shiftBtn = {LIGHT_GREY, WHITE, 38, 158, 47, 20, 5, "SHIFT"};
ButtonFunction symbolsBtn = {LIGHT_GREY, WHITE, 89, 158, 20, 20, 5, "#"};
ButtonFunction spaceBtn = {LIGHT_GREY, WHITE, 113, 158, 133, 20, 5, "Space"};
ButtonFunction okBtn = {GREEN, DARK_GREY, 250, 158, 25, 20, 5, "OK"};

InputField inputField = {WHITE, DARK_GREY, DARK_GREY, 38, 35, 160, 25};

const int BTN_WIDTH = 20;
const int BTN_HEIGHT = 20;
const int BTN_MARGIN_HORIZONTAL = 3;
const int BTN_MARGIN_VERTIKAL = 3;
const int DEFAULT_START_X = 41;
const int DEFAULT_START_Y = 65;

int start_X = DEFAULT_START_X;
int start_Y = DEFAULT_START_Y;

//definieren von zwei Buttons
Button btn1 = { LIGHT_GREY, WHITE, 0, 0, "1", "!" };
Button btn2 = { LIGHT_GREY, WHITE, 0, 0, "2", "\"" };
Button btn3 = { LIGHT_GREY, WHITE, 0, 0, "3", ":" };
Button btn4 = { LIGHT_GREY, WHITE, 0, 0, "4", "$" };
Button btn5 = { LIGHT_GREY, WHITE, 0, 0, "5", "%" };
Button btn6 = { LIGHT_GREY, WHITE, 0, 0, "6", "&" };
Button btn7 = { LIGHT_GREY, WHITE, 0, 0, "7", "/" };
Button btn8 = { LIGHT_GREY, WHITE, 0, 0, "8", "(" };
Button btn9 = { LIGHT_GREY, WHITE, 0, 0, "9", ")" };
Button btn0 = { LIGHT_GREY, WHITE, 0, 0, "0", "=" };

Button btnQ = { LIGHT_GREY, WHITE, 0, 0, "Q", "q" };
Button btnW = { LIGHT_GREY, WHITE, 0, 0, "W", "w" };
Button btnE = { LIGHT_GREY, WHITE, 0, 0, "E", "e" };
Button btnR = { LIGHT_GREY, WHITE, 0, 0, "R", "r" };
Button btnT = { LIGHT_GREY, WHITE, 0, 0, "T", "t" };
Button btnZ = { LIGHT_GREY, WHITE, 0, 0, "Z", "z" };
Button btnU = { LIGHT_GREY, WHITE, 0, 0, "U", "u" };
Button btnI = { LIGHT_GREY, WHITE, 0, 0, "I", "i" };
Button btnO = { LIGHT_GREY, WHITE, 0, 0, "O", "o" };
Button btnP = { LIGHT_GREY, WHITE, 0, 0, "P", "p" };

Button btnA = { LIGHT_GREY, WHITE, 0, 0, "A", "a" };
Button btnS = { LIGHT_GREY, WHITE, 0, 0, "S", "s" };
Button btnD = { LIGHT_GREY, WHITE, 0, 0, "D", "d" };
Button btnF = { LIGHT_GREY, WHITE, 0, 0, "F", "f" };
Button btnG = { LIGHT_GREY, WHITE, 0, 0, "G", "g" };
Button btnH = { LIGHT_GREY, WHITE, 0, 0, "H", "h" };
Button btnJ = { LIGHT_GREY, WHITE, 0, 0, "J", "j" };
Button btnK = { LIGHT_GREY, WHITE, 0, 0, "K", "k" };
Button btnL = { LIGHT_GREY, WHITE, 0, 0, "L", "l" };

Button btnY = { LIGHT_GREY, WHITE, 0, 0, "Y", "y" };
Button btnX = { LIGHT_GREY, WHITE, 0, 0, "X", "x" };
Button btnC = { LIGHT_GREY, WHITE, 0, 0, "C", "c" };
Button btnV = { LIGHT_GREY, WHITE, 0, 0, "V", "v" };
Button btnB = { LIGHT_GREY, WHITE, 0, 0, "B", "b" };
Button btnN = { LIGHT_GREY, WHITE, 0, 0, "N", "n" };
Button btnM = { LIGHT_GREY, WHITE, 0, 0, "M", "m" };

int rowButtonCount[4] = {10, 10, 9, 7};

const int NUM_BUTTONS = 10;
const int NUM_ROWS = 4;
Button keyboard[NUM_ROWS][NUM_BUTTONS] = {
  { btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9, btn0 },
  { btnQ, btnW, btnE, btnR, btnT, btnZ, btnU, btnI, btnO, btnP },
  { btnA, btnS, btnD, btnF, btnG, btnH, btnJ, btnK, btnL, NULL },
  { btnY, btnX, btnC, btnV, btnB, btnN, btnM}
};

Button btnSymbol1 = { LIGHT_GREY, WHITE, 0, 0, "+", "" };
Button btnSymbol2 = { LIGHT_GREY, WHITE, 0, 0, "-", "" };
Button btnSymbol3 = { LIGHT_GREY, WHITE, 0, 0, "/", "" };
Button btnSymbol4 = { LIGHT_GREY, WHITE, 0, 0, "#", "" };
Button btnSymbol5 = { LIGHT_GREY, WHITE, 0, 0, "\'", "" };
Button btnSymbol6 = { LIGHT_GREY, WHITE, 0, 0, "_", "" };
Button btnSymbol7 = { LIGHT_GREY, WHITE, 0, 0, ".", "" };
Button btnSymbol8 = { LIGHT_GREY, WHITE, 0, 0, ":", "" };
Button btnSymbol9 = { LIGHT_GREY, WHITE, 0, 0, ",", "" };
Button btnSymbol10 = { LIGHT_GREY, WHITE, 0, 0, ";", "" };
Button btnSymbol11 = { LIGHT_GREY, WHITE, 0, 0, "<", "" };
Button btnSymbol12 = { LIGHT_GREY, WHITE, 0, 0, ">", "" };
Button btnSymbol13 = { LIGHT_GREY, WHITE, 0, 0, "|", "" };
Button btnSymbol14 = { LIGHT_GREY, WHITE, 0, 0, "?", "" };
Button btnSymbol15 = { LIGHT_GREY, WHITE, 0, 0, "!", "" };
Button btnSymbol16 = { LIGHT_GREY, WHITE, 0, 0, "{", "" };
Button btnSymbol17 = { LIGHT_GREY, WHITE, 0, 0, "}", "" };
Button btnSymbol18 = { LIGHT_GREY, WHITE, 0, 0, "[", "" };
Button btnSymbol19 = { LIGHT_GREY, WHITE, 0, 0, "]", "" };
Button btnSymbol20 = { LIGHT_GREY, WHITE, 0, 0, "~", "" };

const int NUM_BTN_SYMBOL = 5;
const int NUM_BTN_SYMBOL_ROWS = 4;

Button symbols[NUM_BTN_SYMBOL_ROWS][NUM_BTN_SYMBOL] = {
  {btnSymbol1, btnSymbol2, btnSymbol3, btnSymbol4, btnSymbol5},
  {btnSymbol6, btnSymbol7, btnSymbol8, btnSymbol9, btnSymbol10},
  {btnSymbol11, btnSymbol12, btnSymbol13, btnSymbol14, btnSymbol15},
  {btnSymbol16, btnSymbol17, btnSymbol18, btnSymbol19, btnSymbol20}
};

bool isShiftActive = false;
bool isSymbolActive = false;

String inputText = "";

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
    handleKeyboard(x, y);
  }
  //eine Pause von 50 Millisekunden.
  delay(50);
}

void showKeyboard() {
  start_X = DEFAULT_START_X;
  start_Y = DEFAULT_START_Y;

  lcd.fillScreen(BK_GREY);

  lcd.drawRoundRect(30, 30, 250, 155, 10, DARK_GREY);

  drawFunctionButton(closeBtn);

  drawFunctionButton(backBtn);
  if (!isSymbolActive) {
    drawFunctionButton(shiftBtn);
  }
  drawFunctionButton(symbolsBtn);
  drawFunctionButton(spaceBtn);
  drawFunctionButton(okBtn);

  drawInputField(inputField);

  if (!isSymbolActive) {
    for (int row = 0; row < NUM_ROWS; row++) {
      start_X = DEFAULT_START_X;
      if (row > 0) {
        start_Y = DEFAULT_START_Y + (row * (BTN_HEIGHT + BTN_MARGIN_VERTIKAL));
      }

      for (int btnIndex = 0; btnIndex < rowButtonCount[row]; btnIndex++) {
        int offset_X = 0;
        if (rowButtonCount[row] < 10 && btnIndex == 0) {
          switch (rowButtonCount[row]) {
            case 9: offset_X = BTN_WIDTH / 2; break;
            case 7: offset_X = BTN_WIDTH * 1.5 + BTN_MARGIN_HORIZONTAL; break;
          }
        }

        drawButton(row, btnIndex, keyboard[row][btnIndex], offset_X);
      }
    }
  } else {
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

void handleKeyboard(int x, int y) {
  bool redrawKeyboard = false;
  bool redrawInputField = false;
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

  if (checkCoordButton(closeBtn, x, y)) {

  } else if (checkCoordButton(shiftBtn, x, y)) {
    isShiftActive = !isShiftActive;
    isSymbolActive = false;
    redrawKeyboard = true;
  } else if (checkCoordButton(symbolsBtn, x, y)) {
    isSymbolActive = !isSymbolActive;
    isShiftActive = false;
    redrawKeyboard = true;
  } else if (checkCoordButton(spaceBtn, x, y)) {
    addSignToInputField(" ");
  } else if (checkCoordButton(okBtn, x, y)) {
  } else if (checkCoordButton(backBtn, x, y)) {
    if (inputText.length() > 0) {
      inputText = inputText.substring(0, inputText.length() - 1);
      redrawInputField = true;
    }
  }

  if (redrawKeyboard) {
    showKeyboard();
  } else if (redrawInputField) {
    drawInputField(inputField);
  }
}

void drawFunctionButton(ButtonFunction &button) {
  lcd.setFont(&fonts::Font2);
  if ((button.caption == "SHIFT" && isShiftActive) || (button.caption == "#" && isSymbolActive)) {
    lcd.fillRoundRect(button.coord_x, button.coord_y, button.width, button.height, button.roundEdge, DARKER_GREY);
    lcd.setTextColor(button.textcolor, DARKER_GREY);
  } else {
    lcd.fillRoundRect(button.coord_x, button.coord_y, button.width, button.height, button.roundEdge, button.backgroundcolor);
    lcd.setTextColor(button.textcolor, button.backgroundcolor);
  }

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
  if (btnIndex > 0) {
    x = start_X + (BTN_WIDTH + BTN_MARGIN_HORIZONTAL) + offset_X;
  }

  int y = start_Y;

  lcd.fillRoundRect(x, y, BTN_WIDTH, BTN_HEIGHT, 2, button.backgroundcolor);

  lcd.setTextColor(button.textcolor, button.backgroundcolor);
  lcd.setCursor(x + 7, y + 2);

  lcd.print(isShiftActive ? button.caption2 : button.caption1);

  button.coord_x = x;
  button.coord_y = y;

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
