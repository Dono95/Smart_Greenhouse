/* Project specific includes */
#include "ClientBluetoothController.hpp"

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
ClientBluetoothControlller::ClientBluetoothControlller()
{
}

/**
 * @brief Class destructor
 */
ClientBluetoothControlller::~ClientBluetoothControlller()
{
}

/**
 * @brief Method to register gap and gatts callback
 */
esp_err_t ClientBluetoothControlller::RegisterCallback(void)
{
    /*auto GapCallback = [](esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
    {
        const auto manager = GreenhouseManager::GetInstance();
        if (manager)
            manager->GetBluetoothHandler()->HandleGapEvent(event, param);
    };

    esp_err_t registerResult = esp_ble_gap_register_callback(BluetoothController::GapEventHandler);
    if (registerResult)
    {
        ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "gap register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gattc_register_callback(BluetoothController::GattcEventHandler);
    if (registerResult)
    {
        ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "gatts register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gattc_app_register(GREEENHOUSE_PROFILE);
    if (registerResult)
    {
        ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "gatts app register failed, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_APP_FAILED;
    }*/
    return ESP_OK;
}

/**
 * @brief Method to initialize bluetooth controller. Set bluetooth mode and register
 * callbacks for gap and gatts
 */
/*BluetoothController::INIT_BLUETOOTH_RV BluetoothController::InitBluetoothController(esp_bt_mode_t bluetoothMode)
{
    // TO DO -> Need to release opposite mode from bluetoothMode
    // For now programmer is responsible to use only ESP_BT_MODE_BLE

    // Check result of bluetooth controller to memory release of classic bluetooth
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    INIT_BLUETOOTH_RV result = InitESP_BluetoothController(bluetoothMode);
    if (result != INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK)
        return result;

    result = InitBluedroid();
    if (result != INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK)
        return result;

    // Test lambda [](esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {}
    esp_err_t registerResult = esp_ble_gap_register_callback(BluetoothController::GapEventHandler);
    if (registerResult)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gap register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gattc_register_callback(BluetoothController::GattcEventHandler);
    if (registerResult)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gatts register error, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gattc_app_register(GREEENHOUSE_PROFILE);
    if (registerResult)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gatts app register failed, error code = %x", registerResult);
        return INIT_BLUETOOTH_RV::RV_REGISTER_APP_FAILED;
    }

    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(512);
    if (local_mtu_ret)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
        return INIT_BLUETOOTH_RV::RV_SET_LOCAL_MTU_FAILED;
    }

    return INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK;
}*/

/**
 * @brief The method to handle event for Greenhouse profile
 */
