#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "crypto/crypto.h"
#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        namespace Request
        {
            CommandCode ClientPIN::getCommandCode() const
            {
                return authenticatorClientPIN;
            }

            Status parseClientPIN(const CBOR &cbor, std::unique_ptr<Command> &request)
            {
                Serial.println("Parse ClientPIN");

                if (!cbor.is_pair())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }

                std::unique_ptr<ClientPIN> rq(new ClientPIN());

                CBORPair &cborPair = (CBORPair &)cbor;

                // pinUvAuthProtocol (0x01)
                CBOR cborPinUvAuthProtocol = cborPair.find_by_key((uint8_t)ClientPIN::keyPinUvAuthProtocol);
                if (!cborPinUvAuthProtocol.is_uint8())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }

                rq->protocol = cborPinUvAuthProtocol;

                // subCommand (0x02)
                CBOR cborSubCommand = cborPair.find_by_key((uint8_t)ClientPIN::keySubCommand);
                if (!cborSubCommand.is_uint8())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }

                rq->subCommand = (ClientPIN::SubCommand)(uint8_t)cborSubCommand;

                // keyAgreement (0x03)
                CBOR cborKeyAgreement = cborPair.find_by_key((uint8_t)ClientPIN::keyKeyAgreement);

                // pinUvAuthParam (0x04)
                CBOR cborPinUvAuthParam = cborPair.find_by_key((uint8_t)ClientPIN::keyPinUvAuthParam);

                // newPinEnc (0x05)
                CBOR cborNewPinEnc = cborPair.find_by_key((uint8_t)ClientPIN::keyNewPinEnc);

                // pinHashEnc (0x06)
                CBOR cborPinHashEnc = cborPair.find_by_key((uint8_t)ClientPIN::keyPinHashEnc);

                request = std::unique_ptr<Command>(rq.release());

                return CTAP2_OK;
            }

        } // namespace Request

        namespace Response
        {
            CommandCode ClientPIN::getCommandCode() const
            {
                return authenticatorClientPIN;
            }

            Status encode(const ClientPIN *response, std::unique_ptr<CBOR> &cbor)
            {
                // use external buffer?
                std::unique_ptr<CBORPair> cborPair(new CBORPair());

                if (response->publicKey != nullptr)
                {
                    CBORPair cborKey;

                    // kty: EC2 key type
                    cborKey.append(1, 2);

                    // alg: algorithm ECDH-ES+HKDF-256
                    // Note: The COSEAlgorithmIdentifier used is -25 (ECDH-ES + HKDF-256) although this is NOT the algorithm actually used.
                    // Setting this to a different value may result in compatibility issues.
                    cborKey.append(3, -25);

                    // crv: P-256 curve
                    cborKey.append(-1, 1);

                    // x-coordinate as byte string 32 bytes in length
                    CBOR cborX;
                    cborX.encode(response->publicKey->x, 32);
                    cborKey.append(-2, cborX);

                    // y-coordinate as byte string 32 bytes in length
                    CBOR cborY;
                    cborY.encode(response->publicKey->y, 32);
                    cborKey.append(-3, cborY);

                    cborPair->append(0x01, cborKey);
                }

                if (response->pinUvAuthToken != nullptr)
                {
                    // CBOR cborPinUvAuthToken;
                    // cborPinUvAuthToken.encode(response->pinUvAuthToken, );
                    // cborPair->append(0x02, cborPinUvAuthToken);
                }

                if (response->pinRetries != nullptr)
                {
                    cborPair->append(0x03, *response->pinRetries);
                }

                if (response->powerCycleState != nullptr)
                {
                    cborPair->append(0x04, *response->powerCycleState);
                }

                if (response->uvRetries != nullptr)
                {
                    cborPair->append(0x04, *response->uvRetries);
                }

                // finalize the encoding
                cbor = std::unique_ptr<CBOR>(new CBOR(*cborPair));

                return CTAP2_OK;
            }

        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2