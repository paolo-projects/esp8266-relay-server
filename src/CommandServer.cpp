#include "CommandServer.h"

constexpr char CommandServer::UDPBcastPacket[];

CommandServer::CommandServer(StateManager &stateManager, RelayManager &relayManager, std::function<void(void)> manualOverride)
    : stateManager(stateManager), relayManager(relayManager), manualOverride(manualOverride), authHandler(AUTHENTICATION_USERNAME, AUTHENTICATION_PASSWORD)
{
    actionParser
        .with("disconnect", CALLBACK(CommandServer, shutDown))
        .with("getstate", CALLBACK(CommandServer, getState))
        .with("setstate", CALLBACK(CommandServer, setState));
}

void CommandServer::startServer()
{
    BearSSL::WiFiServerSecure server(SERVER_PORT);

    // Attach the server private cert/key combo
    BearSSL::X509List *serverCertList = new BearSSL::X509List(SERVER_CERT);
    BearSSL::PrivateKey *serverPrivKey = new BearSSL::PrivateKey(SERVER_KEY);

    server.setRSACert(serverCertList, serverPrivKey);

    server.begin();
    udp.begin(UDP_BROADCAST_RATE);

    broadcastIp = WiFi.localIP();
    broadcastIp[3] = 255;

    while (serverRunning)
    {
        auto client = server.available();

        if (client)
        {
            Serial.print("Connection received from ");
            Serial.println(client.remoteIP());

            if (authHandler.authenticate(client))
            {
                ActionMap action = ActionMap::fromStream(client, TIMEOUT);
                actionParser.execute(action, client);
            }
            else
            {
                Serial.println("Authentication failed");
            }
            client.stop();
        }
        manualOverride();
        udpBroadcast();
        delay(20);
    }
    server.stop();
}

void CommandServer::udpBroadcast()
{
    if (lastUdpBroadcast == 0 || millis() > lastUdpBroadcast + UDP_BROADCAST_RATE)
    {
        udp.beginPacket(broadcastIp, UDP_BROADCAST_PORT);
        udp.write(CommandServer::UDPBcastPacket, sizeof(CommandServer::UDPBcastPacket));
        udp.endPacket();
        lastUdpBroadcast = millis();
    }
}

void CommandServer::shutDown(ActionMap &action, Stream &output)
{

    Serial.println("Requested connection shutdown, switching to AP mode");

    Response::successResponse().write(output);

    stateManager.setState(AP_MODE);
    serverRunning = false;
}

void CommandServer::getState(ActionMap &action, Stream &output)
{
    // Get state
    Serial.println("Requested relay state");
    RELAY_STATE state = relayManager.getState();
    Serial.print("Sending relay state: ");
    Serial.println(state == RELAY_ON ? "on" : "off");

    Response::statusResponse(state).write(output);
}

void CommandServer::setState(ActionMap &action, Stream &output)
{
    if (action.has("state"))
    {
        const String &state = *action.get("state");
        // Set state
        Serial.println("Requested Relay State SET");
        Serial.print("Setting relay to: ");
        Serial.print(state);

        if (state == "off")
            // Set to off
            relayManager.setState(RELAY_OFF);
        else if (state == "on")
        {
            // Set to on
            relayManager.setState(RELAY_ON);
        }

        Response::successResponse().write(output);
    }
}