/**
 * @author Dominik Regec
 */
#ifndef BASE_BLUETOOTH_HANDLER_INTERFACE
#define BASE_BLUETOOTH_HANDLER_INTERFACE

/* Bluetooth includes */
#include "esp_gatts_api.h"

namespace Greenhouse
{
    namespace Bluetooth
    {
        class BaseBluetoothHandlerInterface
        {
        public:
            /**
             * @brief Virtual method to handle events from gatts interface
             *
             * @param event     : Event type
             * @param gatts_if  : GATT server access interface
             * @param param     : Point to callback parameter, currently is union type
             */
            virtual void HandleGattsEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) = 0;
        };
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // BASE_BLUETOOTH_HANDLER_INTERFACE