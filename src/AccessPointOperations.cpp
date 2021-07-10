#include "AccessPointOperations.h"

AccessPointOperations::AccessPointOperations(
    Configuration &configuration, StateManager &stateManager,
    std::function<void(void)> manualOverride)
    : configuration(configuration), stateManager(stateManager),
      manualOverride(manualOverride), authHandler(AP_AUTH_USER, AP_AUTH_PASS)
{
    actionParser.with("setwifi", CALLBACK(AccessPointOperations, setWifiPassword));
}

/*
 * Format:
 * PI<length of username: uint8_t><length of pass: uint8_t><AUTH USER><AUTH
 * PASS> then PI<length of ssid: uint8_t><length of wifi pass:
 * uint8_t><SSID><WIFI PASS>
 *
 * responses:
 * PIOK
 */

void AccessPointOperations::startServer()
{
    BearSSL::WiFiServerSecure server(AP_SERVER_PORT);

    // Attach the server private cert/key combo
    BearSSL::X509List serverCertList = BearSSL::X509List(SERVER_CERT);
    BearSSL::PrivateKey serverPrivKey = BearSSL::PrivateKey(SERVER_KEY);

    server.setRSACert(&serverCertList, &serverPrivKey);

    server.begin(AP_SERVER_PORT);

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
                if (authHandler.authenticate(incoming))
                {
                    // Wait for new transmission
                    timeout = millis();
                    while (!incoming.available() && millis() - timeout < TIMEOUT)
                        delay(50);

                    ActionMap action = ActionMap::fromStream(incoming, TIMEOUT);

                    actionParser.execute(action, incoming);
                }
                else
                {
                    Serial.println("Bad authentication");
                }

                Serial.println("Closing connection");
                incoming.stop();
            }
        }
        manualOverride();
        delay(20);
    }

    server.stop();
}

void AccessPointOperations::setWifiPassword(ActionMap &action, Stream &output)
{
    if (action.has("bssid") && action.has("password"))
    {
        configuration.updateConfig(*action.get("bssid"), *action.get("password"));
        serverRunning = false;
    }
}