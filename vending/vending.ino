#include "pinku.h"
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include "SerialMP3Player.h"

#define TX 16
#define RX 17
#include <LiquidCrystal_I2C.h>

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int maxMember = 2;
struct member {
  String id;
  String user;
  long saldo;
};

member card[maxMember];

void setup() {

  card[0].id = "73 85 0F 19";
  card[0].user = "MANGURI";
  card[0].saldo = 25000;
  
  card[1].id = "1C 1F 5D 49";
  card[1].user = "NAJWA";
  card[1].saldo = 50000;

  Serial.begin(115200);

  pinMode(relayUV,OUTPUT);
  digitalWrite(relayUV,HIGH);
  
  Serial.println(card[1].saldo);
  while (!Serial);
  SPI.begin();
  rfid.PCD_Init();
  delay(10);
//  rfid.PCD_SetAntennaGain(rfid.RxGain_max);
  
  rfid.PCD_DumpVersionToSerial();

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("VendingBOT Start");
  delay(1000);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("TEMPELKAN");
  lcd.setCursor(2, 1);
  lcd.print("KARTU  ANDA");
}

void loop() {
  lampuON();
  delay(5000);
  lampuOFF();
  delay(5000);

  //Cek Kartu
  /*if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  //Read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  String content = "";

  //extraksi alamat
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  rfid.PICC_HaltA();
  content.toUpperCase();
  Serial.println(content);
  
  int index = cariKartu(content);
  if (index >= 0) showCard(index); 
*/
}
void lampuON(){
  digitalWrite(relayUV,LOW);
}
void lampuOFF(){
  digitalWrite(relayUV,HIGH);
}
int cariKartu(String kartu) {
  int result = -1;
  for (int i = 0; i < maxMember; i++) {
    if (kartu.substring(1) == card[i].id) {
      result = i;
      break;
    }
  }
  return result;
}
void showCard(int index){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(card[index].user);
  lcd.setCursor(0, 1);
  lcd.print("IDR ");
  lcd.print(card[index].saldo);
  delay(100);
}
