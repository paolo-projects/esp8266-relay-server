#include "StateManager.h"

StateManager::StateManager()
{
}

/**
 * @brief Sets the current state
 * 
 * @param newState The new state
 */
void StateManager::setState(MACHINE_STATE newState)
{
  state = newState;
}

/**
 * @brief Gets the current state
 * 
 * @return MACHINE_STATE The current state
 */
MACHINE_STATE StateManager::getState() const
{
  return state;
}

/**
 * @brief Associate the function with the given state
 * 
 * @param state The state to which associate the function
 * @param callback The callback function
 */
void StateManager::registerStateFunction(MACHINE_STATE state, void (*callback)(void))
{
  callbacks.put(state, callback);
}

/**
 * @brief Executes the callback associated to the current state
 * 
 */
void StateManager::executeState() const
{
  auto callback = callbacks[state];
  if (callback != nullptr)
  {
    (*callback)();
  }
}