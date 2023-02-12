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

    /*auto i2c = new Component::Driver::Communication::I2C(GPIO_NUM_21, GPIO_NUM_22);
i2c->SetMode(i2c_mode_t::I2C_MODE_MASTER, I2C_NUM_0, 400000u);

if (i2c->Activate() != ESP_OK)
    ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Activation of I2C failed");

while (true)
{
    std::vector<uint8_t> high_data;
    auto i2c_result = i2c->Read(0x78, high_data, 12);
    ESP_LOGE(MAIN_TAG, "Result: %d", static_cast<int>(i2c_result));

    vTaskDelay(1000);

    std::vector<uint8_t> low_data;
    i2c_result = i2c->Read(0x77, low_data, 8);
    ESP_LOGE(MAIN_TAG, "Result: %d", static_cast<int>(i2c_result));

    int sensorvalue_min = 250;
    int sensorvalue_max = 255;
    int low_count = 0;
    int high_count = 0;

    uint32_t touch_val = 0;
    uint8_t trig_section = 0;
    low_count = 0;
    high_count = 0;

    printf("low 8 sections value = \n");
    for (int i = 0; i < 8; i++)
    {
        printf("%d", low_data[i]);
        printf(".");
        if (low_data[i] >= sensorvalue_min && low_data[i] <= sensorvalue_max)
        {
            low_count++;
        }
        if (low_count == 8)
        {
            printf("      ");
            printf("PASS");
        }
    }
    printf("  \n");
    printf("  \n");
    printf("high 12 sections value = \n");
    for (int i = 0; i < 12; i++)
    {
        printf("%d", high_data[i]);
        printf(".");

        if (high_data[i] >= sensorvalue_min && high_data[i] <= sensorvalue_max)
        {
            high_count++;
        }
        if (high_count == 12)
        {
            printf("      ");
            printf("PASS");
        }
    }

    printf("  \n");
    printf("  \n");

    for (int i = 0; i < 8; i++)
    {
        if (low_data[i] > 100)
        {
            touch_val |= 1 << i;
        }
    }
    for (int i = 0; i < 12; i++)
    {
        if (high_data[i] > 100)
        {
            touch_val |= (uint32_t)1 << (8 + i);
        }
    }

    while (touch_val & 0x01)
    {
        trig_section++;
        touch_val >>= 1;
    }
    printf("water level = ");
    printf("%d", trig_section * 5);
    printf("%% \n");
    printf(" \n");
    printf("*********************************************************\n");
    vTaskDelay(1000);
}*/

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
        vTaskDelay(60000 * 20);
    }
}