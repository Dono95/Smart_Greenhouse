/**
 * Definition of BluetoothController
 *
 * @author Dominik Regec
 */
#ifndef SERVER_BLUETOOTH_CONTROLLER
#define SERVER_BLUETOOTH_CONTROLLER

/* Common components */
#include "Bluetooth/BaseBluetoothController.hpp"

/* STL includes */
//#include <vector>
//#include <cstring>

/* Project specific includes */
//#include "BaseBluetoothDefinitions.hpp"

/* Bluetooth includes */
//#include "esp_bt.h"          //  implements BT controller and VHCI configuration procedures from the host side.
//#include "esp_bt_main.h"     //  implements initialization and enabling of the Bluedroid stack.
//#include "esp_gap_ble_api.h" //  implements GAP configuration, such as advertising and connection parameters.
//#include "esp_gatts_api.h"   //  implements GATT configuration, such as creating services and characteristics.
//#include "esp_gatt_common_api.h"

#define SERVER_BLUETOOTH_CONTROLLER_TAG "ServerBluetoothController"

namespace Greenhouse
{
    namespace Bluetooth
    {
        class ServerBluetoothController : public Component::Bluetooth::BaseBluetoothController
        {
        public:
            /* Typedef for incialization bluetooth return code*/
            using INIT_BLE_STATUS = Component::Bluetooth::INIT_BLUETOOTH_RV;

            /**
             * @brief Class constructor
             */
            explicit ServerBluetoothController();

            /**
             * @brief Class destructor
             */
            ~ServerBluetoothController();

            /**
             * @brief Overridden method to register gap and gatts callback for client controller
             */
            virtual INIT_BLE_STATUS RegisterCallbacks(void) override;

        private:
        };
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // SERVER_BLUETOOTH_CONTROLLER