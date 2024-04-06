## ESP32 Development Board - ESP32-2432S028R
Auf dieser Seite findest du Beispiele zum Mikrocontroller ESP32-2432S028R.
Einen ausführlichen Artikel zu diesem Mikrocontroller findest du auf meinem Blog unter [ESP32 Development Board mit 2,8 Zoll Touch Display: Programmieren für Anfänger](https://draeger-it.blog/esp32-development-board-mit-2-8-zoll-touch-display-programmieren-fuer-anfaenger/)
### Aufbau des Mikrocontrollers ESP32-2432S028R
![TFT Display des ESP32-2432S028R](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/ESP32_2.8Inch_Touch_1.png)
![Rückseite ESP32-2432S028R](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/ESP32_2.8Inch_Touch_2.png)
### Pinbelegung
![Aufbau des ESP32-2432S028R](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/Aufbau_ESP32_Developmentboard_Touch_Display.png)
![Pinbelegung des ESP32-2432S028R](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/ESP32_Development_Board_Pins.png)
### Beispiele
Fangen wir mit ein paar Beispielen an. (Von einfach bis komplex.) 
#### RGB LED
Auf der Rückseite findest du eine RGB LED welche wir in der Infografik zu sehen über die nachfolgenden Pins angeschlossen sind.

 - rot > IO17
 - grün > IO4
 - blau > IO16
#### setzen einer Farbe
[Quellcode für das einfache setzen einer Farbe an der RGB LED](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/tree/main/beispiele/RGB-LED/ESP32_Development_Board_RGB_LED_set_color)

![RGB LED in der Farbe lila am ESP32 Development Board](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/ESP32_Development_Board_RGB_LED_violet.png)
#### Anzeigen von "Hallo Welt!" auf dem TFT Display
[Quellcode für das schreiben des Textes "Hallo Welt!" auf dem TFT-Display](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/tree/main/beispiele/TFT-Display/ESP32_Development_Board_HalloWelt_simple)

![enter image description here](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/Text_HalloWelt_TFT-Display.png)
#### Anzeigen von einem Bild auf dem TFT Display
Damit du ein Bild auf dem TFT Display anzeigen kannst, muss dieses auf den Mikrocontroller hochgeladen werden. Wie du dieses machst habe ich dir unter [Beispiel – Bild auf dem TFT-Display anzeigen](https://draeger-it.blog/esp32-development-board-mit-2-8-zoll-touch-display-programmieren-fuer-anfaenger#beispiel-bild-auf-dem-tftdisplay-anzeigen) ausführlich erläutert.
Du benötigst zum Anzeigen dann das kleine Programm "LittleFS_PNG" aus den Beispielen.

![Logo auf dem TFT-Display des ESP32 Development Boards](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/ESP32_Development_Board_Logo.jpg)

#### Erkennen einer Touchaktion am TFT Display

[Quellcode für das erkennen einer Touchaktion am Display](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/tree/main/beispiele/TFT-Display/ESP32_Development_Board_TFT_Touch_simple)

![erkannte X&Y Koordinate am Touchdisplay](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/Touchaktion_ESP32_Development_Board.png)

#### Zeichnen von einfachen Buttons auf dem Display mit Touchaktion

[Quellcode für das zeichnen von Buttons auf dem Display](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/tree/main/beispiele/TFT-Display/ESP32_Development_Board_TFT_Touch_Buttons)

![einfache Buttons auf dem Display](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/ESP32_Development_Board_Buttons.jpg)

#### aktivieren / deaktivieren von LEDs mit Buttons auf dem Touchdisplay

[Quellcode für das zeichnen von Buttons und dem steuern von LEDs](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/tree/main/beispiele/TFT-Display/ESP32_Development_Board_TFT_Touch_Buttons_LEDs)

![einfache Buttons auf dem Display](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/ESP32_Development_Board_Buttons_control_LED.jpg)

#### Bildschirmtastatur

[Quellcode für das darstellen einer Bildschirmtastatur auf dem ESP32-2432S028R](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/tree/main/beispiele/TFT-Display/ESP32_Development_Board_TFT_Touch_Keyboard)

![kleine Bildschirmtastatur auf dem ESP32-2432S028R](https://github.com/StefanDraeger/ESP32_Development_Board_ESP32-2432S028R/blob/main/bilder/ESP32-2432S028R_onscreen_keyboard.png)
