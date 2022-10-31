/**
 * Definition of Client bluetooth controller
 *
 * @author Dominik Regec
 */
#ifndef CLIENT_BLUETOOTH_CONTROLLER
#define CLIENT_BLUETOOTH_CONTROLLER

/* Common components */
#include "Bluetooth/BaseBluetoothController.hpp"

/* Define class log tag */
#define CLIENT_BLUETOOTH_CONTROLLER_TAG "ClientBluetoothController"

namespace Greenhouse
{
    namespace Bluetooth
    {
        class ClientBluetoothControlller : public Component::Bluetooth::BaseBluetoothController
        {
        public:
            /* Typedef for incialization bluetooth return code*/
            using INIT_BLE_STATUS = Component::Bluetooth::INIT_BLUETOOTH_RV;

            /**
             * @brief Class contrustor
             */
            explicit ClientBluetoothControlller();

            /**
             * @brief Class destructor
             */
            ~ClientBluetoothControlller();

            /**
             * @brief Overridden method to register gap and gatts callback for client controller
             */ 
            virtual INIT_BLE_STATUS RegisterCallbacks(void) override;
        };
    } // namespace Bluetooth
} // namespace Greenhouse

/* STL includes */
//#include <cstring>

/******     DEFINES     *********/
//#define NUMBER_OF_PROFILES 1
//#define INVALID_HANDLE 0

//#define REMOTE_SERVICE_UUID 0x00FF
//#define REMOTE_NOTIFY_CHAR_UUID 0xFF01

//static const char remote_device_name[] = "Greenhouse";
//static bool connect = false;
//static bool get_server = false;
//static esp_gattc_char_elem_t *char_elem_result = NULL;
//static esp_gattc_descr_elem_t *descr_elem_result = NULL;

//#define GATTS_AIR_SERVICE_UUID 0x00FF
//#define GATTS_NUM_HANDLE_TEST_A 4
//#define GATTS_CHAR_UUID_TEST_A 0xFF01

//#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40

//const static char *BLUETOOTH_CONTROLLER_TAG = "BluetoothController";

namespace Greenhouse
{
    /*static esp_bt_uuid_t remote_filter_service_uuid = {
        .len = ESP_UUID_LEN_16,
        .uuid = {
            .uuid16 = REMOTE_SERVICE_UUID,
        },
    };

    static esp_bt_uuid_t remote_filter_char_uuid = {
        .len = ESP_UUID_LEN_16,
        .uuid = {
            .uuid16 = REMOTE_NOTIFY_CHAR_UUID,
        },
    };

    static esp_bt_uuid_t notify_descr_uuid = {
        .len = ESP_UUID_LEN_16,
        .uuid = {
            .uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG,
        },
    };

    static esp_ble_scan_params_t ble_scan_params = {
        .scan_type = BLE_SCAN_TYPE_ACTIVE,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
        .scan_interval = 0x50,
        .scan_window = 0x30,
        .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE};

    struct gattc_profile_inst
    {
        esp_gattc_cb_t gattc_cb;
        uint16_t gattc_if;
        uint16_t app_id;
        uint16_t conn_id;
        uint16_t service_start_handle;
        uint16_t service_end_handle;
        uint16_t char_handle;
        esp_bd_addr_t remote_bda;
    };*/

} // namespace Greenhouse

#endif // CLIENT_BLUETOOTH_CONTROLLER