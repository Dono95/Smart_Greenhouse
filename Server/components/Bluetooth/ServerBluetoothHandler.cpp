/* Project specific includes */
#include "ServerBluetoothHandler.hpp"
#include "GreenhouseManager.hpp"
#include "Managers/EventManager.hpp"

/* ESP log library */
#include "esp_log.h"

/* STL includes */
#include "algorithm"
#include <cstring>

// Include FreeRTOS for delay
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

using namespace Greenhouse::Bluetooth;

/**
 * @brief Class constructor with controller parameter
 */
ServerBluetoothHandler::ServerBluetoothHandler(std::weak_ptr<ServerBluetoothController> controller)
    : mConfigAdvDataSet{false},
      mConfigResAdvDataSet{false}
{
    SetBluetoothController(controller);
}

/**
 * @brief Class destructor
 */
ServerBluetoothHandler::~ServerBluetoothHandler()
{
}

/**
 * @brief Method to initialize bluetooth profiles on server side
 */
bool ServerBluetoothHandler::InitializeBluetoothProfiles()
{
    mProfilesMap.insert({GREENHOUSE_PROFILE, {.gatts_cb = [](esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
                                              {
                                                  const auto manager = GreenhouseManager::GetInstance();

                                                  if (!manager)
                                                      return;

                                                  const auto serverHandler = std::dynamic_pointer_cast<ServerBluetoothHandler>(manager->GetHandler());
                                                  if (serverHandler)
                                                      serverHandler->GreenhouseEventHandler(event, gatts_if, param);
                                              },
                                              .gatts_if = ESP_GATT_IF_NONE}});

    return true;
}

/**
 * @brief Method to handle event that are pushed from BLE stack
 */
void ServerBluetoothHandler::HandleGapEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "[%s] Event: %s.", __func__, Component::Bluetooth::EnumToString(event).c_str());

    switch (event)
    {
    case (ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT):
    {
        if (mConfigAdvDataSet)
        {
            if (auto controller = GetBluetoothController().lock())
                controller->StartAdvertising(&adv_params);
        }
        break;
    }
    case (ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT):
    {
        if (mConfigResAdvDataSet)
        {
            if (auto controller = GetBluetoothController().lock())
                controller->StartAdvertising(&adv_params);
        }
        break;
    }
    case (ESP_GAP_BLE_ADV_START_COMPLETE_EVT):
    {
        // advertising start complete event to indicate advertising start successfully or failed
        if (param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS)
            ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Advertising start successful");
        else
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "Advertising start failed!");
        break;
    }
    case (ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT):
        /*ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                 param->update_conn_params.status,
                 param->update_conn_params.min_int,
                 param->update_conn_params.max_int,
                 param->update_conn_params.conn_int,
                 param->update_conn_params.latency,
                 param->update_conn_params.timeout);*/
        break;
    default:
    {
        ESP_LOGW(SERVER_BLUETOOTH_HANDLER_TAG, "Unhandled event [%s] in function %s", Component::Bluetooth::EnumToString(event).c_str(), __func__);
        break;
    }
    }
}

/**
 * @brief Method to handle events from gatts interface on server side
 */
void ServerBluetoothHandler::HandleGattsEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    if (GetBluetoothController().expired())
    {
        ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "Bluetooth controller is invalid. Unable to continue.");
        return;
    }

    if (IsRegistrationEvent(event))
    {
        if (!HandleRegistrationEvent(gatts_if, param))
            return;
    }

    for (const auto &profile : mProfilesMap)
    {
        if (gatts_if == ESP_GATT_IF_NONE || gatts_if == profile.second.gatts_if)
        {
            if (!profile.second.gatts_cb)
            {
                ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "No callback function set for profile with ID [%d].", profile.first);
                continue;
            }

            ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Calling callback for profile with ID [%d].", profile.first);
            profile.second.gatts_cb(event, gatts_if, param);
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
    auto profile = std::find_if(mProfilesMap.begin(), mProfilesMap.end(), [&](const std::pair<uint8_t, Component::Bluetooth::ServerGattsProfile> &itr)
                                {
        if (itr.first == param->reg.app_id) {
            return true;
        }

        return false; });

    // If profile with specific ID exist, register Gatts interface to profile structure
    if (profile != mProfilesMap.end())
    {
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Register Gatts interfae [%d] to profile with ID: %d", gatts_if, profile->first);
        profile->second.gatts_if = gatts_if;
        return true;
    }

    return false;
}

/**
 * @brief Method to handle events for Greenhouse profile
 */
