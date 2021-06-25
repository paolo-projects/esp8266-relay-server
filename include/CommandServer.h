#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//#include <ESP8266WebServer.h>
#include <functional>
#include "Configuration.h"
#include "StateManager.h"
#include "RelayManager.h"
#include "Cert.h"
#include "PacketParser.h"
#include "Config.h"

class CommandServer
{
public:
  CommandServer(StateManager &stateManager, RelayManager &relayManager, std::function<void(void)> manualOverride);
  void startServer();

private:
  StateManager &stateManager;
  RelayManager &relayManager;
  std::function<void(void)> manualOverride;
  bool authHandler(BearSSL::WiFiClientSecure &client);
  void udpBroadcast();
  unsigned long lastUdpBroadcast = 0;
  WiFiUDP udp;
  IPAddress broadcastIp;

  static constexpr uint8_t UDPBcastPacket[32] = {
      0x0d, 0x2e, 0x82, 0xdf, 0xfd, 0xf7, 0x4f, 0x4a, 0x0d, 0x6d, 0xd8, 0x8a, 0xc1, 0xb2,
      0x8c, 0x9e, 0x4c, 0x28, 0x84, 0xa3, 0x19, 0x18, 0x58, 0xc9, 0xcd, 0x91, 0x98, 0x58,
      0xc2, 0xdc, 0x45, 0xc4};
};

#endif
