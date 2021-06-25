#include "CommandServer.h"

constexpr uint8_t CommandServer::UDPBcastPacket[];

CommandServer::CommandServer(StateManager &stateManager, RelayManager &relayManager, std::function<void(void)> manualOverride)
    : stateManager(stateManager), relayManager(relayManager), manualOverride(manualOverride)
{
}

void CommandServer::startServer()
{
  BearSSL::WiFiServerSecure server(SERVER_PORT);
  bool serverRunning = true;

  // Attach the server private cert/key combo
  BearSSL::X509List *serverCertList = new BearSSL::X509List(SERVER_CERT);
  BearSSL::PrivateKey *serverPrivKey = new BearSSL::PrivateKey(SERVER_KEY);

  server.setRSACert(serverCertList, serverPrivKey);

  server.begin();
  udp.begin(UDP_BROADCAST_RATE);

  broadcastIp = WiFi.localIP();
  broadcastIp[3] = 255;

  /*
    Command -> unsigned 8 bit, one of:
      - 0x00 -> disconnect
      - 0x01 -> get state (on (0x01), off (0x00))
      - 0x02 -> set state (on (0x01), off (0x00))
  */
  /*
    Set value -> unsigned 8 bit:
      If command is 0x02, this byte set to ON if 0x01,
      to OFF if 0x00
  */

  while (serverRunning)
  {
    auto client = server.available();
    size_t read_data;
    char buffer[4];

    if (client)
    {
      Serial.print("Connection received from ");
      Serial.println(client.remoteIP());

      if (!authHandler(client))
      {
        Serial.println("Authentication failed");
        goto clientfailed;
      }

      read_data = client.readBytes(buffer, 4);

      if (read_data != 4)
      {
        Serial.println("Couldn't read the right amount of data");
        goto clientfailed;
      }

      if (buffer[0] != 'P' || buffer[1] != 'I')
      {
        Serial.println("Bad control characters");
        goto clientfailed;
      }

      switch (buffer[2])
      {
      case 0x00:
      {
        // Shutdown
        Serial.println("Requested connection shutdown, switching to AP mode");
        memcpy(buffer, "PIOK", 4);
        client.write((uint8_t *)buffer, 4);
        stateManager.setState(AP_MODE);
        serverRunning = false;
        break;
      }
      case 0x01:
      {
        // Get state
        Serial.println("Requested relay state");
        RELAY_STATE state = relayManager.getState();
        Serial.print("Sending relay state: ");
        Serial.println(state == RELAY_ON ? "ON" : "OFF");

        buffer[2] = state == RELAY_ON ? 0x01 : 0x00;
        client.write((uint8_t *)buffer, 3);
        break;
      }
      case 0x02:
      {
        // Set state
        Serial.println("Requested Relay State SET");
        Serial.print("Setting relay to: ");
        if (buffer[3] == 0x00)
          Serial.println("OFF");
        else if (buffer[3] == 0x01)
          Serial.println("ON");
        else
          Serial.println("unknown?");

        switch (buffer[3])
        {
        case 0x00:
          // Set to off
          relayManager.setState(RELAY_OFF);
          break;
        case 0x01:
          // Set to on
          relayManager.setState(RELAY_ON);
          break;
        }
        memcpy(buffer, "PIOK", 4);
        client.write((uint8_t *)buffer, 4);
        break;
      }
      }

    clientfailed:
      client.stop();
    }
    manualOverride();
    udpBroadcast();
    delay(20);
  }
  server.stop();
}

bool CommandServer::authHandler(BearSSL::WiFiClientSecure &client)
{
  // 2 bytes control chars + 4 bytes for string lengths + max 256*2 strings
  char buffer[256];
  size_t read_data;
  uint16_t user_len, pass_len;
  String user, pass;

  read_data = client.readBytes(buffer, 2);

  if (read_data != 2)
    return false;

  if (buffer[0] != 'P' || buffer[1] != 'I')
    return false;

  read_data = client.readBytes(buffer, 4);
  if (read_data != 4)
    return false;

  memcpy(&user_len, buffer, 2);
  memcpy(&pass_len, buffer + 2, 2);

  if (user_len <= 0 || user_len > 255 || pass_len <= 0 || pass_len > 255)
    return false;

  client.readBytes(buffer, user_len);
  buffer[min(user_len, (uint16_t)255)] = 0;

  user = String(buffer);

  client.readBytes(buffer, pass_len);
  buffer[min(pass_len, (uint16_t)255)] = 0;

  pass = String(buffer);

  if (user == AUTHENTICATION_USERNAME && pass == AUTHENTICATION_PASSWORD)
  {
    memcpy(buffer, "PIOK", 4);
    client.write((uint8_t *)buffer, 4);
    return true;
  }
  else
  {
    return false;
  }
}

void CommandServer::udpBroadcast()
{
  if (lastUdpBroadcast == 0 || millis() > lastUdpBroadcast + UDP_BROADCAST_RATE)
  {
    udp.beginPacket(broadcastIp, UDP_BROADCAST_PORT);
    udp.write(CommandServer::UDPBcastPacket, 32);
    udp.endPacket();
    lastUdpBroadcast = millis();
  }
}