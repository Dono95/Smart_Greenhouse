/* STD includes */
#include <stdio.h>
#include <cstring>

/* non-volatile flash memory */
#include "nvs_flash.h"

/* Project specific includes*/
#include "GreenhouseManager.hpp"

/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* ESP log library*/
#include "esp_log.h"

#define MAIN_TAG "Main"

// TIME
#define MAX_ATTEMPTS 10    // 10 attempts
#define WAIT_FOR_SYNC 1000 // 1

////// TEST

#include "EventManager.hpp"
#include "Common_components/Publisher/EventData.hpp"

#include "NetworkManager.h"

#include <cJSON.h>
#include "Common_components/Convertors/Convertor_JSON.hpp"

#include "Common_components/Managers/TimeManager.hpp"

/// END TEST

extern "C" void app_main(void)
{
    // Initialize NVS.
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {

        ESP_ERROR_CHECK(nvs_flash_erase());
        result = nvs_flash_init();
    }

    // Check result of initialization non-volatile flash memory
    ESP_ERROR_CHECK(result);

    // Creating Greenhouse manager
    auto greenhouseManager = Greenhouse::GreenhouseManager::GetInstance();
    if (!greenhouseManager->ConnectToNetwork())
        ESP_LOGE(MAIN_TAG, "Failed to connect to WiFi network!");

    auto timeManager = Component::Manager::TimeManager::GetInstance();
    /*timeManager->Initialize();

    // Wait to synchronize time with NTP server
    for (uint8_t i = 1; i <= MAX_ATTEMPTS; ++i)
    {
        if (timeManager->GetStatus() == SNTP_SYNC_STATUS_COMPLETED)
            break;

        ESP_LOGI(MAIN_TAG, "Waiting for system time to be synchronized ... (%d/%d)", i, MAX_ATTEMPTS);
        vTaskDelay(WAIT_FOR_SYNC);
    }*/

    if (!greenhouseManager->ConnectToMQTT())
        ESP_LOGE(MAIN_TAG, "Failed to connect to MQTT Broker!");

    // auto eventManager = Greenhouse::Manager::EventManager::GetInstance();

    if (!greenhouseManager->StartBluetoothServer())
        ESP_LOGE(MAIN_TAG, "Bluetooth startup failed!");

    while (true)
    {
        /*auto temp = 18 + (std::rand() % (24 - 18 + 1));
        auto hum = 25 + (std::rand() % (50 - 25 + 1));
        auto eventData = new Component::Publisher::ClientBluetoothEventData_Greenhouse(temp, hum, 20);
        eventData->SetPosition(0x01);
        eventManager->Notify(Greenhouse::Manager::EventManager::Event_T::BLUETOOTH_DATA_RECEIVED, eventData);*/

        // delete eventData;
        //  sleep 1 min
        vTaskDelay(60000 * 20);
    }
}