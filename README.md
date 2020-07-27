# URU Card - Arduino FIDO2 Authenticator

[![Build Status](https://travis-ci.com/uru-card/uru-card.svg?branch=master)](https://travis-ci.com/uru-card/uru-card)
[![Maintainability](https://api.codeclimate.com/v1/badges/f62968e1aea07e03c3d8/maintainability)](https://codeclimate.com/github/uru-card/uru-card/maintainability)

URU Card is an Arduino based FIDO2 Authenticator. This repository contains source code for the firmware.

**Warning**: At the moment BLE transport is supported only by Windows and Chrome browser on Android.

**Warning**: The project is in very early stage of the development

## Getting started

### Required hardware

All you need to start playing with the project is an ESP32 development board and an USB cable.

ESP8266 or AVR based Arduino will not work.

### Required software

To build the firmware you will need [PlatformIO](https://platformio.org/). Follow [the instructions](https://platformio.org/platformio-ide) to install it for your platform.

## Building

Open the project in the PlatformIO IDE. Connect the dev board to your computer then build and upload the firmware from the PlatformIO. All the required libraries will be downloaded automatically.

I'm using the following command in terminal window:

```bash
$ pio run -t upload -t monitor
```

The serial console with the debug information starts.

## Testing

### Pairing (bonding) the device

Open bluetooth settings on your Windows computer or Android phone. Add new device and select "URU Card" from the list of discovered devices.

This step should be performed only once.

### Testing tools

Open the Chrome browser and visit website [webauthn.me](https://webauthn.me/). There you find a number of tools for testing the Authenticator device. The communication between the browser and the authenticator will be displayed in the serial console.

## Contributing

Please read [CONTRIBUTING.md](/CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* [**Andrey Ovcharov**](https://github.com/snakeye) - project owner

See also the list of [contributors](https://github.com/uru-card/uru-card/contributors) who participated in this project.

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](/LICENSE) file for details