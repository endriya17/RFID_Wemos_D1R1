/**
Useful links
https://cdn-images-1.medium.com/max/1400/1*YKc8KpAfMrlhrOLmNjdRwQ.png (D1 full pinout)
https://github.com/Jorgen-VikingGod/ESP8266-MFRC522
d1 mini rc52 wiring:
https://www.b4x.com/android/forum/threads/solved-rfid-rc522-and-wemos-r1d1-working.129616/
you need to correctly initialize the RC-522 chip telling it the WeMos SS and RST pin. 
Now: the SS pin is wired to D8 which is GPIO15, while the reset pin is D3 which is the GPIO0 so the initialization has to be:
RST  - D3 GPIO0
MISO - D6 
MOSI - D7
SCK  - D5
SDA  - D8 GPIO15
*/

#include <SPI.h>
#include <MFRC522.h>

#define LedPin  2
#define SS_PIN  15
#define RST_PIN 0
        
// Inisialisasi Pin Wemos D1 R1
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);    // Init serial communications
  SPI.begin();           // Init SPI bus
  rfid.PCD_Init();       // Init Modul RFID
  pinMode(LedPin, OUTPUT);  
  digitalWrite(LedPin, HIGH);              
  rfid.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

  Serial.println("Scan kartu RFID untuk menampilkan UID."); 
}

void loop() {
  // Periksa apakah ada kartu yang terdeteksi & Periksa apakah kartu dapat dibaca
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    Serial.print("UID tag :");
    String content= "";
    byte letter;
      for (byte i = 0; i < rfid.uid.size; i++) 
    {
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();
    // Cek apakah UID yang terbaca adalah 73 91 B3 1A
    if (rfid.uid.size == 4 &&
        rfid.uid.uidByte[0] == 0x73 &&
        rfid.uid.uidByte[1] == 0x91 &&
        rfid.uid.uidByte[2] == 0xB3 &&
        rfid.uid.uidByte[3] == 0x1A) 
        {
        Serial.println("Kartu terdaftar terbaca! Menyalakan LED.");
        digitalWrite(LedPin, LOW);
        delay(2500);
        digitalWrite(LedPin, HIGH);
        }
      else {
        Serial.println("Kartu tidak dikenal.");
        digitalWrite(LedPin, HIGH);
      } 
    // Berhenti komunikasi pada modul RFID
    rfid.PICC_HaltA(); 
  }
}