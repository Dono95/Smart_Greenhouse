/* STD includes */
#include <stdio.h>

/* non-volatile flash memory */
#include "nvs_flash.h"

/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"

/* ESP logs library */
#include "esp_log.h"

/* Bluetooth includes */
#include "esp_bt.h"             //  implements BT controller and VHCI configuration procedures from the host side.
#include "esp_bt_main.h"        //  implements initialization and enabling of the Bluedroid stack.
#include "esp_gap_ble_api.h"    //  implements GAP configuration, such as advertising and connection parameters.
#include "esp_gatts_api.h"      //  implements GATT configuration, such as creating services and characteristics.



/****** DEFINE *********/
#define NUMBER_OF_PROFILES 1
#define GREEENHOUSE_PROFILE 0

/* Declare profile event handler */
static void gatts_greenhouse_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst gl_profile_tab[NUMBER_OF_PROFILES] = {
    [GREEENHOUSE_PROFILE] = {
        .gatts_cb = gatts_greenhouse_profile_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};

static const char* GATTS_TAG = "Gatt_Server_Greenhouse";


static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) 
{
    ESP_LOGI(GATTS_TAG, "Gap event handler: %s", __func__);
}

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) 
{
    ESP_LOGI(GATTS_TAG, "Gatts event handler: %s", __func__);
}

static void gatts_greenhouse_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) 
{

}

extern "C" void app_main(void)
{
    // Initialize NVS.
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        result = nvs_flash_init();
    }

    // Check result of initialization non-volatile flash memory
    ESP_ERROR_CHECK(result);

    // Check result of bluetooth controller to memory release of classic bluetooth
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    // Initialization of default config
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    result = esp_bt_controller_init(&bt_cfg);
    if (result) {
        ESP_LOGE(GATTS_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(result));
        return;
    }

    // Enable bluettoth controller to Bluetooth low energy mode
    result = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (result) {
        ESP_LOGE(GATTS_TAG, "%s enable controller failed\n", __func__);
        return;
    }

    // Initialize bluedroid
    result = esp_bluedroid_init();
    if (result) {
        ESP_LOGE(GATTS_TAG, "%s init bluetooth failed\n", __func__);
        return;
    }

    // Enable bluedroid
    result = esp_bluedroid_enable();
    if (result) {
        ESP_LOGE(GATTS_TAG, "%s enable bluetooth failed\n", __func__);
        return;
    }

    // Register callback for bluetotth generic attribites
    result = esp_ble_gatts_register_callback(gatts_event_handler);
    if (result){
        ESP_LOGE(GATTS_TAG, "gatts register error, error code = %x", result);
        return;
    }

    result = esp_ble_gap_register_callback(gap_event_handler);
    if (result){
        ESP_LOGE(GATTS_TAG, "gap register error, error code = %x", result);
        return;
    }

    result = esp_ble_gatts_app_register(GREEENHOUSE_PROFILE);
    if (result){
        ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", result);
        return;
    }
}
