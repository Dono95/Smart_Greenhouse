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
#include "sdkconfig.h"
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

    ESP_LOGI(MAIN_TAG, "Client ID is %d", CONFIG_CLIENT_ID);
#ifdef CONFIG_INSIDE
    ESP_LOGI(MAIN_TAG, "Position set to inside %d", CONFIG_INSIDE);
#endif
#ifdef CONFIG_OUTSIDE
    ESP_LOGI(MAIN_TAG, "Position set to outside %d", CONFIG_CLIENT_ID);
#endif

#ifdef CONFIG_TEMPERATURE
    ESP_LOGI(MAIN_TAG, "Temperature has been chosen %d", CONFIG_TEMPERATURE);
#endif

#ifdef CONFIG_HUMANITY
    ESP_LOGI(MAIN_TAG, "Humanity has been chosen %d", CONFIG_HUMANITY);
#endif

#ifdef CONFIG_CO2
    ESP_LOGI(MAIN_TAG, "CO2 has been chosen %d", CONFIG_CO2);
#endif

    /*if (!greenhouseManager->StartBluetooth())
    {
        ESP_LOGE(MAIN_TAG, "Failed to start bluetooth.");
    }*/

    while (true)
    {
        // Wait for 10s
        vTaskDelay(10000);
    }
}
