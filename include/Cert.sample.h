#ifndef CERT_H
#define CERT_H

// The server's private key which must be kept secret
const char SERVER_KEY[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
<YOUR PRIVATE KEY HERE>
-----END PRIVATE KEY-----
)EOF";

// The server's public certificate which must be shared
const char SERVER_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
<YOUR CERTIFICATE HERE>
-----END CERTIFICATE-----
)EOF";

#endif
