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

/* Time manager */
#include "Common_components/Managers/TimeManager.hpp"

/* Status indicator */
#include "Common_components/Utility/Indicator/StatusIndicator.hpp"

// Alias for indicator status code
using StatusCode = Utility::Indicator::StatusCode;

#define MAIN_TAG "Main"

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

    const auto indicator = Utility::Indicator::StatusIndicator::GetInstance();

    // Creating Greenhouse manager
    auto greenhouseManager = Greenhouse::GreenhouseManager::GetInstance();
    if (!greenhouseManager->StartBluetoothServer())
    {
        indicator->RaiseState(StatusCode::BLUETOOTH_INIT_FAILED);
        ESP_LOGE(MAIN_TAG, "Bluetooth startup failed!");
        return;
    }
    else
    {
        indicator->RaiseState(StatusCode::BLUETOOTH_INIT_SUCCESSED);
    }

    indicator->RaiseState(StatusCode::CLIENT_CONNECTING_TO_NETWORK);
    while (const auto networkStatus = !(greenhouseManager->ConnectToNetwork()))
    {
        indicator->RaiseState(StatusCode::CLIENT_CONNECTION_FAILED);
        ESP_LOGE(MAIN_TAG, "Failed to connect to WiFi network!");
        vTaskDelay(60000);

        indicator->RaiseState(StatusCode::CLIENT_CONNECTING_TO_NETWORK);
    }

    indicator->RaiseState(StatusCode::CLIENT_CONNECTION_ESTABLISHED);

    auto timeManager = Component::Manager::TimeManager::GetInstance();
    timeManager->Initialize();

    if (!greenhouseManager->ConnectToMQTT())
        ESP_LOGE(MAIN_TAG, "Failed to connect to MQTT Broker!");

    while (true)
    {
        vTaskDelay(1000);
    }
}