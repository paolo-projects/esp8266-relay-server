# IoT Relay Controller

An ESP8266 based relay server meant to be controlled from the local network, written in C++. Its possible applications include controlling water heating, a lamp, or in general anything that has to be wired to the mains through any WiFi capable device, such as a smartphone, a raspberry, a computer, etc.

It operates in AP mode when no WiFi has been configured, receiving the WiFi configuration from the network client and then switching to client mode. When connected to a WiFi, it creates a TCP socket receiving the packets needed to control the relay.

The connection is secured through the use of TLS, by providing a certificate and a private key, and through the use of a basic user/password authentication hardcoded in the flash memory.

It also supports a button for manually switching the relay state.

When in client mode, it broadcasts an UDP packet at a fixed rate to notify the other clients in the network of the service being avaible.

All the configuration parameters such as button, led, relay pins, wifi ap configuration, ssid, users and passwords are defined in the `Config.h` file.

The TLS certificate and key have to be defined in the `Cert.h` file.

Don't forget to put your own configuration inside the `Cert.sample.h` and `Config.sample.h` and rename the files to `Cert.h` and `Config.h` when done.

Can be compiled through Arduino IDE and the proper ESP library.

Further information about the authentication and messaging protocol are provided as comments in the source code.
