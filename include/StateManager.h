#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "Map.h"

enum MACHINE_STATE
{
  CONNECTING,
  CONNECTED,
  AP_MODE
};

class StateManager
{
public:
  StateManager();
  void registerStateFunction(MACHINE_STATE state, void (*callback)(void));
  void setState(MACHINE_STATE newState);
  MACHINE_STATE getState() const;
  void executeState() const;

private:
  MACHINE_STATE state = CONNECTING;
  Map<MACHINE_STATE, void (*)(void), 3> callbacks;
};

#endif // STATE_MANAGER_H
