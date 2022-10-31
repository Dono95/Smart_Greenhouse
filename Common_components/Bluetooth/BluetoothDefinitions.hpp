#ifndef BLUETOOTH_DEFINITIONS
#define BLUETOOTH_DEFINITIONS

/* STL includes */
#include <cstdint>
#include <map>

/* Bluetooth includes */
#include "esp_gattc_api.h"
#include "esp_gap_ble_api.h"

namespace Component
{
    namespace Bluetooth
    {
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