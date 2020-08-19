# URU Card - Arduino FIDO2 Authenticator

[![Build Status](https://travis-ci.com/uru-card/uru-card.svg?branch=master)](https://travis-ci.com/uru-card/uru-card)
[![Maintainability](https://api.codeclimate.com/v1/badges/f62968e1aea07e03c3d8/maintainability)](https://codeclimate.com/github/uru-card/uru-card/maintainability)

URU Card is an Arduino based FIDO2 Authenticator. This repository contains source code for the firmware.

**Warning**: At the moment BLE transport is supported only by Windows and Chrome browser on Android.

**Warning**: The project is in very early stage of the development

## How to run the project

### ESP32 development board and USB cable

You can use simple and affordable Wroom 32 development board. At the moment advanced features like secure element ATECC508a are not used so there is no need in more sophisticated boards.

![ESP32 development board](/docs/images/uru-card-esp32.jpg)

Connect the board to your computer using USB cable and make sure the UART port is available to your system. Make sure the selected port provides enough current as the ESP32 chips are quite hungry and sensitive to stability of the power source.

ESP8266 or AVR based Arduino will not work.

### Development Environment - PlatformIO

To build the firmware you will need [PlatformIO](https://platformio.org/). Follow [the instructions](https://platformio.org/platformio-ide) to install it for your platform.

## Building

First of all, get a copy of the source code using either `git` command or downloading a zip file.

Open the project in the PlatformIO IDE. Connect the dev board to your computer then build and upload the firmware from the PlatformIO. All the required libraries will be downloaded automatically.

I'm using the following command in terminal window:

```bash
$ pio run -t upload -t monitor
```

The program will start immediately and the serial console will start output of the debug information.

## Testing

### Pairing (bonding) the device

Despite the fact, the FIDO2 protocol supports BLE transport, at the moment is it available only under Windows. So, you will need either a Windows machine or a virtual environment. I am using Oracle Virtual Box and the trial version of Windows 10.

Open bluetooth settings on your Windows computer or Android phone. Add new device and select "URU Card" from the list of discovered devices.

After a few seconds, the device should be paired and appear in the list of available devices.

This step should be performed only once.

### Testing tools

Open the browser in your Windows. Chrome, Firefox and Edge browsers work equally well as they are using the system layer to communicate with the authenticator device.

Visit the website [webauthn.me](https://webauthn.me/). There you find a number of tools for testing the Authenticator device. The communication between the browser and the authenticator will be displayed in the serial console.

## Contributing

Please read [CONTRIBUTING.md](/CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* [**Andrey Ovcharov**](https://github.com/snakeye) - project owner

See also the list of [contributors](https://github.com/uru-card/uru-card/contributors) who participated in this project.

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](/LICENSE) file for details.