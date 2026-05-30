#pragma once
#include <SPI.h>
#include <mcp_can.h>      // Knihovna pro řadič MCP2515
#include <Preferences.h>    // Nevolatilní paměť

class HapcanProtocol {
public:
  HapcanProtocol(uint8_t csPin);
  void begin(uint8_t defaultNode, uint8_t defaultGroup, const char* defaultDesc);
  void loop();

  // Funkce pro sestavení zpráv
  void sendEvent(uint32_t type, uint8_t *data);
  void sendFrame(uint32_t type, uint8_t *data);
  bool isProgramming() const { return is_programming; }

private:
  // Funkce pro odesílání odpovědí
  void processMsg(uint32_t rxId, uint8_t *data);
  void sendHardwareResponse(uint32_t reqType);
  void sendFirmwareResponse(uint32_t reqType);
  void sendDescriptionResponse(uint32_t reqType);
  void sendVoltageResponse(uint32_t reqType);
  void loadConfig(uint8_t defNode, uint8_t defGroup, const char* defDesc);
  void saveDescription();

  MCP_CAN CAN0;
  Preferences prefs;

  // Proměnné pro aktuální konfiguraci
  uint8_t current_node;
  uint8_t current_group;
  uint8_t description[16];

  uint32_t _progAddr;
  uint8_t  _progCmd;
  bool is_programming; 
  uint32_t my_serial;
};