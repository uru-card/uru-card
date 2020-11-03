#include <Arduino.h>

#include <SparkFun_ATECCX08a_Arduino_Library.h>

#include "config.h"
#include "crypto/crypto.h"

#include "util/util.h"

#if defined(HARDWARE_CRYPTO)

namespace Crypto
{
    ATECCX08A atecc;

    bool init()
    {
        if (!atecc.begin())
            return false;

        Serial.println("ATECC init done");

        atecc.readConfigZone(false);

        Serial.print("Serial Number: \t");
        for (int i = 0; i < 9; i++)
        {
            if ((atecc.serialNumber[i] >> 4) == 0)
                Serial.print("0"); // print preceeding high nibble if it's zero
            Serial.print(atecc.serialNumber[i], HEX);
        }
        Serial.println();

        Serial.print("Rev Number: \t");
        for (int i = 0; i < 4; i++)
        {
            if ((atecc.revisionNumber[i] >> 4) == 0)
                Serial.print("0"); // print preceeding high nibble if it's zero
            Serial.print(atecc.revisionNumber[i], HEX);
        }
        Serial.println();

        return true;
    }

    bool isConfigured()
    {
        return atecc.configLockStatus && atecc.dataOTPLockStatus && atecc.slot0LockStatus;
    }

    void configure()
    {
        Serial.println();
        Serial.println("Configuration beginning.");

        Serial.print("Write Config: \t");
        if (atecc.writeConfigSparkFun() == true)
            Serial.println("Success!");
        else
            Serial.println("Failure.");

        Serial.print("Lock Config: \t");
        if (atecc.lockConfig() == true)
            Serial.println("Success!");
        else
            Serial.println("Failure.");

        Serial.print("Key Creation: \t");
        if (atecc.createNewKeyPair() == true)
            Serial.println("Success!");
        else
            Serial.println("Failure.");

        Serial.print("Lock Data-OTP: \t");
        if (atecc.lockDataAndOTP() == true)
            Serial.println("Success!");
        else
            Serial.println("Failure.");

        Serial.print("Lock Slot 0: \t");
        if (atecc.lockDataSlot0() == true)
            Serial.println("Success!");
        else
            Serial.println("Failure.");

        Serial.println("Configuration done.");
        Serial.println();
    }

    namespace SHA256
    {
        bool hash(const uint8_t *data, const size_t length, uint8_t *sha)
        {
            atecc.wakeUp();
            atecc.sha256((uint8_t *)data, length, sha);
            atecc.idleMode();

            Serial.println(" * SHA256:");
            serialDumpBuffer(sha, 32);

            return true;
        }
    } // namespace SHA256

    namespace ECDSA
    {
        void getPublicKey(PublicKey *publicKey)
        {
            atecc.wakeUp();
            atecc.generatePublicKey();
            atecc.idleMode();

            memcpy(publicKey, atecc.publicKey64Bytes, 64);

            Serial.println(" * Public Key:");
            serialDumpBuffer(publicKey->x, 32);
            serialDumpBuffer(publicKey->y, 32);
        }

        void sign(const uint8_t *hash, uint8_t *signature)
        {
            atecc.wakeUp();
            atecc.createSignature((uint8_t *)hash);
            atecc.idleMode();

            memcpy(signature, atecc.signature, 64);

            Serial.println(" * Signature:");
            serialDumpBuffer(signature, 64);
        }
    } // namespace ECDSA

} // namespace Crypto

#endif