// Just use the included SerialMap class

#include <string>
#include "SerialMap.h"

int main(int argc, char **argv)
{
    /* 
        create the socket with SSL and open the connection the C way
        or use an external library such as boost
    */

    // ...

    char payload[256];

    SerialMap<std::string, 2> authentication;
    authentication.put("username", "john miller");
    authentication.put("password", "mysecretpassword");

    int sz = authentication.serialize(payload, 256);

    /*
        SSL_write(socket, payload, sz);

        ...

        read result
        sz = SSL_read(socket, payload, 256);
    */

    SerialMap<std::string, 1> response(payload, sz);

    /* ... */

    SerialMap<std::string, 2> request;
    request.put("action", "setstate");
    request.put("state", "on");

    int sz = request.serialize(payload, 256);

    /*
        SSL_write(socket, payload, sz);
    */
}