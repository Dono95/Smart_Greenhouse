#ifndef BLUETOOTH_DEFINITIONS
#define BLUETOOTH_DEFINITIONS

/* STL includes */
#include <cstdint>
#include <string>
#include <map>

/* Bluetooth includes */
#include "esp_gattc_api.h"
#include "esp_gatts_api.h"
#include "esp_gap_ble_api.h"

namespace Component
{
    namespace Bluetooth
    {
        static std::string EnumToString(esp_gap_ble_cb_event_t event)
        {
            switch (event)
            {
            case (ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT):
                return "ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT";
            case (ESP_GAP_BLE_SCAN_START_COMPLETE_EVT):
                return "ESP_GAP_BLE_SCAN_START_COMPLETE_EVT";
            case (ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT):
                return "ESP_GAP_BLE_SCAN_START_COMPLETE_EVT";
            case (ESP_GAP_BLE_SCAN_RESULT_EVT):
                return "ESP_GAP_BLE_SCAN_RESULT_EVT";
            case (ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT):
                return "ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT";
            case (ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT):
                return "ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT";
            case (ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT):
                return "ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT";
            case (ESP_GAP_BLE_ADV_START_COMPLETE_EVT):
                return "ESP_GAP_BLE_ADV_START_COMPLETE_EVT";
            default:
                break;
            }

            return "Unimplemented event[" + std::to_string(event) + "] conversion.";
        }

        static std::string EnumToString(esp_gap_search_evt_t searchEvent)
        {
            switch (searchEvent)
            {
            case (ESP_GAP_SEARCH_INQ_RES_EVT):
                return "ESP_GAP_SEARCH_INQ_RES_EVT";
            case (ESP_GAP_SEARCH_INQ_CMPL_EVT):
                return "ESP_GAP_SEARCH_INQ_CMPL_EVT";
            case (ESP_GAP_SEARCH_DISC_RES_EVT):
                return "ESP_GAP_SEARCH_DISC_RES_EVT";
            case (ESP_GAP_SEARCH_DISC_BLE_RES_EVT):
                return "ESP_GAP_SEARCH_DISC_BLE_RES_EVT";
            case (ESP_GAP_SEARCH_DISC_CMPL_EVT):
                return "ESP_GAP_SEARCH_DISC_CMPL_EVT";
            case (ESP_GAP_SEARCH_DI_DISC_CMPL_EVT):
                return "ESP_GAP_SEARCH_DI_DISC_CMPL_EVT";
            case (ESP_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT):
                return "ESP_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT";
            case (ESP_GAP_SEARCH_INQ_DISCARD_NUM_EVT):
                return "ESP_GAP_SEARCH_INQ_DISCARD_NUM_EVT";
            default:
                break;
            }

            return "Unimplemented event[" + std::to_string(searchEvent) + "] conversion.";
        }

/***************************************************************************************/
/***********                            SERVER                               ***********/
/***************************************************************************************/
#define GATTS_AIR_SERVICE_UUID 0x00FF

        struct ServerGattsProfile
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

