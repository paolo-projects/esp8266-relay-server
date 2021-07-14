#include "AuthenticationHandler.h"

AuthenticationHandler::AuthenticationHandler(const String &username,
											 const String &password)
	: username(username), password(password) {}

AuthenticationHandler::AuthenticationHandler(const char *username,
											 const char *password)
	: username(username), password(password) {}

bool AuthenticationHandler::authenticate(Stream &client)
{
	ActionMap authentication = ActionMap::fromStream(client, TIMEOUT);

	if (authentication.has("username") &&
		authentication.has("password") &&
		(*authentication.get("username")) == this->username && (*authentication.get("password")) == this->password)
	{
		Response::successResponse().write(client);

		return true;
	}

	Response::errorResponse().write(client);
	return false;
}