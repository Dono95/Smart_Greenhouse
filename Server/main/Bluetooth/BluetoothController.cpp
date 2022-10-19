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

    // Check result of bluetooth controller to memory release of classic bluetooth
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    INIT_BLUETOOTH_RV result = InitESP_BluetoothController(bluetoothMode);
    if (result != INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK)
        return result;

    result = InitBluedroid();
    if (result != INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK)
        return result;

    // Register callback for bluetotth generic attribites
    esp_err_t registerResult = esp_ble_gatts_register_callback(BluetoothController::GattsEventHandler);
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

/**
 * @brief The method to handle event for Greenhouse profile
 */
void BluetoothController::GreenhouseProfileEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    switch (event)
    {
    case ESP_GATTS_REG_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
        profileTab[GREEENHOUSE_PROFILE].service_id.is_primary = true;
        profileTab[GREEENHOUSE_PROFILE].service_id.id.inst_id = 0x00;
        profileTab[GREEENHOUSE_PROFILE].service_id.id.uuid.len = ESP_UUID_LEN_16;
        profileTab[GREEENHOUSE_PROFILE].service_id.id.uuid.uuid.uuid16 = GATTS_AIR_SERVICE_UUID;

        esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(BLUETOOTH_NAME);
        if (set_dev_name_ret)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "set device name failed, error code = %x", set_dev_name_ret);
        }

        // config adv data
        esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
        if (ret)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "config adv data failed, error code = %x", ret);
        }
        adv_config_done |= adv_config_flag;

        // config scan response data
        ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
        if (ret)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "config scan response data failed, error code = %x", ret);
        }
        adv_config_done |= scan_rsp_config_flag;

        esp_ble_gatts_create_service(gatts_if, &profileTab[GREEENHOUSE_PROFILE].service_id, GATTS_NUM_HANDLE_TEST_A);
        break;
    }
    case ESP_GATTS_READ_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
        break;
    }
    case ESP_GATTS_WRITE_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);

        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTS_EXEC_WRITE_EVT");
        break;
    case ESP_GATTS_MTU_EVT:
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
        break;
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
    {
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
        profileTab[GREEENHOUSE_PROFILE].service_handle = param->create.service_handle;
        profileTab[GREEENHOUSE_PROFILE].char_uuid.len = ESP_UUID_LEN_16;
        profileTab[GREEENHOUSE_PROFILE].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_A;

        esp_ble_gatts_start_service(profileTab[GREEENHOUSE_PROFILE].service_handle);
        a_property = ESP_GATT_CHAR_PROP_BIT_WRITE;
        esp_err_t add_char_ret = esp_ble_gatts_add_char(profileTab[GREEENHOUSE_PROFILE].service_handle, &profileTab[GREEENHOUSE_PROFILE].char_uuid,
                                                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                        a_property,
                                                        &gatts_demo_char1_val, NULL);
        if (add_char_ret)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "add char failed, error code =%x", add_char_ret);
        }
        break;
    }
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT:
    {
        uint16_t length = 0;
        const uint8_t *prf_char;

        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        profileTab[GREEENHOUSE_PROFILE].char_handle = param->add_char.attr_handle;
        profileTab[GREEENHOUSE_PROFILE].descr_uuid.len = ESP_UUID_LEN_16;
        profileTab[GREEENHOUSE_PROFILE].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(param->add_char.attr_handle, &length, &prf_char);
        if (get_attr_ret == ESP_FAIL)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "ILLEGAL HANDLE");
        }

        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "the gatts demo char length = %x\n", length);
        for (int i = 0; i < length; i++)
        {
            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "prf_char[%x] =%x\n", i, prf_char[i]);
        }
        esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(profileTab[GREEENHOUSE_PROFILE].service_handle, &profileTab[GREEENHOUSE_PROFILE].descr_uuid,
                                                               ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
        if (add_descr_ret)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "add char descr failed, error code =%x", add_descr_ret);
        }
        break;
    }
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        profileTab[GREEENHOUSE_PROFILE].descr_handle = param->add_char_descr.attr_handle;
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                 param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
                 param->start.status, param->start.service_handle);
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT:
    {
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
        conn_params.latency = 0;
        conn_params.max_int = 0x20; // max_int = 0x20*1.25ms = 40ms
        conn_params.min_int = 0x10; // min_int = 0x10*1.25ms = 20ms
        conn_params.timeout = 400;  // timeout = 400*10ms = 4000ms
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
        profileTab[GREEENHOUSE_PROFILE].conn_id = param->connect.conn_id;
        // start sent the update connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);
        break;
    }
    case ESP_GATTS_DISCONNECT_EVT:
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x", param->disconnect.reason);
        esp_ble_gap_start_advertising(&adv_params);
        break;
    case ESP_GATTS_CONF_EVT:
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ESP_GATTS_CONF_EVT, status %d attr_handle %d", param->conf.status, param->conf.handle);
        if (param->conf.status != ESP_GATT_OK)
        {
            esp_log_buffer_hex(BLUETOOTH_CONTROLLER_TAG, param->conf.value, param->conf.len);
        }
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

/**
 * @brief The method to handle event that are pushed from BLE stack
 */
void BluetoothController::GattsEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "%s with event type: %d", __func__, event);

    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "ID of app is: %d", param->reg.app_id);

    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT)
    {
        if (param->reg.status == ESP_GATT_OK)
            profileTab[param->reg.app_id].gatts_if = gatts_if;
        else
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "Reg app failed, app_id %04x, status %d\n", param->reg.app_id, param->reg.status);
            return;
        }
    }

    for (int profileID = 0; profileID < NUMBER_OF_PROFILES; ++profileID)
    {
        if (gatts_if == ESP_GATT_IF_NONE || gatts_if == profileTab[profileID].gatts_if)
        {
            if (!profileTab[profileID].gatts_cb)
            {
                ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "No callback function set for profile with ID [%d].", profileID);
                continue;
            }

            ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "Calling callback for profile with ID [%d].", profileID);
            profileTab[profileID].gatts_cb(event, gatts_if, param);
        }
    }
}

/**
 * @brief The method to handle event that are pushed from BLE stack
 */
void BluetoothController::GapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "%s with type %d", __func__, event);

    switch (event)
    {
    case (ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT):
    {
        adv_config_done &= (~adv_config_flag);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    }
    case (ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT):
    {
        adv_config_done &= (~scan_rsp_config_flag);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    }
    case (ESP_GAP_BLE_ADV_START_COMPLETE_EVT):
    {
        // advertising start complete event to indicate advertising start successfully or failed
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(BLUETOOTH_CONTROLLER_TAG, "Advertising start failed\n");
        }
        break;
    }
    case (ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT):
        ESP_LOGI(BLUETOOTH_CONTROLLER_TAG, "update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                 param->update_conn_params.status,
                 param->update_conn_params.min_int,
                 param->update_conn_params.max_int,
                 param->update_conn_params.conn_int,
                 param->update_conn_params.latency,
                 param->update_conn_params.timeout);
        break;
    default:
    {
        ESP_LOGW(BLUETOOTH_CONTROLLER_TAG, "Unhandled event with ID: %d in function %s", event, __func__);
        break;
    }
    }
}