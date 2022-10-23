/* Project specific includes */
#include "BluetoothController.hpp"
#include "GreenhouseManager.hpp"

/* STL includes*/
#include <iostream>

/* ESP logs library */
#include "esp_log.h"

using namespace Greenhouse::Bluetooth;

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
    if (bluetoothMode != ESP_BT_MODE_BLE)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "Invalid bluetooth type pass in %s.", __func__);
        return INIT_BLUETOOTH_RV::RV_INVALID_ARGUMENT;
    }

    // Check result of bluetooth controller to memory release of classic bluetooth
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    INIT_BLUETOOTH_RV result = InitESP_BluetoothController(bluetoothMode);
    if (result != INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK)
        return result;

    result = InitBluedroid();
    if (result != INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK)
        return result;

    auto GattsCallback = [](esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
    {
        const auto manager = GreenhouseManager::GetInstance();
        if (manager)
            manager->GetBluetoothHandler()->HandleGattsEvent(event, gatts_if, param);
    };

    esp_err_t registerResult = esp_ble_gatts_register_callback(GattsCallback);
    if (registerResult)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gatts register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_CALLBACK_FAILED;
    }

    auto GapCallback = [](esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
    {
        const auto manager = GreenhouseManager::GetInstance();
        if (manager)
            manager->GetBluetoothHandler()->HandleGapEvent(event, param);
    };

    registerResult = esp_ble_gap_register_callback(GapCallback);
    if (registerResult)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gap register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gatts_app_register(GREENHOUSE_PROFILE);
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

    return INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK;
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