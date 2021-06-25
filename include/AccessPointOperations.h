#ifndef ACCESS_POINT_OPERATIONS_H
#define ACCESS_POINT_OPERATIONS_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include "Configuration.h"
#include "StateManager.h"
#include "Cert.h"
#include "PacketParser.h"
#include "Config.h"

/*
 * Format:
 * PI<length of username: uint8_t><length of pass: uint8_t><AUTH USER><AUTH PASS>
 * then
 * PI<length of ssid: uint8_t><length of wifi pass: uint8_t><SSID><WIFI PASS>
 * 
 * responses:
 * PIOK
 */

class AccessPointOperations
{
public:
  AccessPointOperations(Configuration &configuration, StateManager &stateManager, std::function<void(void)> manualOverride);
  void startServer();

private:
  Configuration &configuration;
  StateManager &stateManager;
  std::function<void(void)> manualOverride;
};

#endif
