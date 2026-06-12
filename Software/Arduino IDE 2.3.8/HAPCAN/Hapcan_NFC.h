#pragma once      // Zabraňuje vícenásobnému vložení hlavičkového souboru
#include <Arduino.h>    // Knihovna základních funkcí
#include <Adafruit_PN532.h>     // Knihovna pro čip PN532

class HapcanProtocol;

// Datová struktura pro dočasné uchování informací o právě přiloženém tagu
struct TagEvent {
  uint8_t uid[8];
  uint8_t len;
  uint32_t removalTime;
};

class HapcanNFC {
public:
  // Konfigurace hardwarových pinů
  HapcanNFC(uint8_t pn532CsPin, uint8_t ledGreenPin, uint8_t ledRedPin);
  
  void begin(HapcanProtocol* protocol, uint32_t frameType);
  void loop();

private:
  // Doba, po kterou modul udržuje stav "karta přiložena" (5 sekund)
  static const uint32_t DELAY_AFTER_REMOVAL = 5000UL;

  HapcanProtocol* _hapcanProto;
  Adafruit_PN532 nfc;

  // Proměnné pro uchování předchozího stavu kvůli detekci změny
  uint8_t lastUid[8];
  uint8_t lastUidLen = 0;
  bool tagPresent = false;

  uint16_t frameTypeBase;
  
  // Stavový cyklus přečteného tagu
  bool cycleRunning = false;
  uint32_t cycleStart = 0;
  uint8_t cycleUid[8];
  uint8_t cycleUidLen = 0;

  bool uidsEqual(const uint8_t *a, const uint8_t *b, uint8_t len);
  void sendUIDOverHapcan(const uint8_t *uid, uint8_t uidLength, bool openFlag);
  void addTag(const uint8_t *uid, uint8_t len);

  uint8_t _ledGreenPin;
  uint8_t _ledRedPin;
};