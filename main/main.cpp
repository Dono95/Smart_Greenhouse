#include <stdio.h>

#include "Drivers/Sensors/SHT4x.hpp"

#include <esp_log.h>
#include <nvs_flash.h>

#include <esp_bt.h>
#include <esp_gap_bt_api.h>
#include "esp_gatt_common_api.h"
#include <esp_bt_main.h>
#include <esp_bt_device.h>

#include "esp_system.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"

static const char *MAIN_TAG = "Main";

extern "C" void app_main(void)
{
    esp_err_t error;

    ESP_LOGI(MAIN_TAG, "Initializing...");

    error = nvs_flash_init();
    if (error == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        error = nvs_flash_init();
    }
    ESP_ERROR_CHECK(error);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    error = esp_bt_controller_init(&bt_cfg);
    if (error)
    {
        ESP_LOGE(MAIN_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(error));
        return;
    }

    error = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (error)
    {
        ESP_LOGE(MAIN_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(error));
        return;
    }

    error = esp_bluedroid_init();
    if (error)
    {
        ESP_LOGE(MAIN_TAG, "%s init bluetooth failed: %s\n", __func__, esp_err_to_name(error));
        return;
    }
    error = esp_bluedroid_enable();
    if (error)
    {
        ESP_LOGE(MAIN_TAG, "%s enable bluetooth failed: %s\n", __func__, esp_err_to_name(error));
        return;
    }

    const uint8_t *address = esp_bt_dev_get_address();
    for (uint8_t i = 0; i < 6; i++)
        printf("%x ", address[i]);

    printf("\n");

    printf("Set name %d\n", esp_bt_dev_set_device_name("ESP Test"));

    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    // esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

    /*Sensor::SHT4x SHT41(0x44);
ESP_LOGI(MAIN_TAG, "The serial number of the SHT41 sensor is %s", SHT41.SerialNumber().c_str());

SHT41.TriggerMeasure(Sensor::SHT4x::MeasurePrecision::LOW);

ESP_LOGI("Main", "Tempearure is %f", SHT41.Temperature());
ESP_LOGI("Main", "Humanity is %d", SHT41.Humanity());*/
    while (true)
    {
    }
}
