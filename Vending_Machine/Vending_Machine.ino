#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "SerialMP3Player.h"

#define TX 16
#define RX 17

SerialMP3Player mp3(RX, TX);

#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance

const int maxMember = 2;
int user;
struct member {
  String id;
  String user;
  long saldo;
};
bool myRFID=true;
bool saldoCukup;
long harga[5]={0,1000,2000,3000,4000};

member card[maxMember];

//TIMER
unsigned long now,before;
int counter=1;

//-------------MOTOR-----------//
#define motor1_A 35
#define motor1_B 33
#define motor2_A 37
#define motor2_B 39
#define motor3_A 25
#define motor3_B 23
#define motor4_A 29
#define motor4_B 27

//---------LDR---------------//
#define pin_LDR1 10
#define pin_LDR2 9
#define LDR1 digitalRead(pin_LDR1)
#define LDR2 digitalRead(pin_LDR2)

//--------UV--------------//
#define relay_UV 11
#define relay_LIGHT 45

//---------SOUND SENSOR---------------//
#define pin_SOUND0 22
#define pin_SOUND1 24
#define pin_SOUND2 26
#define pin_SOUND3 28
#define pin_SOUND4 30

#define SOUND0 digitalRead(pin_SOUND0)
#define SOUND1 digitalRead(pin_SOUND1)
#define SOUND2 digitalRead(pin_SOUND2)
#define SOUND3 digitalRead(pin_SOUND3)
#define SOUND4 digitalRead(pin_SOUND4)

void setup() {
  pinMode(relay_UV, OUTPUT);
  pinMode(relay_LIGHT, OUTPUT);
  UV_off();

  //RFID
   card[0].id = "73 85 0F 19";
  card[0].user = "MANGURI";
  card[0].saldo = 25000;
  
  card[1].id = "97 E5 1D 39";
  card[1].user = "NAJWA";
  card[1].saldo = 500;
  Serial.begin(9600);

  while (!Serial);
  SPI.begin();
  rfid.PCD_Init();
  delay(10);
  rfid.PCD_DumpVersionToSerial();
  
  //LCD
  lcd.init();
  lcd.backlight();
  lcd.setBacklight(LOW);

  //MP3
  mp3.begin(9600);
  delay(500);
  mp3.sendCommand(CMD_SEL_DEV, 0, 2);
  delay(500);
  Serial.println("OK");
  pinMode(motor1_A, OUTPUT);
  pinMode(motor1_B, OUTPUT);
  pinMode(motor2_A, OUTPUT);
  pinMode(motor2_B, OUTPUT);
  pinMode(motor3_A, OUTPUT);
  pinMode(motor3_B, OUTPUT);
  pinMode(motor4_A, OUTPUT);
  pinMode(motor4_B, OUTPUT);

  pinMode(pin_LDR1, INPUT);
  pinMode(pin_LDR2, INPUT);

  pinMode(pin_SOUND0, INPUT_PULLUP);
  pinMode(pin_SOUND1, INPUT_PULLUP);
  pinMode(pin_SOUND2, INPUT_PULLUP);
  pinMode(pin_SOUND3, INPUT_PULLUP);
  pinMode(pin_SOUND4, INPUT_PULLUP);

  delay(500);
  before=millis();
}

