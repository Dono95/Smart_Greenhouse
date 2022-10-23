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

/* Project specific includes */
#include "BaseBluetoothDefinitions.hpp"

/* Bluetooth includes */
#include "esp_bt.h"          //  implements BT controller and VHCI configuration procedures from the host side.
#include "esp_bt_main.h"     //  implements initialization and enabling of the Bluedroid stack.
#include "esp_gap_ble_api.h" //  implements GAP configuration, such as advertising and connection parameters.
#include "esp_gatts_api.h"   //  implements GATT configuration, such as creating services and characteristics.
#include "esp_gatt_common_api.h"

const static char *BLUETOOTH_CONTROLLER_TAG = "BluetoothController";

namespace Greenhouse
{
    namespace Bluetooth
    {
        class BluetoothController
        {
        public:
            /* Enum to define return value of the initialization process */
            enum class INIT_BLUETOOTH_RV
            {
                RV_BLUETOOTH_OFF,             // <- Bluetooth is turn off
                RV_INVALID_ARGUMENT,          // <- Invalid argument passed to function
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
        };
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // BLUETOOTH_CONTROLLER