#include <memory>

#include <Arduino.h>

#if defined(FIDO2_TRANSPORT_USB)

#include "config.h"
#include "fido2/authenticator/authenticator.h"
#include "fido2/ctap/ctap.h"
#include "fido2/transport/usb/service.h"
#include "fido2/transport/usb/buffer.h"
#include "display/display.h"
#include "util/util.h"

// #define GATT_DEVICE_NAME DEVICE_NAME
// #define GATT_MODEL_NUMBER "URU Card v1"
// #define GATT_FIRMWARE_REVISION "1.0.0"

namespace FIDO2
{
    namespace Transport
    {
        namespace USB
        {

            static CTAPHID dev;
            static void (*callback)(uint8_t const* buffer, uint16_t bufsize);
            static const uint8_t cid_broadcast[CID_LENGTH] = {0xff, 0xff, 0xff, 0xff};

            void dummyCallback(uint8_t const* buffer, uint16_t bufsize){}
            void callbackStr(std::string str){
                (*callback)((const uint8_t*)str.c_str(), str.length());
            }

            bool Service::init()
            {
                // dev.deviceID(0x20a0, 0x42b1);
                dev.setCallbacks(new HIDCallbacksImpl());
                callback = &dummyCallback;
                return dev.begin();
            }

            void Service::setCallback(void (*fun)(uint8_t const* buffer, uint16_t bufsize)){
              callback = fun;
            }

            void HIDCallbacksImpl::onData(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize){
                (*callback)(buffer, bufsize);
                // callbackStr("Command buffer init sizeof(buffer): " + std::to_string(sizeof(buffer)) + ", bufsize: " + std::to_string(bufsize));
                if(bufsize < CID_LENGTH + 1){
                  return;
                }

                // A frame is divided into an initialization fragment and zero or more continuation fragments.
                if (buffer[CID_LENGTH] >= 0x80) {
                    if (commandBuffer.init(buffer, bufsize) == 0){
                        // error
                    }
                } else {
                    if (commandBuffer.append(buffer, bufsize) == 0){
                        // error
                    }
                }

                callbackStr("Buffer length: " + std::to_string(commandBuffer.getBufferLength()) + ", payload length: " + std::to_string(commandBuffer.getPayloadLength()));
                if (commandBuffer.isComplete()){
                    callbackStr("Command received completion");
                    processRequest();
                }
            }

            void processRequest()
            {
                switch (commandBuffer.getCmd())
                {
                case CMD_INIT:
                    processINIT();
                    break;
                case CMD_CBOR:
                    processCBOR();
                    break;
                case CMD_WINK:
#if defined(LED_BUILTIN)
                    digitalWrite(LED_BUILTIN, HIGH);
                    delay(100);
                    digitalWrite(LED_BUILTIN, LOW);
#endif
                    sendResponse();
                    break;
                // case CMD_PING:
                //     Serial.println("PING");
                //     statusCharacteristic->setValue(commandBuffer.getBuffer(), commandBuffer.getBufferLength());
                //     statusCharacteristic->notify();
                //     break;
                // case CMD_MSG:
                //     processMessage();
                //     break;
                // case CMD_CANCEL:
                //     Serial.println("CANCEL");
                //     break;
                }
            }

            void processINIT(){
                if(memcmp(commandBuffer.getCid(), cid_broadcast, CID_LENGTH) == 0){
                    // IF is broadcast CID
                    uint8_t *payload = commandBuffer.getPayload();
                    memcpy(&payload[8], commandBuffer.newCid(), 4); // 4-byte new allocated cid
                    payload[12] = 2; // ctaphid version 2
                    payload[13] = 1; // major device version (defined by vendor)
                    payload[14] = 1; // minor device version (defined by vendor)
                    payload[15] = 1; // build device version (defined by vendor)
                    // capability flags bitfield (0x01 -> wink, 0x04 -> implements CMD_CBOR, 0x08 -> not implements CMD_MSG)
                    // 00000100 -> 0x04, 00000101 -> 5
                    payload[16] = 4; // set to 5 to let host know wink is implemented
                    commandBuffer.setPayloadLength(17);
                } else {
                    // ELSE CID is already created
                    // TODO: implement this case
                }
                sendResponse();
            }