void loop() {
  now=millis();
  if(now-before>=500){
    if(counter%2==0)lcd.setBacklight(LOW);
    else{lcd.setBacklight(HIGH);}
    counter++;
    before=millis();
  }
//  lcd.setBacklight(LOW);
//  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   Say Hello   ");
  lcd.setCursor(0, 1);
  lcd.print(" Untuk Belanja ");
  LIGHT_off();
  Serial.println("MASUK LOOP");

  if (!SOUND0) {
    //---------------------selamat datang  
    LIGHT_on();
    lcd.setBacklight(HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Selamat Datang ");
    lcd.setCursor(0, 1);
    lcd.print("  Smart V-Mac  ");
    mp3.play(9);
    delay(3000);
    //-----------------------
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("   Tempelkan");
    lcd.setCursor(0,1);
    lcd.print("  Kartu Anda");
    mp3.play(1);
    delay(3000);
    //------------------CEK KARTU
    
    while(myRFID){
     Serial.println("MASUK");
     cekRFID(); 
    }
    myRFID=true;  
    delay(3000); 

    //---------------------silahkan memilih snack
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Silahkan Pilih ");
    lcd.setCursor(0, 1);
    lcd.print("      Snack     ");
    mp3.play(2);
    delay(3000);
    while (1) {

      if (!SOUND1) { //-----------------------------SOUND 1
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Anda Memilih  ");
        lcd.setCursor(0, 1);
        lcd.print("    Snack [1]   ");
        mp3.play(3);
        delay(1000);

        //ADD
        saldoCukup=cekMutasi(harga[1],card[user].saldo);
        if(saldoCukup){
     
        
        while (!LDR1 && !LDR2) {
          Serial.println("Motor Muter");
          motor1_run();
        }
        motor_stop();
        UV_on();
        showCard(user);
        delay(3000);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Silahkan Ambil ");
        lcd.setCursor(0, 1);
        lcd.print("   Snack Anda   ");
        delay(800);
        mp3.play(7); //silahkan ambil snack
        delay(4500);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   Terimakasih   ");
        lcd.setCursor(0, 1);
        lcd.print("Selamat Menikmati");
        mp3.play(8); //terimakasih
        delay(4000);
        }
        UV_off();
        lcd.clear();
//        showCard(user);delay(2000);
        break;
      }

      if (!SOUND2) { //-----------------------------SOUND 2
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Anda Memilih  ");
        lcd.setCursor(0, 1);
        lcd.print("    Snack [2]   ");
        mp3.play(4);
        delay(1000);
        
        saldoCukup=cekMutasi(harga[2],card[user].saldo);
        if(saldoCukup){
     
        while (!LDR1 && !LDR2) {
          Serial.println("Motor Muter");
          motor2_run();
        }
        motor_stop();
        UV_on();
        showCard(user);
        delay(3000);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Silahkan Ambil ");
        lcd.setCursor(0, 1);
        lcd.print("   Snack Anda   ");
        delay(800);
        mp3.play(7); //silahkan ambil snack
        delay(4500);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   Terimakasih   ");
        lcd.setCursor(0, 1);
        lcd.print("Selamat Menikmati");
        mp3.play(8); //terimakasih
        delay(4000);
        };
        UV_off();
        lcd.clear();
        break;
      }

      if (!SOUND3) { //-----------------------------SOUND 3
       lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Anda Memilih  ");
        lcd.setCursor(0, 1);
        lcd.print("    Snack [3]   ");
        mp3.play(5);
        delay(1000);

        //ADD
        saldoCukup=cekMutasi(harga[3],card[user].saldo);
        if(saldoCukup){
     
        while (!LDR1 && !LDR2) {
          Serial.println("Motor Muter");
          motor3_run();
        }
        motor_stop();
        UV_on();
        showCard(user);
        delay(3000);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Silahkan Ambil ");
        lcd.setCursor(0, 1);
        lcd.print("   Snack Anda   ");
        delay(800);
        mp3.play(7); //silahkan ambil snack
        delay(4500);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   Terimakasih   ");
        lcd.setCursor(0, 1);
        lcd.print("Selamat Menikmati");
        mp3.play(8); //terimakasih
        delay(4000);
        }
        UV_off();
        lcd.clear();
        break;
      }

      if (!SOUND4) { //-----------------------------SOUND 4
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Anda Memilih  ");
        lcd.setCursor(0, 1);
        lcd.print("    Snack [4]   ");
        mp3.play(6);
        delay(1000);
        
        saldoCukup=cekMutasi(harga[4],card[user].saldo);
        if(saldoCukup){
     
        while (!LDR1 && !LDR2) {
          Serial.println("Motor Muter");
          motor4_run();
        }
        motor_stop();
        UV_on();
        showCard(user);
        delay(3000);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Silahkan Ambil ");
        lcd.setCursor(0, 1);
        lcd.print("   Snack Anda   ");
        delay(800);
        mp3.play(7); //silahkan ambil snack
        delay(4500);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   Terimakasih   ");
        lcd.setCursor(0, 1);
        lcd.print("Selamat Menikmati");
        mp3.play(8); //terimakasih
        delay(4000);
        }
        UV_off();
        lcd.clear();
        break;
      }

    }
  }
  //delay(10);
  //lcd.setBacklight(HIGH);
  //delay(10);

}

void motor1_run() {
  digitalWrite(motor1_A, HIGH);
  digitalWrite(motor1_B, LOW);
}

void motor2_run() {
  digitalWrite(motor2_A, HIGH);
  digitalWrite(motor2_B, LOW);
}

void motor3_run() {
  digitalWrite(motor3_A, HIGH);
  digitalWrite(motor3_B, LOW);
}

void motor4_run() {
  digitalWrite(motor4_A, HIGH);
  digitalWrite(motor4_B, LOW);
}

void motor_stop() {
  digitalWrite(motor1_A, LOW);
  digitalWrite(motor1_B, LOW);
  digitalWrite(motor2_A, LOW);
  digitalWrite(motor2_B, LOW);
  digitalWrite(motor3_A, LOW);
  digitalWrite(motor3_B, LOW);
  digitalWrite(motor4_A, LOW);
  digitalWrite(motor4_B, LOW);
}

void UV_off() {
  digitalWrite(relay_UV, HIGH);
}

void UV_on() {
  digitalWrite(relay_UV, LOW);
}

void LIGHT_on(){
  digitalWrite(relay_LIGHT, HIGH);
}


void LIGHT_off(){
  digitalWrite(relay_LIGHT, LOW);
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
void showGagal(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Saldo Kurang!!");
  lcd.setCursor(0, 1);
  lcd.print(card[user].saldo);
  delay(1000);
}

bool cekMutasi(long price,long saldo){
  if(saldo-price>=0){
   
  card[user].saldo=card[user].saldo-price;
//  showCard(user);
//  delay(2000);
  return true;}
  else{
    showGagal();
    mp3.play(10);
    delay(5000);
    return false;
  //9
  
  }
  
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
void cekRFID(){
  if (!rfid.PICC_IsNewCardPresent()) {
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
  };
  rfid.PICC_HaltA();
  content.toUpperCase();
  Serial.println(content);
  
  int index = cariKartu(content);
  if (index >= 0) {showCard(index);myRFID=false;user=index;}
  else{myRFID=true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  Kartu Anda");
  lcd.setCursor(0,1);
  lcd.print("Tidak Terdaftar");  
  }
}
