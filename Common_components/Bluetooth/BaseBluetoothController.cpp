/* Project specific includes */
#include "BaseBluetoothController.hpp"

/* ESP log library */
#include "esp_log.h"

using namespace Component::Bluetooth;

/**
 * @brief Class base contrustor
 */
BaseBluetoothController::BaseBluetoothController()
{
}

/**
 * @brief Virtual class base destructor
 */
BaseBluetoothController::~BaseBluetoothController()
{
}

/**
 * @brief Method to initialize bluetooth controller.
 */
INIT_BLUETOOTH_RV BaseBluetoothController::InitBluetoothController(const esp_bt_mode_t bluetoothMode)
{
    if (bluetoothMode != ESP_BT_MODE_BLE)
    {
        ESP_LOGE(BASE_BLUETOOTH_CONTROLLER_TAG, "Invalid bluetooth type pass in %s.", __func__);
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


    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(512);
    if (local_mtu_ret)
    {
        ESP_LOGE(BASE_BLUETOOTH_CONTROLLER_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
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
INIT_BLUETOOTH_RV BaseBluetoothController::InitESP_BluetoothController(const esp_bt_mode_t bluetoothMode)
{
    // Initialization of default config
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_err_t result = esp_bt_controller_init(&bt_cfg);
    if (result)
    {
        ESP_LOGE(BASE_BLUETOOTH_CONTROLLER_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(result));
        return INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_FAILED;
    }

    // Set Bluetooth  mode
    result = esp_bt_controller_enable(bluetoothMode);
    if (result)
    {
        ESP_LOGE(BASE_BLUETOOTH_CONTROLLER_TAG, "%s enable controller failed\n", __func__);
        return INIT_BLUETOOTH_RV::RV_BLUETOOTH_SET_MODE_FAILED;
    }

    return INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK;
}

/**
 * @brief Method to initialize ESP bluedroid.
 */
INIT_BLUETOOTH_RV BaseBluetoothController::InitBluedroid(void)
{
    // Initialize bluedroid
    esp_err_t result = esp_bluedroid_init();
    if (result)
    {
        ESP_LOGE(BASE_BLUETOOTH_CONTROLLER_TAG, "%s init bluetooth failed\n", __func__);
        return INIT_BLUETOOTH_RV::RV_BLUEDROID_FAILED;
    }

    // Enable bluedroid
    result = esp_bluedroid_enable();
    if (result)
    {
        ESP_LOGE(BASE_BLUETOOTH_CONTROLLER_TAG, "%s enable bluetooth failed\n", __func__);
        return INIT_BLUETOOTH_RV::RV_BLUEDROID_FAILED;
    }

    return INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK;
}