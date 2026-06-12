#include "HapcanProtocol.h"

// Virtuální adresy pro uložení Node, Group a Description
#define EEPROM_NODE_ADDR   0xF00026
#define EEPROM_GROUP_ADDR  0xF00027
#define EEPROM_DESC_ADDR   0xF00030

HapcanProtocol::HapcanProtocol(uint8_t csPin)
  : CAN0(csPin), my_serial(0x00120424), _progAddr(0), _progCmd(0), is_programming(false) {} 

void HapcanProtocol::begin(uint8_t defaultNode, uint8_t defaultGroup, const char* defaultDesc) {
  SPI.begin(18, 19, 23);      // Inicializace SPI (SCK=18, MISO=19, MOSI=23)
  prefs.begin("hapcan_cfg", false);     // Přístup do flash paměti
  loadConfig(defaultNode, defaultGroup, defaultDesc);

  // Inicializace CAN řadiče na rychlost 125 kbps
  if (CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) == CAN_OK)
    CAN0.setMode(MCP_NORMAL);
  else
    while (1) delay(1000);
}

void HapcanProtocol::loadConfig(uint8_t defNode, uint8_t defGroup, const char* defDesc) {
  current_node = prefs.getUChar("node", defNode);     // Načtení dat. Pokud v paměti zatím nic není, použijí se defaultní
  current_group = prefs.getUChar("group", defGroup);
  size_t len = prefs.getBytes("desc", description, 16);
  
  // Ochrana proti nekompletnímu popisu
  if (len < 16) {
    memset(description, 0x00, 16);
    uint8_t copyLen = strlen(defDesc) > 16 ? 16 : strlen(defDesc);
    memcpy(description, (const uint8_t*)defDesc, copyLen);
    prefs.putBytes("desc", description, 16);      // Zápis řetězce do paměti
  }
}

void HapcanProtocol::saveDescription() {
  prefs.putBytes("desc", description, 16);
}

void HapcanProtocol::loop() {
  // CAN_MSGAVAIL indikuje, že je v MCP2515 minimálně jedna nepřečtená zpráva
  while (CAN0.checkReceive() == CAN_MSGAVAIL) {
    uint32_t rxId;
    uint8_t len = 0;
    uint8_t rxBuf[8] = {0};
    CAN0.readMsgBuf(&rxId, &len, rxBuf);    // Vyčtení zprávy do lokálních proměnných
    processMsg(rxId, rxBuf);
  }
}

