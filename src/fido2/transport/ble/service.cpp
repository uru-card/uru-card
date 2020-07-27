#include <memory>

#include <Arduino.h>

#include <BLE2902.h>
#include <BLEService.h>

#include "ble/device.h"

#include "fido2/transport/ble/buffer.h"
#include "fido2/transport/ble/service.h"

#include "fido2/ctap/ctap.h"

#include "fido2/authenticator/authenticator.h"

#include "util.h"

#define FIDO2_CONTROL_POINT_LENGTH 256

namespace FIDO2
{
    namespace Transport
    {
        namespace BLE
        {
            BLEService *Service::fido2Service = nullptr;

            BLECharacteristic *statusCharacteristic = nullptr;

            BLEUUID Service::UUID()
            {
                return BLEUUID((uint16_t)0xFFFD);
            }

            void Service::init()
            {
                fido2Service = ::BLE::server->createService(Service::UUID());

                // FIDO Control Point
                fido2Service
                    ->createCharacteristic(ControlPoint::UUID(), BLECharacteristic::PROPERTY_WRITE)
                    ->setCallbacks(new ControlPoint());

                // FIDO Status
                statusCharacteristic = fido2Service
                                           ->createCharacteristic(Status::UUID(), BLECharacteristic::PROPERTY_NOTIFY);
                statusCharacteristic->addDescriptor(new BLE2902());

                // FIDO Control Point Length
                fido2Service
                    ->createCharacteristic(ControlPointLength::UUID(), BLECharacteristic::PROPERTY_READ)
                    ->setCallbacks(new ControlPointLength());

                // FIDO Service Revision Bitfield
                fido2Service
                    ->createCharacteristic(ServiceRevision::UUID(), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE)
                    ->setCallbacks(new ServiceRevision());

                fido2Service->start();
            }

            BLEUUID ControlPoint::UUID()
            {
                return BLEUUID("F1D0FFF1-DEAA-ECEE-B42F-C9BA7ED623BB");
            }

            /**
             * fidoControlPoint is a write-only command buffer.
             */
            void ControlPoint::onWrite(BLECharacteristic *pCharacteristic)
            {
                std::string value = pCharacteristic->getValue();
                if (value.length() == 0)
                {
                    return;
                }

                // A frame is divided into an initialization fragment and zero or more continuation fragments.
                uint8_t cmd = value[0];
                if (cmd >= 0x80)
                {
                    // The start of an initialization fragment is indicated by setting the high bit in the first byte.
                    // The subsequent two bytes indicate the total length of the frame, in big-endian order.
                    // The first maxLen - 3 bytes of data follow.
                    if (commandBuffer.init((const uint8_t *)value.c_str(), value.length()) == 0)
                    {
                        // error
                    }
                }
                else
                {
                    // Continuation fragments begin with a sequence number, beginning at 0, implicitly with the high bit cleared.
                    // The sequence number must wraparound to 0 after reaching the maximum sequence number of 0x7f.
                    if (commandBuffer.append((const uint8_t *)value.c_str(), value.length()) == 0)
                    {
                        // error
                    }
                }

                //
                if (commandBuffer.isComplete())
                {
                    Serial.printf("Received command 0x%02x with payload\n", commandBuffer.getCmd());
                    serialDumpBuffer(commandBuffer.getPayload(), commandBuffer.getPayloadLength());

                    processRequest();
                }
            }

            void ControlPoint::processRequest()
            {
                switch (commandBuffer.getCmd())
                {
                case CMD_PING:
                    statusCharacteristic->setValue(commandBuffer.getBuffer(), commandBuffer.getBufferLength());
                    statusCharacteristic->notify();
                    break;
                case CMD_MSG:
                    processMessage();
                    break;
                case CMD_CANCEL:
                    break;
                }
            }

