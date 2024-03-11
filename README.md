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