/*void BluetoothController::GreenhouseProfileEventHandler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    esp_ble_gattc_cb_param_t *p_data = (esp_ble_gattc_cb_param_t *)param;

    switch (event)
    {
    case ESP_GATTC_REG_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "REG_EVT");
        esp_err_t scan_ret = esp_ble_gap_set_scan_params(&ble_scan_params);
        if (scan_ret)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "set scan params error, error code = %x", scan_ret);
        }
        break;
    }
    case ESP_GATTC_CONNECT_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTC_CONNECT_EVT conn_id %d, if %d", p_data->connect.conn_id, gattc_if);
        profileTab[GREEENHOUSE_PROFILE].conn_id = p_data->connect.conn_id;
        memcpy(profileTab[GREEENHOUSE_PROFILE].remote_bda, p_data->connect.remote_bda, sizeof(esp_bd_addr_t));
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "REMOTE BDA:");
        esp_log_buffer_hex(BLUETOOTH_CONTROLLER_TAG, profileTab[GREEENHOUSE_PROFILE].remote_bda, sizeof(esp_bd_addr_t));
        esp_err_t mtu_ret = esp_ble_gattc_send_mtu_req(gattc_if, p_data->connect.conn_id);
        if (mtu_ret)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "config MTU error, error code = %x", mtu_ret);
        }
        break;
    }
    case ESP_GATTC_OPEN_EVT:
    {
        //if (param->open.status != ESP_GATT_OK){
        //    ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "open failed, status %d", p_data->open.status);
        //    break;
        //}
        //ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "open success");
        break;
    }
    case ESP_GATTC_DIS_SRVC_CMPL_EVT:
    {
        //if (param->dis_srvc_cmpl.status != ESP_GATT_OK){
        //    ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "discover service failed, status %d", param->dis_srvc_cmpl.status);
        //    break;
        //}
        //ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "discover service complete conn_id %d", param->dis_srvc_cmpl.conn_id);
        //esp_ble_gattc_search_service(gattc_if, param->cfg_mtu.conn_id, &remote_filter_service_uuid);
        break;
    }
    case ESP_GATTC_CFG_MTU_EVT:
    {
        //if (param->cfg_mtu.status != ESP_GATT_OK)
        //{
        //    ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "config mtu failed, error status = %x", param->cfg_mtu.status);
        //}
        //ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTC_CFG_MTU_EVT, Status %d, MTU %d, conn_id %d", param->cfg_mtu.status, param->cfg_mtu.mtu, param->cfg_mtu.conn_id);
        break;
    }
    case ESP_GATTC_SEARCH_RES_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "SEARCH RES: conn_id = %x is primary service %d", p_data->search_res.conn_id, p_data->search_res.is_primary);
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "start handle %d end handle %d current handle value %d", p_data->search_res.start_handle, p_data->search_res.end_handle, p_data->search_res.srvc_id.inst_id);
        if (p_data->search_res.srvc_id.uuid.len == ESP_UUID_LEN_16 && p_data->search_res.srvc_id.uuid.uuid.uuid16 == REMOTE_SERVICE_UUID)
        {
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "service found");
            get_server = true;
            profileTab[GREEENHOUSE_PROFILE].service_start_handle = p_data->search_res.start_handle;
            profileTab[GREEENHOUSE_PROFILE].service_end_handle = p_data->search_res.end_handle;
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "UUID16: %x", p_data->search_res.srvc_id.uuid.uuid.uuid16);
        }
        break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT:
    {
        if (p_data->search_cmpl.status != ESP_GATT_OK)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "search service failed, error status = %x", p_data->search_cmpl.status);
            break;
        }
        if (p_data->search_cmpl.searched_service_source == ESP_GATT_SERVICE_FROM_REMOTE_DEVICE)
        {
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "Get service information from remote device");
        }
        else if (p_data->search_cmpl.searched_service_source == ESP_GATT_SERVICE_FROM_NVS_FLASH)
        {
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "Get service information from flash");
        }
        else
        {
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "unknown service source");
        }
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTC_SEARCH_CMPL_EVT");
        if (get_server)
        {
            uint16_t count = 0;
            esp_gatt_status_t status = esp_ble_gattc_get_attr_count(gattc_if,
                                                                    p_data->search_cmpl.conn_id,
                                                                    ESP_GATT_DB_CHARACTERISTIC,
                                                                    profileTab[GREEENHOUSE_PROFILE].service_start_handle,
                                                                    profileTab[GREEENHOUSE_PROFILE].service_end_handle,
                                                                    INVALID_HANDLE,
                                                                    &count);
            if (status != ESP_GATT_OK)
            {
                ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "esp_ble_gattc_get_attr_count error");
            }

            if (count > 0)
            {
                char_elem_result = (esp_gattc_char_elem_t *)malloc(sizeof(esp_gattc_char_elem_t) * count);
                if (!char_elem_result)
                {
                    ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "gattc no mem");
                }
                else
                {
                    status = esp_ble_gattc_get_char_by_uuid(gattc_if,
                                                            p_data->search_cmpl.conn_id,
                                                            profileTab[GREEENHOUSE_PROFILE].service_start_handle,
                                                            profileTab[GREEENHOUSE_PROFILE].service_end_handle,
                                                            remote_filter_char_uuid,
                                                            char_elem_result,
                                                            &count);
                    if (status != ESP_GATT_OK)
                    {
                        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "esp_ble_gattc_get_char_by_uuid error");
                    }

                    //  Every service have only one char in our 'ESP_GATTS_DEMO' demo, so we used first 'char_elem_result'
                    if (count > 0 && (char_elem_result[0].properties & ESP_GATT_CHAR_PROP_BIT_NOTIFY))
                    {
                        profileTab[GREEENHOUSE_PROFILE].char_handle = char_elem_result[0].char_handle;
                        esp_ble_gattc_register_for_notify(gattc_if, profileTab[GREEENHOUSE_PROFILE].remote_bda, char_elem_result[0].char_handle);
                    }
                }
                // free char_elem_result
                free(char_elem_result);
            }
            else
            {
                ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "no char found");
            }
        }
        break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTC_REG_FOR_NOTIFY_EVT");
        if (p_data->reg_for_notify.status != ESP_GATT_OK)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "REG FOR NOTIFY failed: error status = %d", p_data->reg_for_notify.status);
        }
        else
        {
            uint16_t count = 0;
            uint16_t notify_en = 1;
            esp_gatt_status_t ret_status = esp_ble_gattc_get_attr_count(gattc_if,
                                                                        profileTab[GREEENHOUSE_PROFILE].conn_id,
                                                                        ESP_GATT_DB_DESCRIPTOR,
                                                                        profileTab[GREEENHOUSE_PROFILE].service_start_handle,
                                                                        profileTab[GREEENHOUSE_PROFILE].service_end_handle,
                                                                        profileTab[GREEENHOUSE_PROFILE].char_handle,
                                                                        &count);*/
