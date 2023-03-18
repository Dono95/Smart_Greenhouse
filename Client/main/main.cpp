/* STD includes */
#include <stdio.h>
#include <cstring>

/* non-volatile flash memory */
#include "nvs_flash.h"

/* Project specific includes*/
#include "GreenhouseManager.hpp"

/* Common components */
#include "Common_components/Utility/Indicator/StatusIndicator.hpp"

/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* ESP log library */
#include "esp_log.h"

#define MAIN_TAG "Main"

/******** TEST ************/
#include "Common_components/Trackers/BluetoothConnectionTracker.hpp"
#include "Common_components/Trackers/BluetoothConnectionTracker.cpp"

#include "Common_components/Utility/Indicator/StatusIndicator.hpp"

#include "esp_timer.h"
/*******  END TEST ********/

// Alias for status indicator
using Indicator = Utility::Indicator::StatusIndicator;

// Alias for indicator status code
using Indicator_StatusCode = Utility::Indicator::StatusCode;

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

	if (!greenhouseManager->StartBluetooth())
	{
		ESP_LOGE(MAIN_TAG, "Failed to start bluetooth.");
		Indicator::GetInstance()->RaiseState(Indicator_StatusCode::BLUETOOTH_INIT_FAILED);
		return;
	}

	Indicator::GetInstance()->RaiseState(Indicator_StatusCode::BLUETOOTH_INIT_SUCCESSED);
	greenhouseManager->StartTrackBluetoothConnection();

	while (true)
	{
		// Wait for 2 min
		vTaskDelay(60000 * 10);

		greenhouseManager->SendDataToServer();
	}
}
