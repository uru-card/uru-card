#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "fido2/authenticator/authenticator.h"
#include "fido2/ctap/ctap.h"
#include "util/util.h"

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

            /**
             *
             */
            static Status parsePubKeyCredParams(CBOR &cbor, MakeCredential *request)
            {
                if (!cbor.is_array())
                {
                    RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                }

                CBORArray &cborArray = (CBORArray &)cbor;
                for (auto i = 0; i < cborArray.n_elements(); i++)
                {
                    CBOR param = cborArray.at(i);
                    if (!param.is_pair())
                    {
                        RAISE(Exception(CTAP2_ERR_CBOR_UNEXPECTED_TYPE));
                    }

                    String type;
                    int16_t alg = 0;

                    CBORPair &paramPair = (CBORPair &)param;
                    for (auto j = 0; j < paramPair.n_elements(); j++)
                    {
                        CBOR key = paramPair.key_at(j);
                        CBOR value = paramPair.at(j);

                        String skey;
                        key.get_string(skey);

                        if (skey == "alg" && (value.is_int8() || value.is_int16()))
                        {
                            alg = (int16_t)value;
                        }
                        else if (skey == "type" && value.is_string())
                        {
                            value.get_string(type);
                        }
                    }

                    if (type != "public-key" || alg == 0)
                    {
                        RAISE(Exception(CTAP2_ERR_UNSUPPORTED_ALGORITHM));
                    }

                    request->algorithms.push_back(alg);
                }

                return CTAP2_OK;
            }

            /**
             * @brief
             */
            static Status parseExcludeList(CBOR &cbor, MakeCredential *request)
            {
                if (!cbor.is_array())
                {
                    RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                }

                CBORArray &cborArray = (CBORArray &)cbor;

                for (size_t i = 0; i < cborArray.n_elements(); i++)
                {
                    CBOR param = cborArray.at(i);

                    if (!param.is_pair())
                    {
                        RAISE(Exception(CTAP2_ERR_CBOR_UNEXPECTED_TYPE));
                    }

                    PublicKeyCredentialDescriptor credDescriptor;

                    CBORPair &paramPair = (CBORPair &)param;
                    for (auto j = 0; j < paramPair.n_elements(); j++)
                    {
                        CBOR key = paramPair.key_at(j);
                        CBOR value = paramPair.at(j);

                        String skey;
                        key.get_string(skey);

                        if (skey == "id")
                        {
                            if (!value.is_bytestring())
                            {
                                RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                            }
                            size_t length = value.get_bytestring_len();
                            if (length > credDescriptor.credentialId.maxLength)
                            {
                                RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                            }
                            credDescriptor.credentialId.alloc(length);
                            value.get_bytestring(credDescriptor.credentialId.value);
                        }
                        if (skey == "type")
                        {
                            if (!value.is_string())
                            {
                                RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                            }
                            value.get_string(credDescriptor.type);
                        }
                    }

                    if (credDescriptor.type == "" || credDescriptor.credentialId.length == 0)
                    {
                        RAISE(Exception(CTAP2_ERR_MISSING_PARAMETER));
                    }

                    request->excludeList.push_back(credDescriptor);
                }

                return CTAP2_OK;
            }

            static Status parseExtensions(CBOR &cbor, MakeCredential *request)
            {
                if (!cbor.is_pair())
                {
                    RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                }

                return CTAP2_OK;
            }

            /**
             *
             * @param cbor
             * @param request
             */
            static Status parseOptions(CBOR &cbor, MakeCredential *request)
            {
                if (!cbor.is_pair())
                {
                    RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                }

                CBORPair &paramPair = (CBORPair &)cbor;
                for (auto j = 0; j < paramPair.n_elements(); j++)
                {
                    CBOR key = paramPair.key_at(j);
                    if (!key.is_string())
                    {
                        RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                    }
                    String skey;
                    key.get_string(skey);

                    CBOR value = paramPair.at(j);
                    if (skey == "rk")
                    {
                        request->options.rk = (bool)value;
                    }
                    else if (skey == "uv")
                    {
                        request->options.uv = (bool)value;
                    }
                    else if (skey == "up")
                    {
                        request->options.up = (bool)value;
                    }
                }

                return CTAP2_OK;
            }

            Status parseMakeCredential(const CBOR &cbor, std::unique_ptr<Command> &request)
            {
                Serial.println("Parse MakeCredential");

                if (!cbor.is_pair())
                {
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                CBORPair &cborPair = (CBORPair &)cbor;

                std::unique_ptr<MakeCredential> rq(new MakeCredential());

                // clientDataHash (0x01)
                // Hash of the ClientData contextual binding specified by host.
                CBOR cborClientDataHash = cborPair.find_by_key((uint8_t)MakeCredential::keyclientDataHash);
                if (cborClientDataHash.is_null())
                {
                    RAISE(Exception(CTAP2_ERR_MISSING_PARAMETER));
                }

                if (!cborClientDataHash.is_bytestring())
                {
                    RAISE(Exception(CTAP1_ERR_INVALID_PARAMETER));
                }

                if (cborClientDataHash.get_bytestring_len() != 32)
                {
                    RAISE(Exception(CTAP1_ERR_INVALID_LENGTH));
                }

                cborClientDataHash.get_bytestring(rq->clientDataHash);

                // rp (0x02)
                // This PublicKeyCredentialRpEntity data structure describes a Relying Party
                // with which the new public key credential will be associated.
                CBOR cborRelyingParty = cborPair.find_by_key((uint8_t)MakeCredential::keyRp);
                if (cborRelyingParty.is_null())
                {
                    RAISE(Exception(CTAP2_ERR_MISSING_PARAMETER));
                }

                if (!cborRelyingParty.is_pair())
                {
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                if (parseRpEntity(cborRelyingParty, &rq->rp) != CTAP2_OK)
                {
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                // user (0x03)
                // This PublicKeyCredentialUserEntity data structure describes the user account
                // to which the new public key credential will be associated at the RP.
                CBOR cborUser = cborPair.find_by_key((uint8_t)MakeCredential::keyUser);
                if (cborUser.is_null())
                {
                    RAISE(Exception(CTAP2_ERR_MISSING_PARAMETER));
                }

                if (!cborUser.is_pair())
                {
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                if (parseUserEntity(cborUser, &rq->user) != CTAP2_OK)
                {
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                // pubKeyCredParams (0x04)
                // A sequence of CBOR maps consisting of pairs of PublicKeyCredentialType and cryptographic algorithm
                CBOR cborKeyCredParms = cborPair.find_by_key((uint8_t)MakeCredential::keyPubKeyCredParams);
                if (cborKeyCredParms.is_null())
                {
                    RAISE(Exception(CTAP2_ERR_MISSING_PARAMETER));
                }

                if (parsePubKeyCredParams(cborKeyCredParms, rq.get()) != CTAP2_OK)
                {
                    RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                }

                // excludeList (0x05)
                // A sequence of PublicKeyCredentialDescriptor structures
                CBOR cborExcludeList = cborPair.find_by_key((uint8_t)MakeCredential::keyExcludeList);
                if (!cborExcludeList.is_null())
                {
                    if (parseExcludeList(cborExcludeList, rq.get()) != CTAP2_OK)
                    {
                        RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                    }
                }

                // extensions (0x06)
                // Parameters to influence authenticator operation
                CBOR cborExtensions = cborPair.find_by_key((uint8_t)MakeCredential::keyExtensions);
                if (!cborExtensions.is_null())
                {
                    if (parseExtensions(cborExtensions, rq.get()) != CTAP2_OK)
                    {
                        RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                    }
                }

                // options (0x07)
                // Parameters to influence authenticator operation
                CBOR cborOptions = cborPair.find_by_key((uint8_t)MakeCredential::keyOptions);
                if (!cborOptions.is_null())
                {
                    if (parseOptions(cborOptions, rq.get()) != CTAP2_OK)
                    {
                        RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                    }
                }

                // pinUvAuthParam (0x08)
                // First 16 bytes of HMAC-SHA-256 of clientDataHash using pinUvAuthToken which platform got from the authenticator
                CBOR cborPinUvAuthParam = cborPair.find_by_key((uint8_t)MakeCredential::keyPinUvAuthParam);
                if (!cborPinUvAuthParam.is_null())
                {
                    if (!cborPinUvAuthParam.is_bytestring() || cborPinUvAuthParam.get_bytestring_len() > 16)
                    {
                        RAISE(Exception(CTAP2_ERR_INVALID_CBOR));
                    }

                    rq->pinUvAuthParam = std::unique_ptr<FixedBuffer16>(new FixedBuffer16());

                    cborPinUvAuthParam.get_bytestring(rq->pinUvAuthParam->value);
                    rq->pinUvAuthParam->length = cborPinUvAuthParam.get_bytestring_len();
                }

                // pinUvAuthProtocol (0x09)
                // PIN/UV protocol version chosen by the client
                CBOR cborPinUvAuthProtocol = cborPair.find_by_key((uint8_t)MakeCredential::keyPinUvAuthProtocol);

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
                // use external buffer?
                std::unique_ptr<CBORPair> cborPair(new CBORPair());

                // fmt (0x01)
                cborPair->append(0x01, "packed");

                // authData (0x02)
                CBOR cborAuthData;
                size_t encodeSize = response->authenticatorData.flags.f.attestationData ? sizeof(AuthenticatorData) : sizeof(AuthenticatorData) - sizeof(AttestedCredentialData);

                cborAuthData.encode((uint8_t *)&response->authenticatorData, encodeSize);
                cborPair->append(0x02, cborAuthData);

                // attStmt (0x03)
                CBORPair cborAttStmt;
                cborAttStmt.append("alg", -7);

                CBOR cborSignature;
                cborSignature.encode(response->signature, response->signatureSize);
                cborAttStmt.append("sig", cborSignature);

                CBORArray cborCertificates;

                CBOR cborCertificate;
                cborCertificate.encode(FIDO2::Authenticator::certificate, FIDO2::Authenticator::certificateSize);

                cborCertificates.append(cborCertificate);

                cborAttStmt.append("x5c", cborCertificates);

                cborPair->append(0x03, cborAttStmt);

                // finalize the encoding
                cbor = std::unique_ptr<CBOR>(new CBOR(*cborPair));

                return CTAP2_OK;
            }
        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2