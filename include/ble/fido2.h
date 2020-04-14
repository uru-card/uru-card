#pragma once

#include <BLECharacteristic.h>

class FIDO2ControlPointCallbacks : public BLECharacteristicCallbacks
{
public:
    virtual void onWrite(BLECharacteristic *pCharacteristic);
};

class FIDO2ControlPointLengthCallbacks : public BLECharacteristicCallbacks
{
public:
    virtual void onRead(BLECharacteristic *pCharacteristic);
};

class FIDO2StatusCallbacks : public BLECharacteristicCallbacks
{
public:
};

class FIDO2ServiceRevisionCallbacks : public BLECharacteristicCallbacks
{
public:
    virtual void onRead(BLECharacteristic *pCharacteristic);
    virtual void onWrite(BLECharacteristic *pCharacteristic);
};
