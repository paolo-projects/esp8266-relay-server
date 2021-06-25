#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

enum MACHINE_STATE {
  CONNECTING,
  CONNECTED,
  AP_MODE
};

class StateManager
{
public:
  StateManager();
  void setState(MACHINE_STATE newState);
  MACHINE_STATE getState() const;
private:
  MACHINE_STATE state = CONNECTING;
};

#endif // STATE_MANAGER_H
