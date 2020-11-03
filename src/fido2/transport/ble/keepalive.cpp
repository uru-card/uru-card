#include <Arduino.h>

#include <BLEService.h>

#include "fido2/authenticator/authenticator.h"

#define STACK_SIZE 4096

namespace FIDO2
{
    namespace Transport
    {
        namespace BLE
        {
            static TaskHandle_t xHandle = NULL;
            static SemaphoreHandle_t xSemaphore = NULL;

            static void keepaliveTask(void *pvParameters)
            {
                BLECharacteristic *characteristic = (BLECharacteristic *)pvParameters;

                const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

                while (1)
                {
                    vTaskDelay(xDelay);

                    uint8_t packet[4] = {
                        0xbe, // CMD_KEEPALIVE,
                        0x00,
                        0x01,
                        FIDO2::Authenticator::getStatus(),
                    };

                    Serial.println(".");

                    characteristic->setValue(packet, 4);
                    characteristic->notify();
                }
            }

            bool keepaliveStart(BLECharacteristic *statusCharacteristic)
            {
                if (xHandle != NULL)
                {
                    // ESP_LOGE(LOG_TAG, "Keepalive task is already running");
                    return false;
                }

                BaseType_t xReturned = xTaskCreate(keepaliveTask, "BLE::KeepAlive", STACK_SIZE, (void *)statusCharacteristic, 0, &xHandle);

                // ESP_LOGD(LOG_TAG, "KeepAlive task create result %d", xReturned);

                return ESP_OK;
            }

            void keepaliveStop()
            {
                if (xHandle == NULL)
                {
                    // ESP_LOGW(LOG_TAG, "No keepalive task");
                    return;
                }

                vTaskDelete(xHandle);

                xHandle = NULL;
            }
        } // namespace BLE
    }     // namespace Transport
} // namespace FIDO2