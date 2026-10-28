#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

// Address is usually 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

String incomingMsg = "";
String currentState = "";

// PINY
const uint8_t rGoraPin = 10;
const uint8_t rSrodekPin = 9;
const uint8_t rDolPin = 8;
const uint8_t rZwora = 7;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.setTimeout(50);
  pinMode(rGoraPin, INPUT_PULLUP);
  pinMode(rSrodekPin, INPUT_PULLUP);
  pinMode(rDolPin, INPUT_PULLUP);
  pinMode(rZwora, OUTPUT);
  digitalWrite(rZwora, LOW);
  lcd.init();
  lcd.backlight();
  lcd.print("initializing...");
  Serial.println("init");
}

void loop() {
  Serial.print("a");
  String newState = "-";
  if (digitalRead(rDolPin) == LOW) {
    newState = "D";
    digitalWrite(rZwora, LOW);
  }
  if (digitalRead(rSrodekPin) == LOW) {
    newState = "S";
  }
  if (digitalRead(rGoraPin) == LOW) {
    newState = "G";
  }

  if (newState!=currentState && newState!="-"){
    Serial.println(newState);
    currentState = newState;
  }
   

  if (Serial.available()) {
    delay(30);
    incomingMsg = Serial.readStringUntil('\n');
        
    if (incomingMsg == "O"){
      //Serial.println("OPEN");
      digitalWrite(rZwora, HIGH);
      delay(10);
      digitalWrite(rZwora, HIGH);
      //Serial.println("OPEN-END");
    }
    
    if (incomingMsg == "C"){
      //Serial.println("CLOSE");
      digitalWrite(rZwora, LOW);
      delay(10);
      digitalWrite(rZwora, LOW);
      //Serial.println("CLOSE-END");
    }

    // Pad to 32 characters so old text is overwritten
    while (incomingMsg.length() < 32) incomingMsg += ' ';

    lcd.setCursor(0, 0);
    lcd.print(incomingMsg.substring(0, 16));
    lcd.setCursor(0, 1);
    lcd.print(incomingMsg.substring(16, 32));

  }
  delay(150);
}
