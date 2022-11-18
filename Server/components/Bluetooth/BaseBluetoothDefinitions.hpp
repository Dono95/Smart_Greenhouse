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

#define GATTS_NUM_HANDLE_TEST_A 4
#define GATTS_CHAR_UUID_TEST_A 0xFF01

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40

                static esp_gatt_char_prop_t a_property = 0;

                static uint8_t adv_config_done = 0;
#define adv_config_flag (1 << 0)
#define scan_rsp_config_flag (1 << 1)

        } // namespace Bluetooth
} // namespace Greenhouse

#endif // BASE_BLUETOOTH_DEFINITIONS