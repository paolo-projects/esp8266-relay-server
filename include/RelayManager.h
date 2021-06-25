#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include <Arduino.h>
#include "Config.h"

enum RELAY_STATE
{
    RELAY_ON,
    RELAY_OFF
};

class RelayManager
{
public:
    RelayManager();
    void setState(RELAY_STATE state);
    RELAY_STATE getState() const;

private:
    RELAY_STATE CurrentState = RELAY_OFF;
};

#endif