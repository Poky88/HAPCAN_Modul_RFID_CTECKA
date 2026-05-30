#include <HapcanProtocol.h>
#include <Hapcan_NFC.h>

// Definice pinů CS na SPI sběrnici.
#define CAN_CS_PIN    33    // převodník CAN
#define PN532_CS      5     // čtečka MCP2515

// Definice pinů pro identifikační LED
#define LED_GREEN     13
#define LED_RED       15

// Výchozí hodnoty Node, Group a Description
#define DEFAULT_NODE  2
#define DEFAULT_GROUP 4
#define DEFAULT_DESC  "NFC Card Reader"

// Instance tříd spravující jednotlivé periferie
HapcanProtocol hapcanProto(CAN_CS_PIN);
HapcanNFC hapcanNfc(PN532_CS, LED_GREEN, LED_RED);

void setup() {
  Serial.begin(115200);
  delay(100);

 // Konfigurace CS pinů
  pinMode(CAN_CS_PIN, OUTPUT);
  digitalWrite(CAN_CS_PIN, HIGH);
  pinMode(PN532_CS, OUTPUT);
  digitalWrite(PN532_CS, HIGH);

  hapcanProto.begin(DEFAULT_NODE, DEFAULT_GROUP, DEFAULT_DESC);     // Spuštění CAN komunikace
  hapcanNfc.begin(&hapcanProto, 0x315);     // Spuštění NFC čtečky
}

void loop() {
  hapcanProto.loop();   // Kontrola přijatých zpráv na sběrnici
  if (!hapcanProto.isProgramming()) {   // Modul obsluhuje čtečku pouze když není v programovacím režimu
    hapcanNfc.loop();
  }
}