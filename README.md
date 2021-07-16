# IoT Relay Controller

# Description

An ESP8266 based relay server meant to be controlled from the local network, written in C++. Its possible applications include controlling water heating, a lamp, or in general anything that has to be wired to the mains through any WiFi capable device, such as a smartphone, a raspberry, a computer, etc.

It operates in AP mode when no WiFi has been configured, receiving the WiFi configuration from the network client and then switching to client mode. When connected to a WiFi, it opens a TCP socket receiving the data needed to control the relay.

The connection is secured through the use of TLS, by providing a certificate and a private key, and through the use of a basic user/password authentication hardcoded in the device memory.

It also supports a button for manually switching the relay state, which can be wired to a digital pin and configured into the `Config.h` file.

When in client mode, it broadcasts an UDP packet at a fixed rate to notify the other clients in the network of the service being available, so the client can use this feature to auto-discover the service and IP address in the local network. Although the code for this feature is not provided in the client examples, implementing it just needs that the client listens on a UDP port and checks that the content of the packet matches. The TLS used for TCP communications, with the certificate check implemented in the client, should prevent connecting to a fake service pretending to be the real one thus adding an additional layer of security.

# Dependencies

The Server code has been refactored and placed into an external library dependency. To clone the repository with the needed library you just need to add the `--recursive` options.

# Configuration

All the configuration parameters such as button, led, relay pins, wifi ap configuration, ssid, users and passwords are defined in the `Config.h` file.

The TLS certificate and key have to be defined in the `Cert.h` file.

A `Cert.sample.h` and a `Config.sample.h` are provided, into which you have to put your own configuration and rename the files to `Cert.h` and `Config.h` when done.

Can be compiled natively through VSCode and the Platform.io extension, which makes configuration, testing and project maintenance easy.

# The clients

The SerialMap class, which is at the base of the protocol, has been ported into a few programming languages, and you will find a few examples of its usage into the [/client_examples](/client_examples) folder. The [Java example](/client_examples/java) can be easily put into an Android project.

# Further developments

The project can be easily adapted to support more functions / commands. Support for more relays can be easily implemented by adding an `id` key to the action sent by the client and a few lines of code into the `RelayManager` server class. Relays aside, by including the required logic and electronics, the server can be used for remote-controlling motors, LEDs, doors, etc.
