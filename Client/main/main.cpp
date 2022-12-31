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

/* ESP log library */
#include "esp_log.h"

#define MAIN_TAG "Main"

/******** TEST ************/

#include "driver/adc.h"
#include "esp_adc_cal.h"

/*******  END TEST ********/

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
    /*if (!greenhouseManager->StartBluetooth())
    {
        ESP_LOGE(MAIN_TAG, "Failed to start bluetooth.");
    }*/

    int read_raw;
    adc2_config_channel_atten(ADC2_CHANNEL_0, ADC_ATTEN_11db);

    while (true)
    {
        if (adc2_get_raw(ADC2_CHANNEL_0, ADC_WIDTH_12Bit, &read_raw) == ESP_OK)
            printf("Value: %d\n", read_raw);
        else
            printf("Bad reading...\n");

        vTaskDelay(100);
    }

    while (true)
    {
        vTaskDelay(1000);
    }
}
