#ifndef CERT_H
#define CERT_H

#include <ESP8266WiFi.h>

namespace Certificate
{ // The server's private key which must be kept secret
    const char SERVER_KEY[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----

-----END PRIVATE KEY-----
)EOF";

    // The server's public certificate which must be shared
    const char SERVER_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
}

#endif
