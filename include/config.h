#pragma once

#define DEVICE_NAME "URU Card"

// defines the maximum size in bytes of a single write request to fidoControlPoint. This value SHALL be between 20 and 512
#define FIDO2_CONTROL_POINT_LENGTH 64

// Maximum message size supported by the authenticator.
#define FIDO2_MAX_MSG_SIZE 2048

// Credential ID Length supported by the authenticator.
#define CREDENTIAL_ID_LENGTH 16

// Enable SSD1306 OLED Display
#define DISPLAY_ENABLED

// Enable MPR121 based touch keyboard
#define KEYBOARD_ENABLED

// Enable FPC1020A UART fingerprint sensor module
// #define FPC1020A_ENABLED 1

// Enable Hardware Crypto using ATECCx08A
#define HARDWARE_CRYPTO