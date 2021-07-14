#include "Cert.h"

BearSSL::X509List Certificate::serverCert(SERVER_CERT);
BearSSL::PrivateKey Certificate::serverPrivateKey(SERVER_KEY);