            void ControlPoint::processMessage()
            {
                // parse the request
                std::unique_ptr<FIDO2::CTAP::Command> request(FIDO2::CTAP::parseRequest(commandBuffer.getPayload(), commandBuffer.getPayloadLength()));
                if (request->getCommandCode() == FIDO2::CTAP::authenticatorError)
                {
                    // could not parse, respond with the error
                    return;
                }

                // execute
                std::unique_ptr<FIDO2::CTAP::Command> response(FIDO2::Authenticator::processRequest(request.get()));
                if (response->getCommandCode() == FIDO2::CTAP::authenticatorError)
                {
                    // could not process, respond with the error
                    return;
                }

                // encode the response
                size_t encodedLength = commandBuffer.getMaxBufferLength() - 3;
                FIDO2::CTAP::Status encodeResult = FIDO2::CTAP::encodeResponse(response.get(), commandBuffer.getPayload(), encodedLength);

                if (encodeResult != FIDO2::CTAP::CTAP2_OK)
                {
                    // respond with error
                    sendError(encodeResult);
                    return;
                }

                commandBuffer.setPayloadLength(encodedLength);
                sendResponse();
            }

            void ControlPoint::sendResponse()
            {
                Serial.println("Responding with payload");
                serialDumpBuffer(commandBuffer.getPayload(), commandBuffer.getPayloadLength() - 1);

                // send the response back
                uint8_t sendBuffer[FIDO2_CONTROL_POINT_LENGTH];
                size_t sent = 0;
                size_t copySize, sendSize;
                for (uint8_t seq = 0; sent < commandBuffer.getBufferLength(); seq++)
                {
                    if (seq == 0)
                    {
                        sendSize = MIN(FIDO2_CONTROL_POINT_LENGTH, commandBuffer.getBufferLength());
                        memcpy(sendBuffer, commandBuffer.getBuffer(), sendSize);
                        sent += sendSize;
                    }
                    else
                    {
                        sendBuffer[0] = seq - 1;

                        copySize = MIN(FIDO2_CONTROL_POINT_LENGTH - 1, commandBuffer.getBufferLength() - sent);
                        memcpy(sendBuffer + 1, commandBuffer.getBuffer() + sent, copySize);

                        sent += copySize;
                        sendSize = copySize + 1;
                    }

                    Serial.printf("seq: %d\n", seq);
                    Serial.printf("size: %d\n", sendSize);
                    serialDumpBuffer(sendBuffer, sendSize);

                    statusCharacteristic->setValue(sendBuffer, sendSize);
                    statusCharacteristic->notify();
                }
            }

            void ControlPoint::sendError(uint8_t errorCode)
            {
            }

            BLEUUID Status::UUID()
            {
                return BLEUUID("F1D0FFF2-DEAA-ECEE-B42F-C9BA7ED623BB");
            }

            BLEUUID ControlPointLength::UUID()
            {
                return BLEUUID("F1D0FFF3-DEAA-ECEE-B42F-C9BA7ED623BB");
            }

            /**
             * fidoControlPointLength defines the maximum size in bytes of a single write request to fidoControlPoint.
             * This value SHALL be between 20 and 512.
             */
            void ControlPointLength::onRead(BLECharacteristic *pCharacteristic)
            {
                uint8_t value[] = {
                    (FIDO2_CONTROL_POINT_LENGTH >> 8) & 0xFF,
                    FIDO2_CONTROL_POINT_LENGTH & 0xFF,
                };
                pCharacteristic->setValue(value, sizeof(value));
            }

            BLEUUID ServiceRevision::UUID()
            {
                return BLEUUID("F1D0FFF4-DEAA-ECEE-B42F-C9BA7ED623BB");
            }

            /**
             * fidoServiceRevisionBitfield defines the revision of the FIDO Service.
             * The value is a bit field which each bit representing a version.
             */
            void ServiceRevision::onRead(BLECharacteristic *pCharacteristic)
            {
                // A device that only supports FIDO2 Rev 1 will only have a fidoServiceRevisionBitfield
                // characteristic of length 1 with value 0x20.
                uint8_t value[] = {0x20};
                pCharacteristic->setValue(value, 1);
            }

            void ServiceRevision::onWrite(BLECharacteristic *pCharacteristic)
            {
            }

        } // namespace BLE
    }     // namespace Transport
} // namespace FIDO2