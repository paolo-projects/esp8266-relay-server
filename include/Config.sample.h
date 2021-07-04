#ifndef CONFIG_H
#define CONFIG_H

// Pins
#define MANUAL_BUTTON_PIN <THE BUTTON PIN HERE e.g. D5>
#define LED_PIN <THE LED PIN HERE e.g. D6>
#define RELAY_PIN <THE RELAY PIN HERE e.g. D1>

// Time during which the button state is ignored after a press
#define BUTTON_DEBOUNCE_MS 1500
#define MANUAL_BUTTON_PRESSED_STATE LOW

// Relay ON-OFF states
#define RELAY_PIN_ON HIGH
#define RELAY_PIN_OFF LOW

// Led ON-OFF states
#define LED_PIN_ON HIGH
#define LED_PIN_OFF LOW

// Timeout after which the device switched to AP mode if connection fails
#define WLAN_CONN_TIMEOUT 15 // Seconds
#define WLAN_HOSTNAME "whcontrol.local"

// Access point
#define AP_SSID "WaterHeatingControl"
#define AP_PASS <THE WIFI PASSWORD HERE>
#define AP_CHANNEL 11
#define AP_HIDDEN true
#define AP_MAX_CONN 2
// The IP address of the server when in AP mode
#define AP_IP_ADDR IPAddress(192, 168, 7, 5)
#define AP_GATEWAY IPAddress(192, 168, 7, 1)
#define AP_SUBNET IPAddress(255, 255, 255, 0)

// AP Server
// The AP server port
#define AP_SERVER_PORT 5597
// The AP server authentication
#define AP_AUTH_USER <THE AP MODE USERNAME HERE>
#define AP_AUTH_PASS <THE AP MODE PASSWORD HERE>
#define TIMEOUT 3000

// Main server
// The server port
#define SERVER_PORT 4879
// The UDP broadcast port
#define UDP_BROADCAST_PORT 5589
// The UDP broadcast rate in milliseconds
#define UDP_BROADCAST_RATE 2000
// The packet broadcasted through UDP to notify clients of the server being available
// You can put any kind of data here, a string, random bytes, and the client should identify
// the server from this packet
#define UDP_BROADCAST_PACKET                                                                    \
    {                                                                                           \
        0x0d, 0x2e, 0x82, 0xdf, 0xfd, 0xf7, 0x4f, 0x4a, 0x0d, 0x6d, 0xd8, 0x8a, 0xc1, 0xb2,     \
            0x8c, 0x9e, 0x4c, 0x28, 0x84, 0xa3, 0x19, 0x18, 0x58, 0xc9, 0xcd, 0x91, 0x98, 0x58, \
            0xc2, 0xdc, 0x45, 0xc4                                                              \
    }

// The main server authentication
#define AUTHENTICATION_USERNAME <THE CLIENT MODE USERNAME HERE>
#define AUTHENTICATION_PASSWORD <THE CLIENT MODE PASSWORD HERE>

#endif