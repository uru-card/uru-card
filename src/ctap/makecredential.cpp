#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"
#include <memory>

namespace FIDO2
{
    namespace CTAP
    {
        namespace Request
        {
            CommandCode MakeCredential::getCommandCode() const
            {
                return authenticatorMakeCredential;
            }

            Status parseMakeCredential(const CBOR &cbor, std::unique_ptr<Command> &request)
            {
                Serial.println("Parse MakeCredential");

                if (!cbor.is_pair())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }

                CBORPair &cborPair = (CBORPair &)cbor;

                std::unique_ptr<Request::MakeCredential> rq(new Request::MakeCredential());

                // clientDataHash (0x01)
                // Hash of the ClientData contextual binding specified by host.
                CBOR cborClientDataHash = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyclientDataHash);
                if (cborClientDataHash.is_null())
                {
                    return CTAP2_ERR_MISSING_PARAMETER;
                }

                if (!cborClientDataHash.is_bytestring())
                {
                    return CTAP1_ERR_INVALID_PARAMETER;
                }

                if (cborClientDataHash.get_bytestring_len() != 32)
                {
                    return CTAP1_ERR_INVALID_LENGTH;
                }

                cborClientDataHash.get_bytestring(rq->clientDataHash);

                // rp (0x02)
                // This PublicKeyCredentialRpEntity data structure describes a Relying Party
                // with which the new public key credential will be associated.
                CBOR cborRelyingParty = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyRp);
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
                CBOR cborUser = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyUser);
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
                CBOR cborKeyCredParms = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyPubKeyCredParams);
                if (cborKeyCredParms.is_null())
                {
                    return CTAP2_ERR_MISSING_PARAMETER;
                }

                if (!cborKeyCredParms.is_array())
                {
                    return CTAP1_ERR_INVALID_PARAMETER;
                }

                // excludeList (0x05)
                // A sequence of PublicKeyCredentialDescriptor structures
                CBOR cborExcludeList = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyExcludeList);
                if (!cborExcludeList.is_null())
                {
                    if (!cborExcludeList.is_array())
                    {
                        return CTAP1_ERR_INVALID_PARAMETER;
                    }
                }

                // extensions (0x06)
                // Parameters to influence authenticator operation
                CBOR cborExtensions = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyExtensions);
                if (!cborExtensions.is_null())
                {
                    if (!cborExtensions.is_array())
                    {
                        return CTAP1_ERR_INVALID_PARAMETER;
                    }
                }

                // options (0x07)
                // Parameters to influence authenticator operation
                CBOR cborOptions = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyOptions);
                if (!cborOptions.is_null())
                {
                    if (!cborOptions.is_pair())
                    {
                        return CTAP1_ERR_INVALID_PARAMETER;
                    }
                }

                // pinUvAuthParam (0x08)
                // First 16 bytes of HMAC-SHA-256 of clientDataHash using pinUvAuthToken which platform got from the authenticator
                CBOR cborPinUvAuthParam = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyPinUvAuthParam);

                // pinUvAuthProtocol (0x09)
                // PIN/UV protocol version chosen by the client
                CBOR cborPinUvAuthProtocol = cborPair.find_by_key((uint8_t)Request::MakeCredential::keyPinUvAuthProtocol);

                request = std::unique_ptr<Command>(rq.release());

                return CTAP2_OK;
            }

        } // namespace Request

        namespace Response
        {
            CommandCode MakeCredential::getCommandCode() const
            {
                return authenticatorMakeCredential;
            }

            Status encode(const MakeCredential *response, std::unique_ptr<CBOR> &cbor)
            {
                return CTAP2_OK;
            }
        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2