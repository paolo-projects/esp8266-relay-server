#include "StateManager.h"

StateManager::StateManager() 
{
  
}

void StateManager::setState(MACHINE_STATE newState)
{
  state = newState;
}

MACHINE_STATE StateManager::getState() const
{
  return state;
}
