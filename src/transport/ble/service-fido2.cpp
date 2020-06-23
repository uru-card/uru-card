#include <Arduino.h>

#include "transport/ble/service-fido2.h"

#include "ctap/buffer.h"
#include "util.h"

#define FIDO2_CONTROL_POINT_LENGTH 32

/**
 * fidoControlPoint is a write-only command buffer.
 */
void FIDO2ControlPointCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string value = pCharacteristic->getValue();
    if (value.length() == 0)
    {
        return;
    }

    // A frame is divided into an initialization fragment and zero or more continuation fragments.
    uint8_t cmd = value[0];
    if (cmd >= 0x80)
    {
        // The start of an initialization fragment is indicated by setting the high bit in the first byte.

        // The subsequent two bytes indicate the total length of the frame, in big-endian order.
        uint16_t length = FROM_BIG_ENDIAN(value[1], value[2]);
        // if(length > ) {
        // }

        // The first maxLen - 3 bytes of data follow.
        CTAP::inputBuffer.reset();

        CTAP::inputBuffer.setCmd(cmd);
        CTAP::inputBuffer.setLength(length);

        CTAP::inputBuffer.append();
    }
    else
    {
        // Continuation fragments begin with a sequence number, beginning at 0, implicitly with the high bit cleared.
        // The sequence number must wraparound to 0 after reaching the maximum sequence number of 0x7f.

        CTAP::inputBuffer.append();
    }
}

/**
 * fidoControlPointLength defines the maximum size in bytes of a single write request to fidoControlPoint.
 * This value SHALL be between 20 and 512.
 */
void FIDO2ControlPointLengthCallbacks::onRead(BLECharacteristic *pCharacteristic)
{
    uint8_t value[] = {
        (FIDO2_CONTROL_POINT_LENGTH >> 8) & 0xFF,
        FIDO2_CONTROL_POINT_LENGTH & 0xFF,
    };
    pCharacteristic->setValue(value, sizeof(value));
}

/**
 * fidoServiceRevisionBitfield defines the revision of the FIDO Service.
 * The value is a bit field which each bit representing a version.
 */
void FIDO2ServiceRevisionCallbacks::onRead(BLECharacteristic *pCharacteristic)
{
    // A device that only supports FIDO2 Rev 1 will only have a fidoServiceRevisionBitfield
    // characteristic of length 1 with value 0x20.
    uint8_t value[] = {0x20};
    pCharacteristic->setValue(value, 1);
}

void FIDO2ServiceRevisionCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
}
