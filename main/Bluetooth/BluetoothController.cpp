/* Project specific includes */
#include "BluetoothController.hpp"

/* STL includes*/
#include <iostream>

/* ESP logs library */
#include "esp_log.h"

using namespace Greenhouse;

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Class constructor
 */
BluetoothController::BluetoothController()
{
}

/**
 * @brief Class destructor
 */
BluetoothController::~BluetoothController()
{
}

/**
 * @brief Method to initialize bluetooth controller. Set bluetooth mode and register
 * callbacks for gap and gatts
 */
BluetoothController::INIT_BLUETOOTH_RV BluetoothController::InitBluetoothController(esp_bt_mode_t bluetoothMode)
{
    // TO DO -> Need to release opposite mode from bluetoothMode
    // For now programmer is responsible to use only ESP_BT_MODE_BLE

    // TO DO -> Split init od blupedroid; register callback and app; esp controller init

    // Check result of bluetooth controller to memory release of classic bluetooth
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    INIT_BLUETOOTH_RV result = InitESP_BluetoothController(bluetoothMode);
    if (result != INIT_BLUETOOTH_RV::RV_BLUETOOTH_OFF)
        return result;

    result = InitBluedroid();
    if (result != INIT_BLUETOOTH_RV::RV_BLUETOOTH_OFF)
        return result;

    // Register callback for bluetotth generic attribites
    esp_err_t registerResult = esp_ble_gatts_register_callback(gatts_event_handler);
    if (registerResult)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gatts register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gap_register_callback(BluetoothController::GapEventHandler);
    if (registerResult)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gap register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gatts_app_register(GREEENHOUSE_PROFILE);
    if (registerResult)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gatts app register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_APP_FAILED;
    }

    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(512);
    if (local_mtu_ret)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
        return INIT_BLUETOOTH_RV::RV_SET_LOCAL_MTU_FAILED;
    }

    return INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK;
}

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Method to initialize ESP bluetooth controller.
 */
BluetoothController::INIT_BLUETOOTH_RV BluetoothController::InitESP_BluetoothController(const esp_bt_mode_t bluetoothMode)
{
    // Initialization of default config
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_err_t result = esp_bt_controller_init(&bt_cfg);
    if (result)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(result));
        return INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_FAILED;
    }

    // Set Bluetooth  mode
    result = esp_bt_controller_enable(bluetoothMode);
    if (result)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "%s enable controller failed\n", __func__);
        return INIT_BLUETOOTH_RV::RV_BLUETOOTH_SET_MODE_FAILED;
    }

    return INIT_BLUETOOTH_RV::RV_BLUETOOTH_OFF;
}

/**
 * @brief Method to initialize ESP bluedroid.
 */
BluetoothController::INIT_BLUETOOTH_RV BluetoothController::InitBluedroid(void)
{
    // Initialize bluedroid
    esp_err_t result = esp_bluedroid_init();
    if (result)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "%s init bluetooth failed\n", __func__);
        return INIT_BLUETOOTH_RV::RV_BLUEDROID_FAILED;
    }

    // Enable bluedroid
    result = esp_bluedroid_enable();
    if (result)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "%s enable bluetooth failed\n", __func__);
        return INIT_BLUETOOTH_RV::RV_BLUEDROID_FAILED;
    }

    return INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK;
}

/**
 * @brief The method to handle event that are pushed from BLE stack
 */
void BluetoothController::GattsEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    printf("Tu som in GattsEventHandler\n");
    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "%s with event type: %d", __func__, event);

    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT)
    {
        if (param->reg.status == ESP_GATT_OK)
            ;
        // gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
        else
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "Reg app failed, app_id %04x, status %d\n", param->reg.app_id, param->reg.status);
            return;
        }
    }

    for (int profileID = 0; profileID < NUMBER_OF_PROFILES; ++profileID)
    {
        /*if (gatts_if == ESP_GATT_IF_NONE || gatts_if == gl_profile_tab[profileID].gatts_if)
        {
            if (!gl_profile_tab[profileID].gatts_cb)
            {
                ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "No callback function set for profile with ID [%d].", profileID);
                continue;
            }

            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "Calling callback for profile with ID [%d].", profileID);
            gl_profile_tab[profileID].gatts_cb(event, gatts_if, param);
        }*/
    }
}

/**
 * @brief The method to handle event that are pushed from BLE stack
 */
void BluetoothController::GapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "%s with type %d", __func__, event);
}