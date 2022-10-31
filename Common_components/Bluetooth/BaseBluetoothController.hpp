/**
 * Definition of Base class for Bluetooth controller
 *
 * @author Dominik Regec
 */
#ifndef BASE_BLUETOOTH_CONTROLLER
#define BASE_BLUETOOTH_CONTROLLER

/* Define log tag */
#define BASE_BLUETOOTH_CONTROLLER_TAG "BaseBluetoothController"

/* Bluetooth includes */
#include "esp_bt.h"          //  implements BT controller and VHCI configuration procedures from the host side.
#include "esp_bt_main.h"     //  implements initialization and enabling of the Bluedroid stack.
#include "esp_gap_ble_api.h" //  implements GAP configuration, such as advertising and connection parameters.
#include "esp_gattc_api.h"   //  implements GATT configuration, such as creating services and characteristics.
#include "esp_gatt_common_api.h"


namespace Component
{
    namespace Bluetooth
    {
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

        class BaseBluetoothController
        {
        public:

            /**
             * @brief Class base contrustor
             */
            BaseBluetoothController();

            /**
             * @brief Virtual class base destructor
             */
            virtual ~BaseBluetoothController();

            /**
             * @brief Method to initialize bluetooth controller.
             *
             * @param bluetoothMode[in]     Bluetooth mode
             *
             * @return INIT_BLUETOOTH_RV -> Return value based on initialization progress
             *                           -> RV_BLUETOOTH_INIT_OK - Succeeded, Othrewise - Failed
             */
            INIT_BLUETOOTH_RV InitBluetoothController(const esp_bt_mode_t bluetoothMode);

            /**
             * @brief Method to register gap and gatts callback
             * 
             * @return esp_err_t    ESP_OK  : registration was successful
             *                      Other   : Otherwise with specific enum value with failure code
             */ 
            virtual INIT_BLUETOOTH_RV RegisterCallbacks(void) = 0;

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
} // namespace Component

#endif // BASE_BLUETOOTH_CONTROLLER
