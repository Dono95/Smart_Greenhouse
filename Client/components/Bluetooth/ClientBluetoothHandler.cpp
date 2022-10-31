/* Project specific includes */
#include "ClientBluetoothHandler.hpp"
#include "GreenhouseManager.hpp"

/* ESP log library*/
#include "esp_log.h"

/* STD library includes */
#include <algorithm>

using namespace Greenhouse::Bluetooth;

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Class constructor
 */
ClientBluetoothHandler::ClientBluetoothHandler()
{
}

/**
 * @brief Class destructor
 */
ClientBluetoothHandler::~ClientBluetoothHandler()
{
}

/**
 * @brief Overriden method to initialize bluetooth profiles
 */
bool ClientBluetoothHandler::InitializeBluetoothProfiles()
{
    mProfilesMap.insert({GREENHOUSE_PROFILE, {.gattc_cb = [](esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
                                              {
                                                  const auto manager = GreenhouseManager::GetInstance();

                                                  if (!manager)
                                                      return;

                                                  const auto clientHandler = std::dynamic_pointer_cast<ClientBluetoothHandler>(manager->GetHandler());
                                                  if (clientHandler)
                                                      clientHandler->GreenhouseEventHandler(event, gattc_if, param);
                                              },
                                              .gattc_if = ESP_GATT_IF_NONE}});

    ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Initialization of bluetooth profiles was successful.");
    return true;
}

/**
 * @brief Overriden method to handle events from gatts interface (BLE stack)
 */
void ClientBluetoothHandler::HandleGattcEvent(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    if (IsRegistrationEvent(event))
    {
        if (!HandleRegistrationEvent(gattc_if, param))
        {
            ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Unable to handle registration event.");
            return;
        }
    }

    for (const auto &profile : mProfilesMap)
    {
        if (gattc_if == ESP_GATT_IF_NONE || gattc_if == profile.second.gattc_if)
        {
            if (profile.second.gattc_cb)
                profile.second.gattc_cb(event, gattc_if, param);
        }
    }
}

/**
 * @brief Overriden method to handle event that are pushed from BLE stack
 */
void ClientBluetoothHandler::HandleGapEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event)
    {
    case (ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT):
    {
        ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Event type ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT");
        // Parameter of function is in seconds
        esp_ble_gap_start_scanning(30);
        break;
    }
    case (ESP_GAP_BLE_SCAN_START_COMPLETE_EVT):
    {
        // Scan start complete event to indicate scan start successfully or failed
        if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Scan start failed, error status = %x", param->scan_start_cmpl.status);
            return;
        }

        ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Scan start success");
        break;
    }
    case (ESP_GAP_BLE_SCAN_RESULT_EVT):
    {
        ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Event type ESP_GAP_BLE_SCAN_RESULT_EVT");
        HandleScanResultEvent(param);
    }
    default:
        break;
    }
}

/**
 * @brief Method to handle events for Greenhouse profile
 */
void ClientBluetoothHandler::GreenhouseEventHandler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    ESP_LOGW(CLIENT_BLUETOOTH_HANDLER_TAG, "Event in %s", __func__);

    switch (event)
    {
    case (ESP_GATTC_REG_EVT):
    {
        ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "REG_EVT");
        esp_err_t scan_ret = esp_ble_gap_set_scan_params(&ble_scan_params);
        if (scan_ret)
            ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "set scan params error, error code = %x", scan_ret);

        break;
    }
    default:
        break;
    }
}

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Method to check if incoming event is registration event
 */
bool ClientBluetoothHandler::IsRegistrationEvent(esp_gattc_cb_event_t event) const
{
    return event == ESP_GATTC_REG_EVT;
}

/**
 * @brief Handle registration event
 */
bool ClientBluetoothHandler::HandleRegistrationEvent(esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    // Check if registration was successful
    if (param->reg.status != ESP_GATT_OK)
    {
        ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Reg app failed, app_id %04x, status %d\n", param->reg.app_id, param->reg.status);
        return false;
    }

    // Find profile based on ID
    auto profile = std::find_if(mProfilesMap.begin(), mProfilesMap.end(), [&](const std::pair<uint8_t, Component::Bluetooth::ClientGattcProfile> &itr)
                                {
        if (itr.first == param->reg.app_id) {
            return true;
        }

        return false; });

    // If profile with specific ID exist, register Gatts interface to profile structure
    if (profile != mProfilesMap.end())
    {
        ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Register Gatts interfae [%d] to profile with ID: %d", gattc_if, profile->first);
        profile->second.gattc_if = gattc_if;
        return true;
    }

    return false;
}

/**
 * @brief Handle scanning result event
 */
void ClientBluetoothHandler::HandleScanResultEvent(esp_ble_gap_cb_param_t *scanResult)
{
    switch (scanResult->scan_rst.search_evt)
    {
    case (ESP_GAP_SEARCH_INQ_RES_EVT):
    {
        uint8_t adv_name_len{0};
        uint8_t *adv_name = esp_ble_resolve_adv_data(scanResult->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);

        if (adv_name)
        {
            
        }
        /*if (adv_name)
{
    if (strlen(remote_device_name) == adv_name_len && strncmp((char *)adv_name, remote_device_name, adv_name_len) == 0)
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "searched device %s\n", remote_device_name);
        if (connect == false)
        {
            connect = true;
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "connect to the remote device.");
            esp_ble_gap_stop_scanning();
            esp_ble_gattc_open(profileTab[GREEENHOUSE_PROFILE].gattc_if, scan_result->scan_rst.bda, scan_result->scan_rst.ble_addr_type, true);
        }
    }
}*/
        break;
    }
    default:
        break;
    }
}