/* STD includes */
#include <stdio.h>

/* non-volatile flash memory */
#include "nvs_flash.h"

/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"

/* ESP logs library */
#include "esp_log.h"

/* Bluetooth includes */
#include "esp_bt.h"                 //  implements BT controller and VHCI configuration procedures from the host side.
#include "esp_bt_main.h"            //  implements initialization and enabling of the Bluedroid stack.
#include "esp_gap_ble_api.h"        //  implements GAP configuration, such as advertising and connection parameters.
#include "esp_gatts_api.h"          //  implements GATT configuration, such as creating services and characteristics.
#include "esp_gatt_common_api.h"

/****** DEFINE *********/
#define NUMBER_OF_PROFILES 1
#define GREEENHOUSE_PROFILE 0

#define BLUETOOTH_NAME "Greenhouse"

#define GATTS_AIR_SERVICE_UUID      0x00FF
#define GATTS_NUM_HANDLE_TEST_A     4

static uint8_t adv_config_done = 0;
#define adv_config_flag      (1 << 0)
#define scan_rsp_config_flag (1 << 1)

static uint8_t adv_service_uuid128[32] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xEE, 0x00, 0x00, 0x00,
    //second uuid, 32bit, [12], [13], [14], [15] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
};

// The length of adv data must be less than 31 bytes
//static uint8_t test_manufacturer[TEST_MANUFACTURER_DATA_LEN] =  {0x12, 0x23, 0x45, 0x56};
//adv data
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = false,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
// scan response data
static esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = true,
    //.min_interval = 0x0006,
    //.max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr            =
    //.peer_addr_type       =
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

/* Declare profile event handler */
static void gatts_greenhouse_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst gl_profile_tab[NUMBER_OF_PROFILES] = {
    [GREEENHOUSE_PROFILE] = {
        .gatts_cb = gatts_greenhouse_profile_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};

static const char* GATTS_TAG = "Gatt_Server_Greenhouse";


static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) 
{
    ESP_LOGI(GATTS_TAG, "%s with type %d", __func__, event);

    switch (event)
    {
    case (ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT): {
        adv_config_done &= (~adv_config_flag);
        if (adv_config_done == 0){
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    }
    case (ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT): {
        adv_config_done &= (~scan_rsp_config_flag);
        if (adv_config_done == 0){
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    }
    default: {
        ESP_LOGW(GATTS_TAG, "Unhandled event with ID: %d in function %s", event, __func__);
        break;
    }
    }
}

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) 
{
    ESP_LOGI(GATTS_TAG, "%s with event type: %d", __func__, event);

    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT) {
        if (param->reg.status == ESP_GATT_OK) {
            gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
        } else {
            ESP_LOGE(GATTS_TAG, "Reg app failed, app_id %04x, status %d\n", param->reg.app_id, param->reg.status);
            return;
        }
    }


    for (int profileID = 0; profileID < NUMBER_OF_PROFILES; ++profileID) 
    {
        if (gatts_if == ESP_GATT_IF_NONE || gatts_if == gl_profile_tab[profileID].gatts_if) {
            if (!gl_profile_tab[profileID].gatts_cb) {
                ESP_LOGE(GATTS_TAG, "No callback function set for profile with ID [%d].", profileID);
                continue;
            }

            ESP_LOGI(GATTS_TAG, "Calling callback for profile with ID [%d].", profileID);
            gl_profile_tab[profileID].gatts_cb(event, gatts_if, param);
        }
    }
}

static void gatts_greenhouse_profile_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) 
{
    switch (event)
    {
        case (ESP_GATTS_REG_EVT): {
           ESP_LOGI(GATTS_TAG, "Regsiter application with ID [%d] and status %d", param->reg.app_id, param->reg.status); 
            gl_profile_tab[GREEENHOUSE_PROFILE].service_id.is_primary = true;
            gl_profile_tab[GREEENHOUSE_PROFILE].service_id.id.inst_id = 0x00;
            gl_profile_tab[GREEENHOUSE_PROFILE].service_id.id.uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[GREEENHOUSE_PROFILE].service_id.id.uuid.uuid.uuid16 = GATTS_AIR_SERVICE_UUID;

            esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(BLUETOOTH_NAME);
            if (set_dev_name_ret){
                ESP_LOGE(GATTS_TAG, "set device name failed, error code = %x", set_dev_name_ret);
            }

           //config adv data
            esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
            if (ret){
                ESP_LOGE(GATTS_TAG, "config adv data failed, error code = %x", ret);
            }
            adv_config_done |= adv_config_flag;
        
            //config scan response data
            ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
            if (ret){
                ESP_LOGE(GATTS_TAG, "config scan response data failed, error code = %x", ret);
            }
            adv_config_done |= scan_rsp_config_flag;

            esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[GREEENHOUSE_PROFILE].service_id, GATTS_NUM_HANDLE_TEST_A);
            break;
        }
        case (ESP_GATTS_CREATE_EVT): 
        {
            break;
        }
        default: {
            ESP_LOGW(GATTS_TAG, "Unhandled event with ID: %d", event);
            break;
        }
    }
}

extern "C" void app_main(void)
{
    // Initialize NVS.
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        result = nvs_flash_init();
    }

    // Check result of initialization non-volatile flash memory
    ESP_ERROR_CHECK(result);

    // Check result of bluetooth controller to memory release of classic bluetooth
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    // Initialization of default config
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    result = esp_bt_controller_init(&bt_cfg);
    if (result) {
        ESP_LOGE(GATTS_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(result));
        return;
    }

    // Enable bluettoth controller to Bluetooth low energy mode
    result = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (result) {
        ESP_LOGE(GATTS_TAG, "%s enable controller failed\n", __func__);
        return;
    }

    // Initialize bluedroid
    result = esp_bluedroid_init();
    if (result) {
        ESP_LOGE(GATTS_TAG, "%s init bluetooth failed\n", __func__);
        return;
    }

    // Enable bluedroid
    result = esp_bluedroid_enable();
    if (result) {
        ESP_LOGE(GATTS_TAG, "%s enable bluetooth failed\n", __func__);
        return;
    }

    // Register callback for bluetotth generic attribites
    result = esp_ble_gatts_register_callback(gatts_event_handler);
    if (result){
        ESP_LOGE(GATTS_TAG, "gatts register error, error code = %x", result);
        return;
    }

    result = esp_ble_gap_register_callback(gap_event_handler);
    if (result){
        ESP_LOGE(GATTS_TAG, "gap register error, error code = %x", result);
        return;
    }

    result = esp_ble_gatts_app_register(GREEENHOUSE_PROFILE);
    if (result){
        ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", result);
        return;
    }

    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(512);
    if (local_mtu_ret){
        ESP_LOGE(GATTS_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
    }

    while (true)
    {
    }
}
