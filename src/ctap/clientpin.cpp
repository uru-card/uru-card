#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "crypto/crypto.h"
#include "fido2/ctap/ctap.h"
#include "util/util.h"

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
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                std::unique_ptr<ClientPIN> rq(new ClientPIN());

                CBORPair &cborPair = (CBORPair &)cbor;

                // pinUvAuthProtocol (0x01)
                CBOR cborPinUvAuthProtocol = cborPair.find_by_key((uint8_t)ClientPIN::keyPinUvAuthProtocol);
                if (!cborPinUvAuthProtocol.is_uint8())
                {
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                rq->protocol = cborPinUvAuthProtocol;

                // subCommand (0x02)
                CBOR cborSubCommand = cborPair.find_by_key((uint8_t)ClientPIN::keySubCommand);
                if (!cborSubCommand.is_uint8())
                {
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                rq->subCommand = (ClientPIN::SubCommand)(uint8_t)cborSubCommand;

                // keyAgreement (0x03)
                CBOR cborKeyAgreement = cborPair.find_by_key((uint8_t)ClientPIN::keyKeyAgreement);
                if (!cborKeyAgreement.is_null())
                {
                    if (parsePublicKey(cborKeyAgreement, &rq->publicKey) != CTAP2_OK)
                    {
                        RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                    }
                }

                // pinUvAuthParam (0x04)
                CBOR cborPinUvAuthParam = cborPair.find_by_key((uint8_t)ClientPIN::keyPinUvAuthParam);
                if (!cborPinUvAuthParam.is_null())
                {
                    if (!cborPinUvAuthParam.is_bytestring() || cborPinUvAuthParam.get_bytestring_len() != 16)
                    {
                        RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                    }

                    cborPinUvAuthParam.get_bytestring(rq->pinUvAuthParam);
                }

                // newPinEnc (0x05)
                CBOR cborNewPinEnc = cborPair.find_by_key((uint8_t)ClientPIN::keyNewPinEnc);
                if (!cborNewPinEnc.is_null())
                {
                    if (!cborNewPinEnc.is_bytestring() || cborNewPinEnc.get_bytestring_len() != 64)
                    {
                        RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                    }

                    cborNewPinEnc.get_bytestring(rq->newPinEnc);
                }

                // pinHashEnc (0x06)
                CBOR cborPinHashEnc = cborPair.find_by_key((uint8_t)ClientPIN::keyPinHashEnc);
                if (!cborPinHashEnc.is_null())
                {
                    if (!cborPinHashEnc.is_bytestring() || cborPinHashEnc.get_bytestring_len() != 16)
                    {
                        RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                    }

                    cborPinHashEnc.get_bytestring(rq->pinHashEnc);
                }

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

                bool do_it = false;

                if (response->publicKey != nullptr)
                {
                    CBORPair cborKey;

                    // kty: EC2 key type
                    cborKey.append(1, 2);

                    // alg: algorithm ECDH-ES + HKDF-256
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

                    do_it = true;
                }

                if (response->pinUvAuthToken != nullptr)
                {
                    CBOR cborPinUvAuthToken;
                    cborPinUvAuthToken.encode(response->pinUvAuthToken->value, response->pinUvAuthToken->maxLength);
                    cborPair->append(0x02, cborPinUvAuthToken);

                    do_it = true;
                }

                if (response->pinRetries != nullptr)
                {
                    cborPair->append(0x03, *response->pinRetries);

                    do_it = true;
                }

                if (response->powerCycleState != nullptr)
                {
                    cborPair->append(0x04, *response->powerCycleState);

                    do_it = true;
                }

                if (response->uvRetries != nullptr)
                {
                    cborPair->append(0x04, *response->uvRetries);

                    do_it = true;
                }

                // finalize the encoding
                if (do_it)
                {
                    cbor = std::unique_ptr<CBOR>(new CBOR(*cborPair));
                }

                return CTAP2_OK;
            }

        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2