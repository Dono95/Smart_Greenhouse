/**
 * Definition of ServerBluetoothhandler to handle events from ESP bluetooth virtual layer
 *
 * @author Dominik Regec
 */
#ifndef SERVER_BLUETOOTH_HANDLER
#define SERVER_BLUETOOTH_HANDLER

/* Project specific includes */
#include "ServerBluetoothController.hpp"

/* Common components */
#include "Bluetooth/BluetoothDefinitions.hpp"
#include "Bluetooth/Interfaces/BaseBluetoothHandlerInterface.hpp"

/* STD library includes */
#include <memory>

/*************         DEFINES        *************/
#define SERVER_BLUETOOTH_HANDLER_TAG "ServerBluetoothHandler"

#define BLUETOOTH_NAME "Greenhouse"

namespace Greenhouse
{
    namespace Bluetooth
    {
        class ServerBluetoothHandler : public Component::Bluetooth::Interface::ServerBluetoothHandlerInterface<ServerBluetoothController>
        {
        public:
            /* Typedef for incialization bluetooth return code*/
            using INIT_BLE_STATUS = Component::Bluetooth::INIT_BLUETOOTH_RV;

            /**
             * @brief Class constructor with controller parameter
             */
            explicit ServerBluetoothHandler(std::weak_ptr<ServerBluetoothController> controller);

            /**
             * @brief Class destructor
             */
            ~ServerBluetoothHandler();

            /**
             * @brief Method to initialize bluetooth profiles on server side
             *
             * @return bool     true    : Initialization has been successful
             *                  false   : Otherwise
             */
            bool InitializeBluetoothProfiles() override;

            /**
             * @brief Method to handle event that are pushed from BLE stack
             *
             * @param event     -> Event from BLE stack
             * @param param     -> Pointer to parameters of gatts
             */
            virtual void HandleGapEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) override;

            /**
             * @brief Method to handle events from gatts interface on server side
             *
             * @param event     : Event type
             * @param gatts_if  : GATT server access interface
             * @param param     : Point to callback parameter, currently is union type
             */
            void HandleGattsEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) override;

        private:
            /**
             * @brief Checking eventy for comparison on ESP_GATTS_REG_EVT
             *
             * @param event     : Event type
             *
             * @return bool     true    : Event type is ESP_GATTS_REG_ETV
             *                  false   : Otherwise
             */
            bool IsRegistrationEvent(esp_gatts_cb_event_t event) const;

            /**
             * @brief Handle registration event
             *
             * @param gatts_if  : GATT server access interface
             * @param param     : Point to callback parameter, currently is union type
             *
             * @return bool     true    : Gatts interface is stored into profile table
             *                  false   : Could not store Gatts interface
             */
            bool HandleRegistrationEvent(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

            /**
             * @brief Method to handle events for Greenhouse profile
             *
             * @param event     : Event type
             * @param gatts_if  : GATT server access interface
             * @param param     : Point to callback parameter, currently is union type
             */
            void GreenhouseEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

            /* Profiles map */
            Component::Bluetooth::ServerProfileMap mProfilesMap;

            bool mConfigAdvDataSet;
            bool mConfigResAdvDataSet;
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

        // Advertising parameters
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

        static uint8_t char1_str[] = {0x11, 0x22, 0x33};

        static esp_attr_value_t char_value = {
            .attr_max_len = 0x40,
            .attr_len = sizeof(char1_str),
            .attr_value = char1_str,
        };
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // SERVER_BLUETOOTH_HANDLER