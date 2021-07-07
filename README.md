# IoT Relay Controller

# Description

An ESP8266 based relay server meant to be controlled from the local network, written in C++. Its possible applications include controlling water heating, a lamp, or in general anything that has to be wired to the mains through any WiFi capable device, such as a smartphone, a raspberry, a computer, etc.

It operates in AP mode when no WiFi has been configured, receiving the WiFi configuration from the network client and then switching to client mode. When connected to a WiFi, it opens a TCP socket receiving the data needed to control the relay.

The connection is secured through the use of TLS, by providing a certificate and a private key, and through the use of a basic user/password authentication hardcoded in the device memory.

It also supports a button for manually switching the relay state, which can be wired to a digital pin and configured into the `Config.h` file.

When in client mode, it broadcasts an UDP packet at a fixed rate to notify the other clients in the network of the service being avaible, so the client can use this feature to auto-discover the service and IP address in the local network.

# Configuration

All the configuration parameters such as button, led, relay pins, wifi ap configuration, ssid, users and passwords are defined in the `Config.h` file.

The TLS certificate and key have to be defined in the `Cert.h` file.

A `Cert.sample.h` and a `Config.sample.h` are provided, into which you have to put your own configuration and rename the files to `Cert.h` and `Config.h` when done.

Can be compiled through Arduino IDE and the proper ESP library, by renaming the `relay_controller.cpp` to `relay_controller.ino` and fixing the imports, or through PlatformIO natively.

# The clients

The SerialMap class, which is at the base of the protocol, has been ported into a few programming languages, and you will find a few examples of its usage into the `client_examples` folder. The Java example can be easily put into an Android project.

# Further developments

The project can be easily adapted to support more functions / commands. Support for more relays can be easily implemented by adding an `id` key to the action sent by the client and a few lines of code into the `RelayManager` server class. Relays aside, by including the required logic and electronics, the server can be used for remote-controlling motors, LEDs, doors, etc.
