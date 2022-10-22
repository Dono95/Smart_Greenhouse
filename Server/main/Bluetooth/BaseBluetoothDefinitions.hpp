#ifndef BASE_BLUETOOTH_DEFINITIONS
#define BASE_BLUETOOTH_DEFINITIONS

/* STL includes */
#include "cstdint"

/* Bluetooth includes */
#include "esp_gatts_api.h"

namespace Greenhouse
{
    namespace Bluetooth
    {
        struct GattsProfile_I
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
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // BASE_BLUETOOTH_DEFINITIONS