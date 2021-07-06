#ifndef AUTHENTICATION_HANDLER_H
#define AUTHENTICATION_HANDLER_H

#include <memory>
#include <Arduino.h>
#include "Config.h"
#include "SerialMap.h"
#include "Response.h"
#include "Common.h"

class AuthenticationHandler
{
public:
  AuthenticationHandler() = delete;
  AuthenticationHandler(const String &username,
                        const String &password);
  AuthenticationHandler(const char *username, const char *password);
  bool authenticate(Stream &client);

private:
  String username, password;
  char buffer[1024];
};

#endif