            void processCBOR()
            {
                try
                {
                    // parse the request
                    std::unique_ptr<FIDO2::CTAP::Command> request;
                    FIDO2::CTAP::Status statusParse = FIDO2::CTAP::Request::parse(commandBuffer.getPayload(), commandBuffer.getPayloadLength(), request);
                    assert(request != nullptr);

                    // execute
                    std::unique_ptr<FIDO2::CTAP::Command> response;
                    FIDO2::CTAP::Status statusProcess = FIDO2::Authenticator::processRequest(request.get(), response);
                    if (statusProcess != FIDO2::CTAP::CTAP2_OK)
                    {
                        throw FIDO2::CTAP::Exception(statusProcess);
                    }

                    std::unique_ptr<CBOR> cborResponse;
                    if (response != nullptr)
                    {
                        // encode the response
                        FIDO2::CTAP::Status statusEncode = FIDO2::CTAP::Response::encode(response.get(), cborResponse);
                        if (statusEncode != FIDO2::CTAP::CTAP2_OK)
                        {
                            throw FIDO2::CTAP::Exception(statusEncode);
                        }
                    }

                    // send successful result
                    uint8_t *payload = commandBuffer.getPayload();
                    payload[0] = FIDO2::CTAP::CTAP2_OK;
                    if (cborResponse != nullptr && cborResponse->length() > 0)
                    {
                        memcpy(payload + 1, cborResponse->to_CBOR(), cborResponse->length());
                        commandBuffer.setPayloadLength(cborResponse->length() + 1);
                    }
                    else
                    {
                        commandBuffer.setPayloadLength(1);
                    }
                }
                catch (FIDO2::CTAP::Exception &e)
                {
                    uint8_t *payload = commandBuffer.getPayload();
                    payload[0] = e.getStatus();
                    commandBuffer.setPayloadLength(1);
                }

                // stop keepalive
                // keepaliveStop();

                sendResponse();
            }

            // /**
            //  * Send the response with splitting in frames of FIDO2_CONTROL_POINT_LENGTH size
            //  */
            void sendResponse()
            {   
                // Serial.println("Responding with payload");
                // serialDumpBuffer(commandBuffer.getPayload(), commandBuffer.getPayloadLength());
                memset(commandBuffer.getBuffer() + commandBuffer.getBufferLength(), 0, commandBuffer.getMaxBufferLength() - commandBuffer.getBufferLength());
                int pageMultiple = (commandBuffer.getBufferLength() / CFG_TUD_HID_BUFSIZE) + (commandBuffer.getBufferLength() % CFG_TUD_HID_BUFSIZE > 0 ? 1 : 0);

                callbackStr("Responding with payload");
                (*callback)(commandBuffer.getBuffer(), CFG_TUD_HID_BUFSIZE * pageMultiple);

                if(dev.write(commandBuffer.getBuffer(), CFG_TUD_HID_BUFSIZE * pageMultiple) == -1){
                    callbackStr("Failed to send data to hid client");
                } else {
                    callbackStr("Success to send data to hid client");
                }

                // send the response back
                // uint8_t sendBuffer[FIDO2_CONTROL_POINT_LENGTH];

                // uint8_t sendBuffer[CFG_TUD_HID_BUFSIZE];
                // size_t sent = 0;
                // size_t copySize, sendSize;
                // for (uint8_t seq = 0; sent < commandBuffer.getBufferLength(); seq++)
                // {
                //     memset(sendBuffer, 0, CFG_TUD_HID_BUFSIZE);
                //     if (seq == 0)
                //     {
                //         sendSize = MIN(CFG_TUD_HID_BUFSIZE, commandBuffer.getBufferLength());
                //         memcpy(sendBuffer, commandBuffer.getBuffer(), sendSize);
                //         sent += sendSize;
                //     }
                //     else
                //     {
                //         memcpy(sendBuffer, commandBuffer.getBuffer(), CID_LENGTH);
                //         sendBuffer[CID_LENGTH] = seq - 1;

                //         sendSize = MIN(CFG_TUD_HID_BUFSIZE - CID_LENGTH - 1, commandBuffer.getBufferLength() - sent);
                //         memcpy(sendBuffer + CID_LENGTH + 1, commandBuffer.getBuffer() + sent, sendSize);
                //         sent += sendSize;
                //     }

                //     callbackStr("Send chunk " + std::to_string(seq));
                //     (*callback)(sendBuffer, CFG_TUD_HID_BUFSIZE);
                //     if(dev.write(sendBuffer, CFG_TUD_HID_BUFSIZE) == -1){
                //         callbackStr("Failed to send data to hid client");
                //     } else {
                //         callbackStr("Success to send data to hid client");
                //     }
                // }
            }

        } // namespace USB
    }     // namespace Transport
} // namespace FIDO2

#endif