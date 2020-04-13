#pragma once

#include <BLECharacteristic.h>

class FIDO2ControlPointCallbacks : public BLECharacteristicCallbacks
{
};

class FIDO2ControlPointLengthCallbacks : public BLECharacteristicCallbacks
{
};

class FIDO2StatusCallbacks : public BLECharacteristicCallbacks
{
};

class FIDO2ServiceRevisionCallbacks : public BLECharacteristicCallbacks
{
};