void ServerBluetoothHandler::GreenhouseEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "[%s] Event: %s.", __func__, Component::Bluetooth::EnumToString(event).c_str());

    switch (event)
    {
    case ESP_GATTS_REG_EVT:
    {
        mProfilesMap.at(GREENHOUSE_PROFILE).service_id.is_primary = true;
        mProfilesMap.at(GREENHOUSE_PROFILE).service_id.id.inst_id = 0x00;
        mProfilesMap.at(GREENHOUSE_PROFILE).service_id.id.uuid.len = ESP_UUID_LEN_16;
        mProfilesMap.at(GREENHOUSE_PROFILE).service_id.id.uuid.uuid.uuid16 = GATTS_AIR_SERVICE_UUID;

        auto controller = GetBluetoothController().lock();
        if (!controller)
        {
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "Invalid pointer to bluetooth controller.");
            return;
        }

        controller->SetDeviceName(BLUETOOTH_NAME);

        // Set config adv data
        mConfigAdvDataSet = controller->SetConfigAdvData(&adv_data) == ESP_OK;

        // Set config scan response data
        mConfigResAdvDataSet = controller->SetScanResponseData(&scan_rsp_data) == ESP_OK;

        // Create service
        controller->CreateService(gatts_if, &mProfilesMap.at(GREENHOUSE_PROFILE).service_id, 4);
        break;
    }
    case ESP_GATTS_READ_EVT:
    {
        // ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
        break;
    }
    case ESP_GATTS_WRITE_EVT:
    {
        auto eventManager = Manager::EventManager::GetInstance();
        eventManager->Notify(Greenhouse::Manager::EventManager::Event_T::BLUETOOTH_DATA_RECEIVED);
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);

        // sleep(3);
        /*ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);

        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);*/
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Goint to sleep.");
        vTaskDelay(10000);
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Woke up");
        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        // ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\n", param->write.conn_id, param->write.trans_id, param->write.handle);
        break;
    case ESP_GATTS_MTU_EVT:
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "MTU set to %d.", param->mtu.mtu);
        break;
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
    {
        // ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
        mProfilesMap.at(GREENHOUSE_PROFILE).service_handle = param->create.service_handle;
        mProfilesMap.at(GREENHOUSE_PROFILE).char_uuid.len = ESP_UUID_LEN_16;
        mProfilesMap.at(GREENHOUSE_PROFILE).char_uuid.uuid.uuid16 = 0xFF01;

        if (auto connector = GetBluetoothController().lock())
        {
            connector->StartService(mProfilesMap.at(GREENHOUSE_PROFILE).service_handle);
            connector->AddCharacteristicToService(mProfilesMap.at(GREENHOUSE_PROFILE).service_handle,
                                                  &mProfilesMap.at(GREENHOUSE_PROFILE).char_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                  ESP_GATT_CHAR_PROP_BIT_WRITE,
                                                  &char_value);
        }

        break;
    }
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT:
    {
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        mProfilesMap.at(GREENHOUSE_PROFILE).char_handle = param->add_char.attr_handle;
        mProfilesMap.at(GREENHOUSE_PROFILE).descr_uuid.len = ESP_UUID_LEN_16;
        mProfilesMap.at(GREENHOUSE_PROFILE).descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;

        auto controller = GetBluetoothController().lock();
        if (!controller)
            return;

        std::vector<uint8_t> attrValues;
        controller->GetAttributeValues(param->add_char.attr_handle, attrValues);

        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "The gatts char length = %x\n", attrValues.size());
        for (const auto &value : attrValues)
            ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Char value = %x\n", value);

        controller->AddCharacteristicDescriptor(mProfilesMap.at(GREENHOUSE_PROFILE).service_handle,
                                                &mProfilesMap.at(GREENHOUSE_PROFILE).descr_uuid,
                                                ESP_GATT_PERM_WRITE);
        break;
    }
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        mProfilesMap.at(GREENHOUSE_PROFILE).descr_handle = param->add_char_descr.attr_handle;
        /*ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                 param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);*/
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Service start with status %d, service_handle %d\n", param->start.status, param->start.service_handle);
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT:
    {
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        conn_params.latency = 0;
        conn_params.max_int = 0x20; // max_int = 0x20*1.25ms = 40ms
        conn_params.min_int = 0x10; // min_int = 0x10*1.25ms = 20ms
        conn_params.timeout = 400;  // timeout = 400*10ms = 4000ms
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Connection ID: %d, Remote bluetooth device address: %02x:%02x:%02x:%02x:%02x:%02x:",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
        mProfilesMap.at(GREENHOUSE_PROFILE).conn_id = param->connect.conn_id;

        // start sent the update connection parameters to the peer device.
        if (auto connector = GetBluetoothController().lock())
            connector->UpdateConParameteres(&conn_params);

        // esp_ble_gap_start_advertising(&adv_params);
        break;
    }
    case ESP_GATTS_DISCONNECT_EVT:
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "Client disconnected. The reason is 0x%x", param->disconnect.reason);

        if (auto connector = GetBluetoothController().lock())
            connector->StartAdvertising(&adv_params);

        break;
    case ESP_GATTS_CONF_EVT:
        /*ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "ESP_GATTS_CONF_EVT, status %d attr_handle %d", param->conf.status, param->conf.handle);
        if (param->conf.status != ESP_GATT_OK)
        {
            esp_log_buffer_hex(SERVER_BLUETOOTH_HANDLER_TAG, param->conf.value, param->conf.len);
        }*/
        break;
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    default:
        break;
    }
}