#include "Hapcan_NFC.h"
#include "HapcanProtocol.h"
#include <string.h>

HapcanNFC::HapcanNFC(uint8_t pn532CsPin, uint8_t ledGreenPin, uint8_t ledRedPin)    //Inicializace proměnných
  : nfc(pn532CsPin), _ledGreenPin(ledGreenPin), _ledRedPin(ledRedPin) {
  memset(lastUid, 0, sizeof(lastUid));
  _hapcanProto = nullptr;

  // Výchozí stavy LED pinů
  pinMode(_ledGreenPin, OUTPUT);
  pinMode(_ledRedPin, OUTPUT);
  
  digitalWrite(_ledGreenPin, LOW);
  digitalWrite(_ledRedPin, HIGH);
}

void HapcanNFC::begin(HapcanProtocol* protocol, uint32_t frameType) {
  _hapcanProto = protocol;
  frameTypeBase = (uint16_t)frameType;

  // Kontrola, zda NFC čip komunikuje
  nfc.begin();
  uint32_t ver = nfc.getFirmwareVersion();
  if (ver) {
    nfc.SAMConfig();    // Režim čtení karet
  }

  cycleRunning = false;
  tagPresent = false;
  lastUidLen = 0;
}

// Funkce porovnávající obsah dvou UID bajt po bajtu
bool HapcanNFC::uidsEqual(const uint8_t *a, const uint8_t *b, uint8_t len) {
  for (uint8_t i = 0; i < len; i++)
    if (a[i] != b[i]) return false;
  return true;
}

// Vytvoření HAPCAN rámce a zaslání
void HapcanNFC::sendUIDOverHapcan(const uint8_t *uid, uint8_t uidLength, bool openFlag) {
  if (_hapcanProto == nullptr) return;

  uint8_t data[8] = {0};
  for (uint8_t i = 0; i < uidLength && i < 7; i++)
    data[i] = uid[i];
  data[7] = openFlag ? 1 : 0;   // 1 = karta přiložena, 0 = karta oddálena

  _hapcanProto->sendEvent(frameTypeBase, data);  // Odeslání zprávy
}

// Zaznamená detekci nového tagu a odstartuje časovač
void HapcanNFC::addTag(const uint8_t *uid, uint8_t len) {
  if (cycleRunning) return;

  memcpy(cycleUid, uid, len);
  cycleUidLen = len;

  sendUIDOverHapcan(uid, len, true);
  cycleStart = millis();        // Zaznamenání času procesoru pro odpočet
  cycleRunning = true;
}

void HapcanNFC::loop() {
  uint8_t uid[8];
  uint8_t uidLen = 0;

  // Čtení z PN532
  bool present = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen, 100);

  if (present) {    // Karta detekována

    digitalWrite(_ledGreenPin, HIGH);
    digitalWrite(_ledRedPin, LOW);   

    // Kontrola, zda jde o novou kartu nebo o tu samou
    if (!tagPresent || uidLen != lastUidLen || !uidsEqual(uid, lastUid, uidLen)) {
      memcpy(lastUid, uid, uidLen);
      lastUidLen = uidLen;
      tagPresent = true;
      addTag(uid, uidLen);
    }
  } else {
    tagPresent = false;   // Nic nenačteno

    digitalWrite(_ledGreenPin, LOW);
    digitalWrite(_ledRedPin, HIGH);
  }

  // Časovač pro uvolnění tagu
  // Po uplynutí 5 sekund pošle zprávu s informací o oddálení tag
  if (cycleRunning && (millis() - cycleStart >= 5000UL)) {
    sendUIDOverHapcan(cycleUid, cycleUidLen, false);
    cycleRunning = false;
  }
}