void HapcanProtocol::processMsg(uint32_t rxId, uint8_t *data) {
  // Bitové vymaskování horních 3 bitů, na 29 bitů
  uint32_t realRxId = rxId & 0x1FFFFFFF;
  if ((realRxId & (1UL << 16)) != 0) {
      return; 
  }
  // Získání typu zprávy
  uint32_t msgType = (uint32_t)((realRxId >> 17) & 0xFFF);

  // Požadavek na vstup do programovacího režimu
  if (msgType == 0x100) {
      // Modul vstoupí do režimu pouze, pokud data obsahují jeho stávající Node a Group
      if (data[2] == current_node && data[3] == current_group) {
          is_programming = true;
          // Odeslání potvrzovací odpovědi.
          uint8_t resp[8] = { current_node, current_group, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
          sendFrame(0x100, resp);
      }
      return;
  }

  // Restart nebo ukončení programovacího režimu
  if (msgType == 0x010 || msgType == 0x020) {
      if (is_programming) {
          is_programming = false;
          // Uložení změn z mezipaměti do paměti FLASH
          prefs.putUChar("node", current_node);
          prefs.putUChar("group", current_group);
          saveDescription();
      }
      return;
  }

  // zápis/čtení do paměti
  if (msgType == 0x030) {
      if (!is_programming) return;
      // Adresa v paměti
      _progAddr = ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2];
      _progCmd = data[5];
      sendFrame(0x030, data);
      return;
  }

  // Operace s pamětí
  if (msgType == 0x040) {
      if (!is_programming) return; 
      if (_progCmd == 0x02) {     // Zápis
          uint8_t next_n = current_node;
          uint8_t next_g = current_group;
          for (uint8_t i = 0; i < 8; i++) {
              if (data[i] == 0xFF) continue; 
              uint32_t addr = _progAddr + i;
              if (addr == EEPROM_NODE_ADDR) next_n = data[i];
              else if (addr == EEPROM_GROUP_ADDR) next_g = data[i];
              else if (addr >= EEPROM_DESC_ADDR && addr <= EEPROM_DESC_ADDR + 15)
                  description[addr - EEPROM_DESC_ADDR] = data[i];
          }
          sendFrame(0x040, data);
          current_node = next_n;
          current_group = next_g;
          return;
      }
      if (_progCmd == 0x01) {   // Čtení
          uint8_t resp[8]; memset(resp, 0xFF, 8); 
          for (uint8_t i = 0; i < 8; i++) {
              uint32_t addr = _progAddr + i;
              if (addr == EEPROM_NODE_ADDR) resp[i] = current_node;
              else if (addr == EEPROM_GROUP_ADDR) resp[i] = current_group;
              else if (addr >= EEPROM_DESC_ADDR && addr <= EEPROM_DESC_ADDR + 15)
                  resp[i] = description[addr - EEPROM_DESC_ADDR];
          }
          sendFrame(0x040, resp);
          return;
      }
  }

  uint8_t targetNode = data[2];
  uint8_t targetGroup = data[3];

// Musí být v programovacím režimu a určena pro tento modul
if ((targetGroup != current_group && targetGroup) || (targetNode != current_node && targetNode) || is_programming) return;

  // Jednotlivé dotazy
  switch (msgType) {
    case 0x103:
    case 0x104:
      sendHardwareResponse(msgType);
      break;
    case 0x105:
    case 0x106:
      sendFirmwareResponse(msgType);
      break;
    case 0x10B:
    case 0x10C:
      sendVoltageResponse(msgType);
      break;
    case 0x10D:
    case 0x10E:
      sendDescriptionResponse(msgType);
      break;
  }
}

// Jednotlivé definice odpovědí na dotazy
void HapcanProtocol::sendFrame(uint32_t type, uint8_t *data) {
  uint32_t id = ((uint32_t)type << 17) | (1UL << 16) | ((uint32_t)current_node << 8) | current_group;
  CAN0.sendMsgBuf(id, 1, 8, data);
}

void HapcanProtocol::sendEvent(uint32_t type, uint8_t *data) {
  uint32_t id = ((uint32_t)type << 17) | (0UL << 16) | ((uint32_t)current_node << 8) | current_group;
  CAN0.sendMsgBuf(id, 1, 8, data);
}

void HapcanProtocol::sendHardwareResponse(uint32_t reqType) {
  uint8_t d[8] = { 0x30, 0x00, 0x03, 0xFF, 0x00, 0x12, 0x04, 0x24 }; 
  sendFrame(reqType, d);
}

void HapcanProtocol::sendFirmwareResponse(uint32_t reqType) {
  uint8_t d[8] = { 0x30, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00 }; 
  sendFrame(reqType, d);
}

void HapcanProtocol::sendDescriptionResponse(uint32_t reqType) {
   uint8_t d[8];
   // Rozdělení Description do dvou zpráv
   memcpy(d, description, 8);
   sendFrame(0x10D, d);
   delay(40); 
   memcpy(d, description + 8, 8);
   sendFrame(0x10D, d); 
}

void HapcanProtocol::sendVoltageResponse(uint32_t reqType) {    // Měření napětí
  const uint8_t analogPin = 34; 
  analogReadResolution(12); 
  
  // Konstanty napěťového děliče
  constexpr float R1 = 100000.0f; 
  constexpr float R2 = 10000.0f; 
  
  constexpr float VBUS_FACTOR = (3.3f / 4095.0f) * ((R1 + R2) / R2) * (65472.0f / 30.5f);

  uint16_t v_bus = (uint16_t)(analogRead(analogPin) * VBUS_FACTOR);
  
  uint8_t d[8];
  d[0] = (uint8_t)(v_bus >> 8);
  d[1] = (uint8_t)(v_bus & 0xFF);
  d[2] = 0xFF; 
  d[3] = 0xFF;
  memset(d + 4, 0xFF, 4);

  sendFrame(reqType, d);
}