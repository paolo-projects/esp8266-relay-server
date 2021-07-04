#ifndef ACCESS_POINT_OPERATIONS_H
#define ACCESS_POINT_OPERATIONS_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include "AuthenticationHandler.h"
#include "Cert.h"
#include "Config.h"
#include "Configuration.h"
#include "StateManager.h"
#include "ActionParser.h"
#include "SerialMap.h"
#include "Common.h"

class AccessPointOperations
{
public:
  AccessPointOperations(Configuration &configuration,
                        StateManager &stateManager,
                        std::function<void(void)> manualOverride);
  void startServer();

private:
  Configuration &configuration;
  StateManager &stateManager;
  std::function<void(void)> manualOverride;
  AuthenticationHandler authHandler;
  ActionParser<10> actionParser;

  bool serverRunning = true;

  void setWifiPassword(SerialMap<10> &action, Stream &output);
};

#endif
