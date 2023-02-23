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

#include "Common_components/Managers/TimeManager.hpp"

#include "Common_components/Drivers/Communication/I2C.hpp"
#include "Common_components/Drivers/Sensor/WaterLevelSensor.hpp"

#include "GreenhouseDefinitions.hpp"

#include "Managers/ComponentController.hpp"

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
    timeManager->Initialize();

    if (!greenhouseManager->ConnectToMQTT())
        ESP_LOGE(MAIN_TAG, "Failed to connect to MQTT Broker!");

    if (!greenhouseManager->StartBluetoothServer())
        ESP_LOGE(MAIN_TAG, "Bluetooth startup failed!");

    while (true)
    {
        vTaskDelay(10000);
    }
}