/*if (ret_status != ESP_GATT_OK){
    ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "esp_ble_gattc_get_attr_count error");
}
if (count > 0){
    descr_elem_result = malloc(sizeof(esp_gattc_descr_elem_t) * count);
    if (!descr_elem_result){
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "malloc error, gattc no mem");
    }else{
        ret_status = esp_ble_gattc_get_descr_by_char_handle( gattc_if,
                                                             profileTab[GREEENHOUSE_PROFILE].conn_id,
                                                             p_data->reg_for_notify.handle,
                                                             notify_descr_uuid,
                                                             descr_elem_result,
                                                             &count);
        if (ret_status != ESP_GATT_OK){
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "esp_ble_gattc_get_descr_by_char_handle error");
        }
        // Every char has only one descriptor in our 'ESP_GATTS_DEMO' demo, so we used first 'descr_elem_result'
        if (count > 0 && descr_elem_result[0].uuid.len == ESP_UUID_LEN_16 && descr_elem_result[0].uuid.uuid.uuid16 == ESP_GATT_UUID_CHAR_CLIENT_CONFIG){
            ret_status = esp_ble_gattc_write_char_descr( gattc_if,
                                                        profileTab[GREEENHOUSE_PROFILE].conn_id,
                                                         descr_elem_result[0].handle,
                                                         sizeof(notify_en),
                                                         (uint8_t *)&notify_en,
                                                         ESP_GATT_WRITE_TYPE_RSP,
                                                         ESP_GATT_AUTH_REQ_NONE);
        }

        if (ret_status != ESP_GATT_OK){
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "esp_ble_gattc_write_char_descr error");
        }

        // free descr_elem_result
        free(descr_elem_result);
    }
}
else{
    ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "decsr not found");
}*/
//}
// break;
/*}
case ESP_GATTC_NOTIFY_EVT:
{
    if (p_data->notify.is_notify)
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTC_NOTIFY_EVT, receive notify value:");
    }
    else
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTC_NOTIFY_EVT, receive indicate value:");
    }
    esp_log_buffer_hex(BLUETOOTH_CONTROLLER_TAG, p_data->notify.value, p_data->notify.value_len);
    break;
}
case ESP_GATTC_WRITE_DESCR_EVT:
{
    if (p_data->write.status != ESP_GATT_OK)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "write descr failed, error status = %x", p_data->write.status);
        break;
    }
    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "write descr success ");
    uint8_t write_char_data[35];
    for (int i = 0; i < sizeof(write_char_data); ++i)
    {
        write_char_data[i] = i % 256;
    }
    esp_ble_gattc_write_char(gattc_if,
                             profileTab[GREEENHOUSE_PROFILE].conn_id,
                             profileTab[GREEENHOUSE_PROFILE].char_handle,
                             sizeof(write_char_data),
                             write_char_data,
                             ESP_GATT_WRITE_TYPE_RSP,
                             ESP_GATT_AUTH_REQ_NONE);
    break;
}
case ESP_GATTC_SRVC_CHG_EVT:
{
    esp_bd_addr_t bda;
    memcpy(bda, p_data->srvc_chg.remote_bda, sizeof(esp_bd_addr_t));
    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTC_SRVC_CHG_EVT, bd_addr:");
    esp_log_buffer_hex(BLUETOOTH_CONTROLLER_TAG, bda, sizeof(esp_bd_addr_t));
    break;
}
case ESP_GATTC_WRITE_CHAR_EVT:
{
    if (p_data->write.status != ESP_GATT_OK)
    {
        ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "write char failed, error status = %x", p_data->write.status);
        break;
    }
    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "write char success ");
    break;
}
case ESP_GATTC_DISCONNECT_EVT:
{
    connect = false;
    get_server = false;
    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTC_DISCONNECT_EVT, reason = %d", p_data->disconnect.reason);
    break;
}
default:
    break;
}
}*/

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Method to initialize ESP bluetooth controller.
 */