        static std::string EnumToString(esp_gatts_cb_event_t event)
        {
            switch (event)
            {
            case (ESP_GATTS_REG_EVT):
                return "ESP_GATTS_REG_EVT";
            case (ESP_GATTS_READ_EVT):
                return "ESP_GATTS_READ_EVT";
            case (ESP_GATTS_WRITE_EVT):
                return "ESP_GATTS_WRITE_EVT";
            case (ESP_GATTS_EXEC_WRITE_EVT):
                return "ESP_GATTS_EXEC_WRITE_EVT";
            case (ESP_GATTS_MTU_EVT):
                return "ESP_GATTS_MTU_EVT";
            case (ESP_GATTS_CONF_EVT):
                return "ESP_GATTS_CONF_EVT";
            case (ESP_GATTS_UNREG_EVT):
                return "ESP_GATTS_UNREG_EVT";
            case (ESP_GATTS_CREATE_EVT):
                return "ESP_GATTS_CREATE_EVT";
            case (ESP_GATTS_ADD_INCL_SRVC_EVT):
                return "ESP_GATTS_ADD_INCL_SRVC_EVT";
            case (ESP_GATTS_ADD_CHAR_EVT):
                return "ESP_GATTS_ADD_CHAR_EVT";
            case (ESP_GATTS_ADD_CHAR_DESCR_EVT):
                return "ESP_GATTS_ADD_CHAR_DESCR_EVT";
            case (ESP_GATTS_DELETE_EVT):
                return "ESP_GATTS_DELETE_EVT";
            case (ESP_GATTS_START_EVT):
                return "ESP_GATTS_START_EVT";
            case (ESP_GATTS_STOP_EVT):
                return "ESP_GATTS_STOP_EVT";
            case (ESP_GATTS_CONNECT_EVT):
                return "ESP_GATTS_CONNECT_EVT";
            case (ESP_GATTS_DISCONNECT_EVT):
                return "ESP_GATTS_DISCONNECT_EVT";
            case (ESP_GATTS_OPEN_EVT):
                return "ESP_GATTS_OPEN_EVT";
            case (ESP_GATTS_CANCEL_OPEN_EVT):
                return "ESP_GATTS_CANCEL_OPEN_EVT";
            case (ESP_GATTS_CLOSE_EVT):
                return "ESP_GATTS_CLOSE_EVT";
            case (ESP_GATTS_LISTEN_EVT):
                return "ESP_GATTS_LISTEN_EVT";
            case (ESP_GATTS_CONGEST_EVT):
                return "ESP_GATTS_CONGEST_EVT";
            case (ESP_GATTS_RESPONSE_EVT):
                return "ESP_GATTS_RESPONSE_EVT";
            case (ESP_GATTS_CREAT_ATTR_TAB_EVT):
                return "ESP_GATTS_CREAT_ATTR_TAB_EVT";
            case (ESP_GATTS_SET_ATTR_VAL_EVT):
                return "ESP_GATTS_SET_ATTR_VAL_EVT";
            case (ESP_GATTS_SEND_SERVICE_CHANGE_EVT):
                return "ESP_GATTS_SEND_SERVICE_CHANGE_EVT";
            default:
                break;
            }

            return "Unimplemented event[" + std::to_string(event) + "] conversion.";
        }

        /* Server profile map with Key [Profile ID] and Value [ServerGattsProfile structure]*/
        using ServerProfileMap = std::map<uint8_t, ServerGattsProfile>;

        /***************************************************************************************/
        /***********                            CLIENT                               ***********/
        /***************************************************************************************/
        static esp_gattc_char_elem_t *char_elem_result = NULL;
        static esp_gattc_descr_elem_t *descr_elem_result = NULL;

#define REMOTE_SERVICE_UUID 0x00FF
#define REMOTE_NOTIFY_CHAR_UUID 0xFF01

        static esp_bt_uuid_t remote_filter_service_uuid = {
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

        struct ClientGattcProfile
        {
            esp_gattc_cb_t gattc_cb;
            uint16_t gattc_if;
            uint16_t app_id;
            uint16_t conn_id;
            uint16_t service_start_handle;
            uint16_t service_end_handle;
            uint16_t char_handle;
            esp_bd_addr_t remote_bda;
        };

        static std::string EnumToString(esp_gattc_cb_event_t gattc_event)
        {
            switch (gattc_event)
            {
            case (ESP_GATTC_REG_EVT):
                return "ESP_GATTC_REG_EVT";
            case (ESP_GATTC_OPEN_EVT):
                return "ESP_GATTC_OPEN_EVT";
            case (ESP_GATTC_WRITE_CHAR_EVT):
                return "ESP_GATTC_WRITE_CHAR_EVT";
            case (ESP_GATTC_SEARCH_CMPL_EVT):
                return "ESP_GATTC_SEARCH_CMPL_EVT";
            case (ESP_GATTC_SEARCH_RES_EVT):
                return "ESP_GATTC_SEARCH_RES_EVT";
            case (ESP_GATTC_CFG_MTU_EVT):
                return "ESP_GATTC_CFG_MTU_EVT";
            case (ESP_GATTC_CONNECT_EVT):
                return "ESP_GATTC_CONNECT_EVT";
            case (ESP_GATTC_DIS_SRVC_CMPL_EVT):
                return "ESP_GATTC_DIS_SRVC_CMPL_EVT";
            default:
                break;
            }

            return "Unimplemented event[" + std::to_string(gattc_event) + "] conversion.";
        }

        /* Client profile map with Key [Profile ID] and Value [ClientGattcProfile structure]*/
        using ClientProfileMap = std::map<uint8_t, ClientGattcProfile>;

/* PROFILES */
#define GREENHOUSE_PROFILE 0

    } // namespace Bluetooth
} // namespace Component

#endif // BLUETOOTH_DEFINITIONS