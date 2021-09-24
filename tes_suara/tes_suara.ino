/******************************************************************************
  Simple SerialMP3Player "Hello World" example of YX5300 chip.

  Copy the "hello.mp3" file to an empty SD card
  Connect the Serial MP3 Player to the Arduino board
    GND → GND
    VCC → 5V
    TX → pin 11
    RX → pin 10

  After compile and upload the code you must hear “Hello world” over and over.


  This example code is in the public domain.

  https://github.com/salvadorrueda/ArduinoSerialMP3Player

  by Salvador Rueda
 *******************************************************************************/
#include <LiquidCrystal_I2C.h>
#include "SerialMP3Player.h"

#define TX 16
#define RX 17

SerialMP3Player mp3(RX, TX);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  Serial.begin(9600);     // start serial interface
  mp3.begin(9600);        // start mp3-communication
  delay(500);             // wait for init

  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(500);             // wait for init
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(5000);

  
}

// the loop function runs over and over again forever
void loop() {
  //delay(5000);
  for (int i = 1; i <= 5; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (i == 1) {
      mp3.play(i);
      lcd.print("Selamat Datang");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Saldo: ");
      lcd.setCursor(0, 1);
      lcd.print("50000");
//    delay(3000);
    } else {
      mp3.play(i);
      lcd.print("Anda Memilih: "); lcd.print(i-1);
    }
    
    delay(6000);
  }
}
