#ifndef BLUETOOTH_DEFINITIONS
#define BLUETOOTH_DEFINITIONS

/* STL includes */
#include <cstdint>
#include <string>
#include <map>

/* Bluetooth includes */
#include "esp_gattc_api.h"
#include "esp_gap_ble_api.h"

namespace Component
{
    namespace Bluetooth
    {
        static std::string EnumToString(esp_gap_ble_cb_event_t event) 
        {
            switch (event)
            {
            case (ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT): return "ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT";
            case (ESP_GAP_BLE_SCAN_START_COMPLETE_EVT): return "ESP_GAP_BLE_SCAN_START_COMPLETE_EVT";
            case (ESP_GAP_BLE_SCAN_RESULT_EVT): return "ESP_GAP_BLE_SCAN_RESULT_EVT";
            default:
                break;
            } 

            return "Unimplemented event[" + std::to_string(event) + "] conversion.";
        }

        static std::string EnumToString(esp_gap_search_evt_t searchEvent) 
        {
            switch (searchEvent)
            {
            case (ESP_GAP_SEARCH_INQ_RES_EVT): return "ESP_GAP_SEARCH_INQ_RES_EVT";
            case (ESP_GAP_SEARCH_INQ_CMPL_EVT): return "ESP_GAP_SEARCH_INQ_CMPL_EVT";
            case (ESP_GAP_SEARCH_DISC_RES_EVT): return "ESP_GAP_SEARCH_DISC_RES_EVT";
            case (ESP_GAP_SEARCH_DISC_BLE_RES_EVT): return "ESP_GAP_SEARCH_DISC_BLE_RES_EVT";
            case (ESP_GAP_SEARCH_DISC_CMPL_EVT): return "ESP_GAP_SEARCH_DISC_CMPL_EVT";
            case (ESP_GAP_SEARCH_DI_DISC_CMPL_EVT): return "ESP_GAP_SEARCH_DI_DISC_CMPL_EVT";
            case (ESP_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT): return "ESP_GAP_SEARCH_SEARCH_CANCEL_CMPL_EVT";
            case (ESP_GAP_SEARCH_INQ_DISCARD_NUM_EVT): return "ESP_GAP_SEARCH_INQ_DISCARD_NUM_EVT";
            default:
                break;
            } 

            return "Unimplemented event[" + std::to_string(searchEvent) + "] conversion.";
        }

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