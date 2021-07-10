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
#include "Config.h"
#include "AuthenticationHandler.h"
#include "ActionParser.h"
#include "SerialMap.h"
#include "Common.h"
#include "Response.h"

class CommandServer
{
public:
  CommandServer(StateManager &stateManager, RelayManager &relayManager, std::function<void(void)> manualOverride);
  void startServer();

private:
  StateManager &stateManager;
  RelayManager &relayManager;
  std::function<void(void)> manualOverride;
  AuthenticationHandler authHandler;
  ActionParser<10> actionParser;
  //bool authHandler(BearSSL::WiFiClientSecure &client);
  void udpBroadcast();
  void shutDown(ActionMap &action, Stream &output);
  void getState(ActionMap &action, Stream &output);
  void setState(ActionMap &action, Stream &output);

  unsigned long lastUdpBroadcast = 0;
  WiFiUDP udp;
  IPAddress broadcastIp;
  bool serverRunning = true;
  char outBuffer[512] = {};

  static constexpr char UDPBcastPacket[32] = UDP_BROADCAST_PACKET;
};

#endif
