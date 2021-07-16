#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "RemoteControlServer.h"
#include "RelayManager.h"
#include "Cert.h"

RemoteControlSettings setupServer();
bool getRelayState(ActionMap &action, Stream &output);
bool setRelayState(ActionMap &action, Stream &output);
bool switchToAp(ActionMap &action, Stream &output);
void serverLoop();
void manualOverride();

RemoteControlServer<5> server(setupServer());
RelayManager relayManager;

unsigned long lastButtonPress = millis();

void setup()
{
  Serial.begin(9600);
  pinMode(MANUAL_BUTTON_PIN, INPUT_PULLUP);

  server.addAction("getstate", getRelayState);
  server.addAction("setstate", setRelayState);
  server.addAction("disconnect", switchToAp);

  server.setLoopCallback(serverLoop);
}

void loop()
{
  server.execute();
}

void serverLoop()
{
  manualOverride();
}

void manualOverride()
{
  // Check if a set time has passed since the last button press
  // The timeout is necessary to avoid executing the action multiple times
  // until the button is released
  if (millis() > lastButtonPress + BUTTON_DEBOUNCE_MS)
  {
    if (digitalRead(MANUAL_BUTTON_PIN) == MANUAL_BUTTON_PRESSED_STATE)
    {
      lastButtonPress = millis();
      if (relayManager.getState() == RELAY_OFF)
      {
        relayManager.setState(RELAY_ON);
      }
      else
      {
        relayManager.setState(RELAY_OFF);
      }
    }
  }
}

RemoteControlSettings setupServer()
{
  RemoteControlSettings settings;

  settings.ACCESS_POINT_SETTINGS.WIFI_AP_SSID = AP_SSID;
  settings.ACCESS_POINT_SETTINGS.WIFI_AP_PASS = AP_PASS;
  settings.ACCESS_POINT_SETTINGS.WIFI_AP_HIDDEN = AP_HIDDEN;
  settings.ACCESS_POINT_SETTINGS.WIFI_AP_CHANNEL = AP_CHANNEL;
  settings.ACCESS_POINT_SETTINGS.WIFI_AP_IP_ADDRESS = AP_IP_ADDR;
  settings.ACCESS_POINT_SETTINGS.WIFI_AP_IP_GATEWAY = AP_GATEWAY;
  settings.ACCESS_POINT_SETTINGS.WIFI_AP_SUBNET = AP_SUBNET;
  settings.ACCESS_POINT_SETTINGS.WIFI_AP_MAX_CONN = AP_MAX_CONN;
  settings.ACCESS_POINT_SETTINGS.AUTH_USER = AP_AUTH_USER;
  settings.ACCESS_POINT_SETTINGS.AUTH_PASS = AP_AUTH_PASS;
  settings.ACCESS_POINT_SETTINGS.PORT = AP_SERVER_PORT;
  settings.ACCESS_POINT_SETTINGS.TIMEOUT_MS = TIMEOUT;
  settings.ACCESS_POINT_SETTINGS.CERTIFICATE = Certificate::SERVER_CERT;
  settings.ACCESS_POINT_SETTINGS.PRIVATE_KEY = Certificate::SERVER_KEY;

  settings.COMMAND_SERVER_SETTINGS.HOSTNAME = WLAN_HOSTNAME;
  settings.COMMAND_SERVER_SETTINGS.PORT = SERVER_PORT;
  settings.COMMAND_SERVER_SETTINGS.AUTH_USERNAME = AUTHENTICATION_USERNAME;
  settings.COMMAND_SERVER_SETTINGS.AUTH_PASSWORD = AUTHENTICATION_PASSWORD;
  settings.COMMAND_SERVER_SETTINGS.UDP_PACKET = (char *)UDP_BROADCAST_PACKET;
  settings.COMMAND_SERVER_SETTINGS.UDP_PACKET_SIZE = 32;
  settings.COMMAND_SERVER_SETTINGS.UDP_PORT = UDP_BROADCAST_PORT;
  settings.COMMAND_SERVER_SETTINGS.UDP_RATE_MS = UDP_BROADCAST_RATE;
  settings.COMMAND_SERVER_SETTINGS.TIMEOUT_MS = TIMEOUT;
  settings.COMMAND_SERVER_SETTINGS.CERTIFICATE = Certificate::SERVER_CERT;
  settings.COMMAND_SERVER_SETTINGS.PRIVATE_KEY = Certificate::SERVER_KEY;
  settings.COMMAND_SERVER_SETTINGS.WIFI_TIMEOUT_S = WLAN_CONN_TIMEOUT;

  return settings;
}

bool getRelayState(ActionMap &action, Stream &output)
{
  SerialMap response;
  response.put("state", relayManager.getState() == RELAY_ON ? "on" : "off");
  response.write(output);
  return false;
}

bool setRelayState(ActionMap &action, Stream &output)
{
  if (action.has("state"))
  {
    relayManager.setState((*action["state"]) == "on" ? RELAY_ON : RELAY_OFF);
    Response::successResponse().write(output);
  }
  else
  {
    Response::errorResponse().write(output);
  }
  return false;
}

bool switchToAp(ActionMap &action, Stream &output)
{
  Response::successResponse().write(output);
  return true;
}