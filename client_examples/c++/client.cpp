// Just use the included SerialMap class

#include <string>
#include "SerialMap.h"

int main(int argc, char **argv)
{
    /* 
        create the socket with SSL and open the connection
    */

    // ...

    char payload[256];

    SerialMap<std::string, 2> authentication;
    authentication.put("username", "john miller");
    authentication.put("password", "mysecretpassword");

    int sz = authentication.serialize(payload, 256);

    /*
        write authentication
        _socket.write(payload, sz);

        ...

        read result
        sz = _socket.read(payload, 256);
    */

    SerialMap<std::string, 1> response(payload, sz);

    /* ... */

    SerialMap<std::string, 2> request;
    request.put("action", "setstate");
    request.put("state", "on");

    int sz = request.serialize(payload, 256);

    /*
        write the data to the socket
        _socket.write(payload, sz); 
    */
}