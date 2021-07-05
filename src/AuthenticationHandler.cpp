#include "AuthenticationHandler.h"

AuthenticationHandler::AuthenticationHandler(const String &username,
											 const String &password)
	: username(username), password(password) {}

AuthenticationHandler::AuthenticationHandler(const char *username,
											 const char *password)
	: username(username), password(password) {}

bool AuthenticationHandler::authenticate(Stream &client)
{
	auto data = std::unique_ptr<char[]>(new char[1024]);
	size_t read = client.readBytes(data.get(), 1024);

	ActionMap authentication(data.get(), read);

	if (authentication.has("username") &&
		authentication.has("password"))
	{
		return *authentication.get("username") == this->username && *authentication.get("password") == this->password;
	}

	return false;
}