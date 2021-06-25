#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "StateManager.h"
#include "Configuration.h"
#include "AccessPointOperations.h"
#include "RelayManager.h"
#include "CommandServer.h"
#include "Config.h"

bool connectToWlan();
bool startAccessPoint();
void manualOverride();
void setup();
void loop();
void manualOverride();

StateManager stateManager;
RelayManager relayManager;
Configuration configuration;
AccessPointOperations accessPoint(configuration, stateManager, manualOverride);
CommandServer commandServer(stateManager, relayManager, manualOverride);

unsigned long lastButtonPress = millis();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MANUAL_BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  // put your main code here, to run repeatedly:
  switch (stateManager.getState())
  {
  case CONNECTING:
    // Search for valid configuration and attempt connecting to the WiFi
    if (configuration.isValid())
    {
      stateManager.setState(CONNECTED);
    }
    else
    {
      // Start AP
      stateManager.setState(AP_MODE);
    }
    break;
  case CONNECTED:
    // Start the command server
    if (connectToWlan())
    {
      // Start server
      commandServer.startServer();
    }
    else
    {
      // Start AP
      stateManager.setState(AP_MODE);
    }

    break;
  case AP_MODE:
    bool result = false;
    while ((result = startAccessPoint()) == false)
    {
      manualOverride();
      delay(150);
    }
    accessPoint.startServer();
    break;
  }
}

void manualOverride()
{
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

bool connectToWlan()
{
  WiFi.mode(WIFI_STA);
  WiFi.hostname(WLAN_HOSTNAME);
  WiFi.begin(configuration.getBSSID(), configuration.getPass());
  Serial.println("Connecting...");
  uint16_t timeout = WLAN_CONN_TIMEOUT;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (timeout <= 0)
    {
      Serial.println("");
      Serial.println("Connection Timeout!");
      return false;
    }
    // Divide 1s delay in 10 iterations of 100ms to allow manual overriding through button
    for (uint8_t i = 0; i < 10; i++)
    {
      manualOverride();
      delay(100);
    }
    timeout--;
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Successfully connected to: ");
  Serial.print(configuration.getBSSID());
  Serial.print(", with IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

bool startAccessPoint()
{
  WiFi.mode(WIFI_AP);
  Serial.println("Starting AP mode...");
  bool result = WiFi.softAPConfig(AP_IP_ADDR, AP_GATEWAY, AP_SUBNET);
  if (!result)
  {
    Serial.println("Error setting AP configuration");
    return false;
  }
  result = WiFi.softAP(AP_SSID, AP_PASS, AP_CHANNEL, AP_HIDDEN, AP_MAX_CONN);
  if (!result)
  {
    Serial.println("Error start soft AP mode");
    return false;
  }
  Serial.print("Soft AP started with SSID: ");
  Serial.print(AP_SSID);
  Serial.print(" and local IP: ");
  Serial.println(WiFi.softAPIP());
  return true;
}
