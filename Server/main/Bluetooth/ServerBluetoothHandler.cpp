#include "ServerBluetoothHandler.hpp"

/* ESP log library */
#include "esp_log.h"

/* STL includes */
#include "algorithm"

using namespace Greenhouse::Bluetooth;

/**
 * @brief Default class constructor
 */
ServerBluetoothHandler::ServerBluetoothHandler()
{
    mProfileMap.insert({GREENHOUSE_PROFILE, {}});
}

/**
 * @brief Class destructor
 */
ServerBluetoothHandler::~ServerBluetoothHandler()
{
}

/**
 * @brief Method to handle events from gatts interface on server side
 */
void ServerBluetoothHandler::HandleGattsEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    if (IsRegistrationEvent(event)) {
        if (!HandleRegistrationEvent(gatts_if, param)) {
            return;
        }
    }
}

/**
 * @brief Checking eventy for comparasion on ESP_GATTS_REG_EVT
 */
bool ServerBluetoothHandler::IsRegistrationEvent(esp_gatts_cb_event_t event) const
{
    return event == ESP_GATTS_REG_EVT;
}

/**
 * @brief Handle registration event
 */
bool ServerBluetoothHandler::HandleRegistrationEvent(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    // Check if registration was successful
    if (param->reg.status != ESP_GATT_OK)
    {
        ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "Reg app failed, app_id %04x, status %d\n", param->reg.app_id, param->reg.status);
        return false;
    }

    // Find profile based on ID
    auto profile = std::find_if(mProfileMap.begin(), mProfileMap.end(), [&](const std::pair<uint8_t, GattsProfile_I>& itr){
        if (itr.first == param->reg.app_id) {
            return true;
        }

        return false;
    });

    // If profile with specific ID exist, register Gatts interface to profile structure
    if (profile != mProfileMap.end()) {
        profile->second.gatts_if = gatts_if;
        return true;
    }

    return false;
}