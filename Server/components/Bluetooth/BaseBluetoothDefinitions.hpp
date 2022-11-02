#ifndef BASE_BLUETOOTH_DEFINITIONS
#define BASE_BLUETOOTH_DEFINITIONS

/* STL includes */
#include "cstdint"

/* Bluetooth includes */
#include "esp_gatts_api.h"
#include "esp_gap_ble_api.h"

namespace Greenhouse
{
    namespace Bluetooth
    {

/******          PROFILES ID DEFINES        *******/
// It is necessary that each profile has a unique ID
#define GREENHOUSE_PROFILE 0

#define GATTS_AIR_SERVICE_UUID 0x00FF
#define GATTS_NUM_HANDLE_TEST_A 4
#define GATTS_CHAR_UUID_TEST_A 0xFF01

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40

                static uint8_t char1_str[] = {0x11, 0x22, 0x33};
        static esp_gatt_char_prop_t a_property = 0;

        static esp_attr_value_t gatts_demo_char1_val =
            {
                .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
                .attr_len = sizeof(char1_str),
                .attr_value = char1_str,
        };

        static uint8_t adv_config_done = 0;
#define adv_config_flag (1 << 0)
#define scan_rsp_config_flag (1 << 1)

        static uint8_t adv_service_uuid128[32] = {
            /* LSB <--------------------------------------------------------------------------------> MSB */
            // first uuid, 16bit, [12],[13] is the value
            0xfb,
            0x34,
            0x9b,
            0x5f,
            0x80,
            0x00,
            0x00,
            0x80,
            0x00,
            0x10,
            0x00,
            0x00,
            0xEC,
            0x00,
            0x00,
            0x00,
            // second uuid, 32bit, [12], [13], [14], [15] is the value
            0xfb,
            0x34,
            0x9b,
            0x5f,
            0x80,
            0x00,
            0x00,
            0x80,
            0x00,
            0x10,
            0x00,
            0x00,
            0xFF,
            0x00,
            0x00,
            0x00,
        };

        // adv data
        static esp_ble_adv_data_t adv_data = {
            .set_scan_rsp = false,
            .include_name = true,
            .include_txpower = false,
            .min_interval = 0x0006, // slave connection min interval, Time = min_interval * 1.25 msec
            .max_interval = 0x0010, // slave connection max interval, Time = max_interval * 1.25 msec
            .appearance = 0x0082,
            .manufacturer_len = 5,       // TEST_MANUFACTURER_DATA_LEN,
            .p_manufacturer_data = NULL, //&test_manufacturer[0],
            .service_data_len = 5,
            .p_service_data = NULL,
            .service_uuid_len = sizeof(adv_service_uuid128),
            .p_service_uuid = adv_service_uuid128,
            .flag = (ESP_BLE_ADV_FLAG_LIMIT_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
        };
        // scan response data
        static esp_ble_adv_data_t scan_rsp_data = {
            .set_scan_rsp = true,
            .include_name = true,
            .include_txpower = true,
            //.min_interval = 0x0006,
            //.max_interval = 0x0010,
            .appearance = 0x0082,
            .manufacturer_len = 5,       // TEST_MANUFACTURER_DATA_LEN,
            .p_manufacturer_data = NULL, //&test_manufacturer[0],
            .service_data_len = 5,
            .p_service_data = NULL,
            .service_uuid_len = sizeof(adv_service_uuid128),
            .p_service_uuid = adv_service_uuid128,
            .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
        };

        static esp_ble_adv_params_t adv_params = {
            .adv_int_min = 0x20,
            .adv_int_max = 0x40,
            .adv_type = ADV_TYPE_IND,
            .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
            //.peer_addr            =
            //.peer_addr_type       =
            .channel_map = ADV_CHNL_ALL,
            .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
        };
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // BASE_BLUETOOTH_DEFINITIONS