/* Project specific includes */
#include "ServerBluetoothController.hpp"
#include "GreenhouseManager.hpp"

/* ESP logs library */
#include "esp_log.h"

using namespace Greenhouse::Bluetooth;

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Class constructor
 */
ServerBluetoothController::ServerBluetoothController()
{
}

/**
 * @brief Class destructor
 */
ServerBluetoothController::~ServerBluetoothController()
{
}

/**
 * @brief Overridden method to register gap and gatts callback for client controller
 */
ServerBluetoothController::INIT_BLE_STATUS ServerBluetoothController::RegisterCallbacks(void)
{
    auto registerResult = esp_ble_gap_register_callback([](esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
                                                        {
        const auto manager = GreenhouseManager::GetInstance();
        if (manager)
            manager->GetHandler()->HandleGapEvent(event, param); });

    if (registerResult)
    {
        ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "Gap register error, error code = %x", registerResult);
        return INIT_BLE_STATUS::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gatts_register_callback([](esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
                                                     {
        const auto manager = GreenhouseManager::GetInstance();
        if (manager)
            manager->GetHandler()->HandleGattsEvent(event, gatts_if, param); });

    if (registerResult)
    {
        ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "gatts register error, error code = %x", registerResult);
        return INIT_BLE_STATUS::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gatts_app_register(GREENHOUSE_PROFILE);
    if (registerResult)
    {
        ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "gatts app register error, error code = %x", registerResult);
        return INIT_BLE_STATUS::RV_REGISTER_APP_FAILED;
    }

    return INIT_BLE_STATUS::RV_BLUETOOTH_INIT_OK;
}

/*********************************************
 *              PRIVATE API                  *
 ********************************************/