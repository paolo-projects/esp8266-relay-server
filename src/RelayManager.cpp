#include "RelayManager.h"

RelayManager::RelayManager()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(LED_PIN, LED_PIN_OFF);
    digitalWrite(RELAY_PIN, RELAY_PIN_OFF);
}

void RelayManager::setState(RELAY_STATE state)
{
    if (CurrentState != state)
    {
        if (state == RELAY_ON)
        {
            digitalWrite(RELAY_PIN, RELAY_PIN_ON);
            digitalWrite(LED_PIN, LED_PIN_ON);
        }
        else
        {
            digitalWrite(RELAY_PIN, RELAY_PIN_OFF);
            digitalWrite(LED_PIN, LED_PIN_OFF);
        }
        CurrentState = state;
    }
}

RELAY_STATE RelayManager::getState() const
{
    return CurrentState;
}