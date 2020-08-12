#include <memory>

#include <Arduino.h>

#include <YACL.h>

#include "fido2/ctap/ctap.h"
#include "util.h"

namespace FIDO2
{
    namespace CTAP
    {
        namespace Request
        {
            CommandCode GetAssertion::getCommandCode() const
            {
                return authenticatorGetAssertion;
            }

            Status parseAllowList(CBOR &cbor, GetAssertion *request)
            {
                if (!cbor.is_array())
                {
                    return CTAP1_ERR_INVALID_PARAMETER;
                }

                CBORArray &cborArray = (CBORArray &)cbor;

                for (size_t i = 0; i < cborArray.n_elements(); i++)
                {
                    CBOR param = cborArray.at(i);

                    if (!param.is_pair())
                    {
                        return CTAP2_ERR_CBOR_UNEXPECTED_TYPE;
                    }

                    std::unique_ptr<PublicKeyCredentialDescriptor> cd(new PublicKeyCredentialDescriptor());

                    //
                    CBOR cborType = param.find_by_key("type");
                    if (!cborType.is_string())
                    {
                        return CTAP2_ERR_INVALID_CBOR;
                    }

                    cborType.get_string(cd->type);

                    //
                    CBOR cborId = param.find_by_key("id");
                    if(!cborId.is_bytestring() || cborId.get_bytestring_len() > CREDENTIAL_ID_LENGTH)
                    {
                        return CTAP2_ERR_INVALID_CBOR;
                    }

                    cborId.get_bytestring(cd->credentialId);

                    request->allowList.push_back(std::move(cd));
                }

                return CTAP2_OK;
            }

            Status parseExtensions(CBOR &cbor, GetAssertion *request)
            {
                if (!cbor.is_pair())
                {
                    return CTAP1_ERR_INVALID_PARAMETER;
                }

                return CTAP2_OK;
            }

            Status parseOptions(CBOR &cbor, GetAssertion *request)
            {
                if (!cbor.is_pair())
                {
                    return CTAP1_ERR_INVALID_PARAMETER;
                }

                return CTAP2_OK;
            }

            Status parseGetAssertion(const CBOR &cbor, std::unique_ptr<Command> &request)
            {
                Serial.println("Parse GetAssertion");

                if (!cbor.is_pair())
                {
                    return CTAP2_ERR_INVALID_CBOR;
                }

                CBORPair &cborPair = (CBORPair &)cbor;

                std::unique_ptr<GetAssertion> rq(new GetAssertion());

                // rpId (0x01)
                CBOR cborRpId = cborPair.find_by_key((uint8_t)GetAssertion::keyRpId);
                if (cborRpId.is_null())
                {
                    return CTAP2_ERR_MISSING_PARAMETER;
                }

                if (!cborRpId.is_string())
                {
                    return CTAP1_ERR_INVALID_PARAMETER;
                }

                cborRpId.get_string(rq->rpId);

                // clientDataHash (0x02)
                CBOR cborClientDataHash = cborPair.find_by_key((uint8_t)GetAssertion::keyClientDataHash);
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

                // allowList (0x03)
                CBOR cborAllowList = cborPair.find_by_key((uint8_t)GetAssertion::keyAllowList);
                if (!cborAllowList.is_null())
                {
                    if (parseAllowList(cborAllowList, rq.get()) != CTAP2_OK)
                    {
                        return CTAP2_ERR_INVALID_CBOR;
                    }
                }

                // extensions (0x04)
                CBOR cborExtensions = cborPair.find_by_key((uint8_t)GetAssertion::keyExtensions);
                if (!cborExtensions.is_null())
                {
                    if (parseExtensions(cborExtensions, rq.get()) != CTAP2_OK)
                    {
                        return CTAP2_ERR_INVALID_CBOR;
                    }
                }

                // options (0x05)
                CBOR cborOptions = cborPair.find_by_key((uint8_t)GetAssertion::keyOptions);
                if (!cborOptions.is_null())
                {
                    if (parseOptions(cborOptions, rq.get()) != CTAP2_OK)
                    {
                        return CTAP2_ERR_INVALID_CBOR;
                    }
                }

                // pinUvAuthParam (0x06)
                CBOR cborPinUvAuthParam = cborPair.find_by_key((uint8_t)GetAssertion::keyPinUvAuthParam);
                if (!cborPinUvAuthParam.is_null())
                {

                }

                // pinUvAuthProtocol (0x07)
                CBOR cborPinUvAuthProtocol = cborPair.find_by_key((uint8_t)GetAssertion::keyPinUvAuthProtocol);
                if (!cborPinUvAuthProtocol.is_null())
                {

                }

                request = std::unique_ptr<Command>(rq.release());

                return CTAP2_OK;
            }
        } // namespace Request

        namespace Response
        {
            CommandCode GetAssertion::getCommandCode() const
            {
                return authenticatorGetAssertion;
            }

            Status encode(const GetAssertion *response, std::unique_ptr<CBOR> &cbor)
            {
                // use external buffer?
                std::unique_ptr<CBORPair> cborPair(new CBORPair());

                // credential (0x01)

                // authData (0x02)
                CBOR cborAuthData;
                cborAuthData.encode((uint8_t*)&response->authenticatorData, sizeof(AuthenticatorData) - sizeof(AttestedCredentialData));
                cborPair->append(0x02, cborAuthData);

                // signature (0x03)
                CBOR cborSignature;
                cborSignature.encode(response->signature, response->signatureSize);
                cborPair->append(0x03, cborSignature);

                // user (0x04)

                // numberOfCredentials (0x05)

                // userSelected (0x06)

                // finalize the encoding
                cbor = std::unique_ptr<CBOR>(new CBOR(*cborPair));

                return CTAP2_OK;
            }

        } // namespace Response
    }     // namespace CTAP
} // namespace FIDO2