/*BluetoothController::INIT_BLUETOOTH_RV BluetoothController::InitESP_BluetoothController(const esp_bt_mode_t bluetoothMode)
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
}*/

/**
 * @brief Method to initialize ESP bluedroid.
 */
/*BluetoothController::INIT_BLUETOOTH_RV BluetoothController::InitBluedroid(void)
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
}*/

/**
 * @brief The method to handle event that are pushed from BLE stack
 */
/*void BluetoothController::GattcEventHandler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    // If event is register event, store the gattc_if for each profile
    if (event == ESP_GATTC_REG_EVT)
    {
        if (param->reg.status == ESP_GATT_OK)
        {
            profileTab[param->reg.app_id].gattc_if = gattc_if;
        }
        else
        {
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "reg app failed, app_id %04x, status %d",
                     param->reg.app_id,
                     param->reg.status);
            return;
        }
    }*/

/* If the gattc_if equal to profile A, call profile A cb handler,
 * so here call each profile's callback */
/*do
{
    int idx;
    for (idx = 0; idx < NUMBER_OF_PROFILES; idx++)
    {
        if (gattc_if == ESP_GATT_IF_NONE ||  ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
/*gattc_if == profileTab[idx].gattc_if)
{
if (profileTab[idx].gattc_cb)
{
    profileTab[idx].gattc_cb(event, gattc_if, param);
}
}
}
} while (0);
}*/

/**
 * @brief The method to handle event that are pushed from BLE stack
 */
/*void BluetoothController::GapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    uint8_t *adv_name = NULL;
    uint8_t adv_name_len = 0;
    switch (event)
    {
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "Event type ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT");
        // the unit of the duration is second
        uint32_t duration = 30;
        esp_ble_gap_start_scanning(duration);
        break;
    }
    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
        // scan start complete event to indicate scan start successfully or failed
        if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "scan start failed, error status = %x", param->scan_start_cmpl.status);
            break;
        }
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "scan start success");

        break;
    case ESP_GAP_BLE_SCAN_RESULT_EVT:
    {
        esp_ble_gap_cb_param_t *scan_result = (esp_ble_gap_cb_param_t *)param;
        switch (scan_result->scan_rst.search_evt)
        {
        case ESP_GAP_SEARCH_INQ_RES_EVT:
            esp_log_buffer_hex(BLUETOOTH_CONTROLLER_TAG, scan_result->scan_rst.bda, 6);
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "searched Adv Data Len %d, Scan Response Len %d", scan_result->scan_rst.adv_data_len, scan_result->scan_rst.scan_rsp_len);
            adv_name = esp_ble_resolve_adv_data(scan_result->scan_rst.ble_adv,
                                                ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "searched Device Name Len %d", adv_name_len);
            esp_log_buffer_char(BLUETOOTH_CONTROLLER_TAG, adv_name, adv_name_len);

#if CONFIG_EXAMPLE_DUMP_ADV_DATA_AND_SCAN_RESP
            if (scan_result->scan_rst.adv_data_len > 0)
            {
                ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "adv data:");
                esp_log_buffer_hex(BLUETOOTH_CONTROLLER_TAG, &scan_result->scan_rst.ble_adv[0], scan_result->scan_rst.adv_data_len);
            }
            if (scan_result->scan_rst.scan_rsp_len > 0)
            {
                ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "scan resp:");
                esp_log_buffer_hex(BLUETOOTH_CONTROLLER_TAG, &scan_result->scan_rst.ble_adv[scan_result->scan_rst.adv_data_len], scan_result->scan_rst.scan_rsp_len);
            }
#endif
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "\n");

            if (adv_name != NULL)
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
            }
            break;
        case ESP_GAP_SEARCH_INQ_CMPL_EVT:
            break;
        default:
            break;
        }
        break;
    }

    case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
    {
        if (param->scan_stop_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "scan stop failed, error status = %x", param->scan_stop_cmpl.status);
            break;
        }
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "stop scan successfully");
        break;
    }
    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
    {
        if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "adv stop failed, error status = %x", param->adv_stop_cmpl.status);
            break;
        }
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "stop adv successfully");
        break;
    }
    case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                 param->update_conn_params.status,
                 param->update_conn_params.min_int,
                 param->update_conn_params.max_int,
                 param->update_conn_params.conn_int,
                 param->update_conn_params.latency,
                 param->update_conn_params.timeout);
        break;
    }
    default:
        break;
    }
}*/
