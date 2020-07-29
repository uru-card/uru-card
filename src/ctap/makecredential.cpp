#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"
#include <memory>

namespace FIDO2
{
    namespace CTAP
    {
        CommandCode Request::MakeCredential::getCommandCode() const
        {
            return authenticatorMakeCredential;
        }

        Status parseRequestMakeCredential(const uint8_t *data, const size_t len, std::unique_ptr<Command> &request)
        {
            Serial.println("Parse MakeCredential");

            if (!CBOR::is_pair(data))
            {
                return CTAP2_ERR_INVALID_CBOR;
            }

            std::unique_ptr<Request::MakeCredential> rq(new Request::MakeCredential());

            CBOR cbor = CBOR((uint8_t *)data, len, true);

            // clientDataHash (0x01)
            // Hash of the ClientData contextual binding specified by host.
            CBOR cborClientDataHash = cbor.find_by_key((uint8_t)Request::MakeCredential::keyclientDataHash);
            if (cborClientDataHash.is_null())
            {
                return CTAP2_ERR_MISSING_PARAMETER;
            }

            if (cborClientDataHash.is_bytestring())
            {
                if (cborClientDataHash.get_bytestring_len() != 32)
                {
                    return CTAP1_ERR_INVALID_LENGTH;
                }

                cborClientDataHash.get_bytestring(rq->clientDataHash);
            }

            // rp (0x02)
            // This PublicKeyCredentialRpEntity data structure describes a Relying Party
            // with which the new public key credential will be associated.
            CBOR cborRelyingParty = cbor.find_by_key((uint8_t)Request::MakeCredential::keyRp);
            if (cborRelyingParty.is_null())
            {
                return CTAP2_ERR_MISSING_PARAMETER;
            }

            if (!cborRelyingParty.is_pair())
            {
                return CTAP2_ERR_INVALID_CBOR;
            }

            if (parseRpEntity(cborRelyingParty, &rq->rp) != CTAP2_OK)
            {
                return CTAP2_ERR_INVALID_CBOR;
            }

            // user (0x03)
            // This PublicKeyCredentialUserEntity data structure describes the user account
            // to which the new public key credential will be associated at the RP.
            CBOR cborUser = cbor.find_by_key((uint8_t)Request::MakeCredential::keyUser);
            if (cborUser.is_null())
            {
                return CTAP2_ERR_MISSING_PARAMETER;
            }

            if (!cborUser.is_pair())
            {
                return CTAP2_ERR_INVALID_CBOR;
            }

            if (parseUserEntity(cborUser, &rq->user) != CTAP2_OK)
            {
                return CTAP2_ERR_INVALID_CBOR;
            }

            // pubKeyCredParams (0x04)
            // A sequence of CBOR maps consisting of pairs of PublicKeyCredentialType and cryptographic algorithm
            CBOR cborKeyCredParms = cbor.find_by_key((uint8_t)Request::MakeCredential::keyPubKeyCredParams);
            if (cborKeyCredParms.is_null())
            {
                return CTAP2_ERR_MISSING_PARAMETER;
            }

            // excludeList (0x05)
            // A sequence of PublicKeyCredentialDescriptor structures
            CBOR cborExcludeList = cbor.find_by_key((uint8_t)Request::MakeCredential::keyExcludeList);

            // extensions (0x06)
            // Parameters to influence authenticator operation
            CBOR cborExtensions = cbor.find_by_key((uint8_t)Request::MakeCredential::keyExtensions);

            // options (0x07)
            // Parameters to influence authenticator operation
            CBOR cborOptions = cbor.find_by_key((uint8_t)Request::MakeCredential::keyOptions);

            // pinUvAuthParam (0x08)
            // First 16 bytes of HMAC-SHA-256 of clientDataHash using pinUvAuthToken which platform got from the authenticator
            CBOR cborPinUvAuthParam = cbor.find_by_key((uint8_t)Request::MakeCredential::keyPinUvAuthParam);

            // pinUvAuthProtocol (0x09)
            // PIN/UV protocol version chosen by the client
            CBOR cborPinUvAuthProtocol = cbor.find_by_key((uint8_t)Request::MakeCredential::keyPinUvAuthProtocol);

            request = std::unique_ptr<Command>(rq.release());

            return CTAP2_OK;
        }

        CommandCode Response::MakeCredential::getCommandCode() const
        {
            return authenticatorMakeCredential;
        }

        Status encodeResponse(const Response::MakeCredential *response, uint8_t *data, size_t &length)
        {
            // use external buffer?
            CBORPair cbor = CBORPair();

            // finalize the encoding
            data[0] = CTAP2_OK;
            memcpy(data + 1, cbor.to_CBOR(), cbor.length());
            length = cbor.length() + 1;

            return CTAP2_OK;
        }
    } // namespace CTAP
} // namespace FIDO2