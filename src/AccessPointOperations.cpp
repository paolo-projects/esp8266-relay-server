#include "AccessPointOperations.h"

AccessPointOperations::AccessPointOperations(Configuration &configuration, StateManager &stateManager, std::function<void(void)> manualOverride)
    : configuration(configuration), stateManager(stateManager), manualOverride(manualOverride)
{
}

/*
 * Format:
 * PI<length of username: uint8_t><length of pass: uint8_t><AUTH USER><AUTH PASS>
 * then
 * PI<length of ssid: uint8_t><length of wifi pass: uint8_t><SSID><WIFI PASS>
 * 
 * responses:
 * PIOK
 */

void AccessPointOperations::startServer()
{
  BearSSL::WiFiServerSecure server(AP_SERVER_PORT);
  bool serverRunning = true;

  // Attach the server private cert/key combo
  BearSSL::X509List *serverCertList = new BearSSL::X509List(SERVER_CERT);
  BearSSL::PrivateKey *serverPrivKey = new BearSSL::PrivateKey(SERVER_KEY);

  server.setRSACert(serverCertList, serverPrivKey);

  server.begin();

  while (serverRunning)
  {
    BearSSL::WiFiClientSecure incoming = server.available();
    if (incoming)
    {
      Serial.print("Connection received from ");
      Serial.println(incoming.remoteIP());
      int timeout = millis();
      while (!incoming.available() && millis() - timeout < TIMEOUT)
        delay(50);
      if (incoming.available())
      {
        char buffer[256];

        String auth_user, auth_pass, wlan_ssid, wlan_pass;
        uint16_t user_len, pass_len;

        // Read control characters PI
        size_t read_data = incoming.readBytes((uint8_t *)(&buffer), 2);
        if (read_data != 2 || buffer[0] != 'P' || buffer[1] != 'I')
        {
          Serial.println("Control characters not received");
          goto breakconn;
        }

        // Read Auth user and pass length
        read_data = incoming.readBytes((uint8_t *)(&buffer), 4);
        if (read_data != 4)
        {
          Serial.println("User/Pass lengths not received");
          goto breakconn;
        }

        memcpy(&user_len, buffer, 2);
        memcpy(&pass_len, buffer + 2, 2);
        if (user_len <= 0 || user_len > 255 || pass_len <= 0 || pass_len > 255)
        {
          Serial.println("Bad user/pass lengths");
          goto breakconn;
        }

        // Read user
        read_data = incoming.readBytes((uint8_t *)(&buffer), user_len);
        if (read_data != user_len)
        {
          Serial.println("Couldn't read user");
          goto breakconn;
        }

        buffer[min(user_len, (uint16_t)255)] = 0;
        auth_user = String(buffer);

        // Read pass
        read_data = incoming.readBytes((uint8_t *)(&buffer), pass_len);
        if (read_data != pass_len)
        {
          Serial.println("Couldn't read pass");
          goto breakconn;
        }

        buffer[min(pass_len, (uint16_t)255)] = 0;
        auth_pass = String(buffer);

        if (auth_user != AUTH_USER || auth_pass != AUTH_PASS)
        {
          Serial.println("Bad authentication");
          goto breakconn;
        }

        // Authentication ok
        Serial.println("Authentication OK");
        incoming.write((const uint8_t *)"PIOK", 4);
        incoming.flush();

        // Wait for new transmission
        timeout = millis();
        while (!incoming.available() && millis() - timeout < TIMEOUT)
          delay(50);

        // Read control characters PI
        read_data = incoming.readBytes((uint8_t *)(&buffer), 2);
        if (read_data != 2 || buffer[0] != 'P' || buffer[1] != 'I')
        {
          Serial.println("Control characters not received");
          goto breakconn;
        }

        // Read ssid and password len
        read_data = incoming.readBytes((uint8_t *)(&buffer), 4);
        if (read_data != 4)
        {
          Serial.println("SSID/Pass lengths not received");
          goto breakconn;
        }

        memcpy(&user_len, buffer, 2);
        memcpy(&pass_len, buffer + 2, 2);
        if (user_len <= 0 || user_len > 255 || pass_len <= 0 || pass_len > 255)
        {
          Serial.println("Bad SSID/Pass lengths");
          goto breakconn;
        }

        // Read SSID
        read_data = incoming.readBytes((uint8_t *)(&buffer), user_len);
        if (read_data != user_len)
        {
          Serial.println("Bad SSID");
          goto breakconn;
        }

        buffer[min(user_len, (uint16_t)255)] = 0;
        wlan_ssid = String(buffer);

        // Read Pass
        read_data = incoming.readBytes((uint8_t *)(&buffer), pass_len);
        if (read_data != pass_len)
        {
          Serial.println("Bad pass");
          goto breakconn;
        }

        buffer[min(pass_len, (uint16_t)255)] = 0;
        wlan_pass = String(buffer);

        // Authentication ok
        Serial.println("New Wlan SSID/Password saved");
        incoming.write((const uint8_t *)"PIOK", 4);
        incoming.flush();

        configuration.updateConfig(wlan_ssid, wlan_pass);
        stateManager.setState(CONNECTING);
        serverRunning = false;
      breakconn:
        Serial.println("Closing connection");
        incoming.stop();
      }
    }
    manualOverride();
    delay(20);
  }

  server.stop();
}
