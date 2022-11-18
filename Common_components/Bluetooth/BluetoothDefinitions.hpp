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

        /* Client profile map with Key [Profile ID] and Value [ClientGattcProfile structure]*/
        using ClientProfileMap = std::map<uint8_t, ClientGattcProfile>;

/* PROFILES */
#define GREENHOUSE_PROFILE 0

    } // namespace Bluetooth
} // namespace Component

#endif // BLUETOOTH_DEFINITIONS