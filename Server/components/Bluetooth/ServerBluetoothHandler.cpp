/* Project specific includes */
#include "ServerBluetoothHandler.hpp"
#include "GreenhouseManager.hpp"

/* ESP log library */
#include "esp_log.h"

/* STL includes */
#include "algorithm"

using namespace Greenhouse::Bluetooth;

/**
 * @brief Class constructor with controller parameter
 */
ServerBluetoothHandler::ServerBluetoothHandler(std::weak_ptr<ServerBluetoothController> controller)
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
        /*adv_config_done &= (~adv_config_flag);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&adv_params);
        }*/
        break;
    }
    case (ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT):
    {
        /*adv_config_done &= (~scan_rsp_config_flag);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&adv_params);
        }*/
        break;
    }
    case (ESP_GAP_BLE_ADV_START_COMPLETE_EVT):
    {
        // advertising start complete event to indicate advertising start successfully or failed
        /*if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "Advertising start failed\n");
        }*/
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
        // ESP_LOGW(SERVER_BLUETOOTH_HANDLER_TAG, "Unhandled event with ID: %d in function %s", event, __func__);
        break;
    }
    }
}

/**
 * @brief Method to handle events from gatts interface on server side
 */
void ServerBluetoothHandler::HandleGattsEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
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

        /*ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
        mProfileMap.at(GREENHOUSE_PROFILE).service_id.is_primary = true;
        mProfileMap.at(GREENHOUSE_PROFILE).service_id.id.inst_id = 0x00;
        mProfileMap.at(GREENHOUSE_PROFILE).service_id.id.uuid.len = ESP_UUID_LEN_16;
        mProfileMap.at(GREENHOUSE_PROFILE).service_id.id.uuid.uuid.uuid16 = GATTS_AIR_SERVICE_UUID;

        esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(BLUETOOTH_NAME);
        if (set_dev_name_ret)
        {
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "Set device name failed, error code = %x", set_dev_name_ret);
        }

        // config adv data
        esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
        if (ret)
        {
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "config adv data failed, error code = %x", ret);
        }
        adv_config_done |= adv_config_flag;

        // config scan response data
        ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
        if (ret)
        {
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "config scan response data failed, error code = %x", ret);
        }
        adv_config_done |= scan_rsp_config_flag;

        //esp_ble_gatts_create_service(gatts_if, &profileTab[GREENHOUSE_PROFILE].service_id, GATTS_NUM_HANDLE_TEST_A);
        esp_ble_gatts_create_service(gatts_if, &mProfileMap.at(GREENHOUSE_PROFILE).service_id, GATTS_NUM_HANDLE_TEST_A);*/
        break;
    }
    case ESP_GATTS_READ_EVT:
    {
        // ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
        break;
    }
    case ESP_GATTS_WRITE_EVT:
    {
        /*ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);

        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);*/
        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        // ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\n", param->write.conn_id, param->write.trans_id, param->write.handle);
        break;
    case ESP_GATTS_MTU_EVT:
        // ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
        break;
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
    {
        /*ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
        mProfileMap.at(GREENHOUSE_PROFILE).service_handle = param->create.service_handle;
        mProfileMap.at(GREENHOUSE_PROFILE).char_uuid.len = ESP_UUID_LEN_16;
        mProfileMap.at(GREENHOUSE_PROFILE).char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_A;

        esp_ble_gatts_start_service(mProfileMap.at(GREENHOUSE_PROFILE).service_handle);

        esp_err_t add_char_ret = esp_ble_gatts_add_char(mProfileMap.at(GREENHOUSE_PROFILE).service_handle, &mProfileMap.at(GREENHOUSE_PROFILE).char_uuid,
                                                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                        ESP_GATT_CHAR_PROP_BIT_WRITE,
                                                        &gatts_demo_char1_val, NULL);
        if (add_char_ret)
        {
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "add char failed, error code =%x", add_char_ret);
        }*/
        break;
    }
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT:
    {
        /*uint16_t length = 0;
        const uint8_t *prf_char;

        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        mProfileMap.at(GREENHOUSE_PROFILE).char_handle = param->add_char.attr_handle;
        mProfileMap.at(GREENHOUSE_PROFILE).descr_uuid.len = ESP_UUID_LEN_16;
        mProfileMap.at(GREENHOUSE_PROFILE).descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(param->add_char.attr_handle, &length, &prf_char);
        if (get_attr_ret == ESP_FAIL)
        {
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "ILLEGAL HANDLE");
        }

        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "the gatts demo char length = %x\n", length);
        for (int i = 0; i < length; i++)
        {
            ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "prf_char[%x] =%x\n", i, prf_char[i]);
        }
        esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(mProfileMap.at(GREENHOUSE_PROFILE).service_handle, &mProfileMap.at(GREENHOUSE_PROFILE).descr_uuid,
                                                               ESP_GATT_PERM_WRITE, NULL, NULL);
        if (add_descr_ret)
        {
            ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "add char descr failed, error code =%x", add_descr_ret);
        }*/
        break;
    }
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        /*mProfileMap.at(GREENHOUSE_PROFILE).descr_handle = param->add_char_descr.attr_handle;
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                 param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);*/
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        /*ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
                 param->start.status, param->start.service_handle);*/
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT:
    {
        /*esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        //For the IOS system, please reference the apple official documents about the ble connection parameters restrictions.
        conn_params.latency = 0;
        conn_params.max_int = 0x20; // max_int = 0x20*1.25ms = 40ms
        conn_params.min_int = 0x10; // min_int = 0x10*1.25ms = 20ms
        conn_params.timeout = 400;  // timeout = 400*10ms = 4000ms
        ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
        mProfileMap.at(GREENHOUSE_PROFILE).conn_id = param->connect.conn_id;
        // start sent the update connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);

        //esp_ble_gap_start_advertising(&adv_params);*/
        break;
    }
    case ESP_GATTS_DISCONNECT_EVT:
        /*ESP_LOGI(SERVER_BLUETOOTH_HANDLER_TAG, "ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x", param->disconnect.reason);
        esp_ble_gap_start_advertising(&adv_params);*/
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