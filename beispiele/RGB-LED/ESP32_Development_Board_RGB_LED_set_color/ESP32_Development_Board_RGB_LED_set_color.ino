//RGB LED
//LED, Farbe rot
#define ledRed 17
//LED, Farbe grün
#define ledGreen 4
//LED, Farbe blau
#define ledBlue 16

void setup() {
  //definieren das die Pins als Ausgang dienen
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  
  //setzen der Farbe lila an der RGB LED
  //Farbe rot mit 178 von max. 255
  analogWrite(ledRed, 178); 
  //Farbe grün mit 102 von max. 255
  analogWrite(ledGreen, 102);
  //Farbe blau mit dem maximalen Wert von 255
  analogWrite(ledBlue, 255);
}

void loop() { }