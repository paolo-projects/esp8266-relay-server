#include "AccessPointOperations.h"

AccessPointOperations::AccessPointOperations(
    Configuration &configuration, StateManager &stateManager,
    std::function<void(void)> manualOverride)
    : configuration(configuration), stateManager(stateManager),
      manualOverride(manualOverride), authHandler(AP_AUTH_USER, AP_AUTH_PASS)
{
    actionParser.with("setwifi", CALLBACK(AccessPointOperations, setWifiPassword));
}

void AccessPointOperations::startServer()
{
    BearSSL::WiFiServerSecure server(AP_SERVER_PORT);

    server.setRSACert(&Certificate::serverCert, &Certificate::serverPrivateKey);

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
        stateManager.setState(CONNECTING);

        Response::successResponse().write(output);
    }
}