/**
 * Definition of BluetoothController
 *
 * @author Dominik Regec
 */
#ifndef BLUETOOTH_CONTROLLER
#define BLUETOOTH_CONTROLLER

/* STL includes */
#include <vector>
#include <cstring>


/* Bluetooth includes */
#include "esp_bt.h"          //  implements BT controller and VHCI configuration procedures from the host side.
#include "esp_bt_main.h"     //  implements initialization and enabling of the Bluedroid stack.
#include "esp_gap_ble_api.h" //  implements GAP configuration, such as advertising and connection parameters.
#include "esp_gatts_api.h"   //  implements GATT configuration, such as creating services and characteristics.
#include "esp_gatt_common_api.h"

/******     DEFINES     *********/
#define NUMBER_OF_PROFILES 1
#define GREEENHOUSE_PROFILE 0

#define BLUETOOTH_NAME "Greenhouse"

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

const static char *BLUETOOTH_CONTROLLER_TAG = "BluetoothController";

namespace Greenhouse
{
    struct gatts_profile_inst
    {
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
        0xEE,
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

    // The length of adv data must be less than 31 bytes
    // static uint8_t test_manufacturer[TEST_MANUFACTURER_DATA_LEN] =  {0x12, 0x23, 0x45, 0x56};
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
        .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
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

    class BluetoothController
    {
    public:
        /* Enum to define return value of the initialization process */
        enum class INIT_BLUETOOTH_RV
        {
            RV_BLUETOOTH_OFF,             // <- Bluetooth is turn off
            RV_BLUETOOTH_INIT_OK,         // <- Initialize of bluetooth was successful
            RV_BLUETOOTH_INIT_FAILED,     // <- Initialize of bluetooth controlled failed
            RV_BLUETOOTH_SET_MODE_FAILED, // <- Set bluetooth mode failed
            RV_BLUEDROID_FAILED,          // <- Initialize of enabling of bluedroid failed
            RV_REGISTER_CALLBACK_FAILED,  // <- Registration of callback failed
            RV_REGISTER_APP_FAILED,       // <- Registration of app failed
            RV_SET_LOCAL_MTU_FAILED,      // <- Setting local mtu failed
        };

        /**
         * @brief Class constructor
         */
        explicit BluetoothController();

        /**
         * @brief Class destructor
         */
        ~BluetoothController();

        /**
         * @brief Method to initialize bluetooth controller. Set bluetooth mode and register
         * callbacks for gap and gatts
         *
         * @param bluetoothMode[in]     Bluetooth mode
         *
         * @return INIT_BLUETOOTH_RV -> Return value based on initialization progress
         *                           -> RV_BLUETOOTH_INIT_OK - Succeeded, Othrewise - Failed
         */
        INIT_BLUETOOTH_RV InitBluetoothController(const esp_bt_mode_t bluetoothMode);

        /**
         * @brief The method to handle event for Greenhouse profile
         *
         * @param event     -> Event from BLE stack
         * @param gatts_if  -> Gatts interface
         * @param param     -> Pointer to parameters of gatts
         */
        static void GreenhouseProfileEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

    private:
        /**
         * @brief Method to initialize ESP bluetooth controller.
         *
         * @param bluetoothMode[in]     Bluetooth mode
         *
         * @return INIT_BLUETOOTH_RV -> Return value based on initialization progress
         *                           -> RV_BLUETOOTH_INIT_OK - Succeeded, Othrewise - Failed
         */
        INIT_BLUETOOTH_RV InitESP_BluetoothController(const esp_bt_mode_t bluetoothMode);

        /**
         * @brief Method to initialize ESP bluedroid.
         *
         * @return INIT_BLUETOOTH_RV -> Return value based on initialization progress
         *                           -> RV_BLUETOOTH_INIT_OK - Succeeded, Othrewise - Failed
         */
        INIT_BLUETOOTH_RV InitBluedroid(void);

        /**
         * @brief The method to handle event that are pushed from BLE stack
         *
         * @param event     -> Event from BLE stack
         * @param gatts_if  -> Gatts interface
         * @param param     -> Pointer to parameters of gatts
         */
        static void GattsEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

        /**
         * @brief The method to handle event that are pushed from BLE stack
         *
         * @param event     -> Event from BLE stack
         * @param param     -> Pointer to parameters of gatts
         */
        static void GapEventHandler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
    };

    /* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
    static struct gatts_profile_inst profileTab[NUMBER_OF_PROFILES] = {
        [GREEENHOUSE_PROFILE] = {
            .gatts_cb = BluetoothController::GreenhouseProfileEventHandler,
            .gatts_if = ESP_GATT_IF_NONE, // Not get the gatt_if, so initial is ESP_GATT_IF_NONE
        },
    };
} // namespace Greenhouse

#endif // BLUETOOTH_CONTROLLER