/**
 * @author Dominik Regec
 */
#ifndef BASE_BLUETOOTH_HANDLER_INTERFACE
#define BASE_BLUETOOTH_HANDLER_INTERFACE

/* Bluetooth includes */
#include "esp_gatts_api.h"   //  Server side -> implements GATT configuration
#include "esp_gattc_api.h"   //  Client side -> implements GATT configuration
#include "esp_gap_ble_api.h" //  implements GAP configuration, such as advertising and connection parameters.

/* STD library includes */
#include <memory>

namespace Component
{
    namespace Bluetooth
    {
        namespace Interface
        {
            template <class Controller>
            class BaseBluetoothHandlerInterface
            {
            public:
                /**
                 * @brief Pure virtual method to initialize bluetooth profiles
                 *
                 * @return bool     true    : Initialization has been successful
                 *                  false   : Otherwise
                 */
                virtual bool InitializeBluetoothProfiles() = 0;

                /**
                 * @brief Pure virtual method to handle event that are pushed from BLE stack
                 *
                 * @param event     -> Event from BLE stack
                 * @param param     -> Pointer to parameters of gatts
                 */
                virtual void HandleGapEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) = 0;

            protected:
                /**
                 * @brief Setter to set pointer to bluetooth controller
                 *
                 * @param[in] controller    : Pointer to bluetooth controller
                 */
                void SetBluetoothController(std::weak_ptr<Controller> controller)
                {
                    mBluetoothController = controller;
                }

                /**
                 * @brief Getter to pointer to bluetooth controller
                 *
                 * @return std::weak<Controller>    : Pointer to bluetooth controller
                 */
                std::weak_ptr<Controller> GetBluetoothController() const
                {
                    return mBluetoothController;
                }

            private:
                std::weak_ptr<Controller> mBluetoothController;
            };

            template <class Controller>
            class ServerBluetoothHandlerInterface : public BaseBluetoothHandlerInterface<Controller>
            {
                /**
                 * @brief Pure virtual method to handle events from gatts interface (BLE stack) for Server side
                 *
                 * @param event     : Event type
                 * @param gatts_if  : GATT server access interface
                 * @param param     : Point to callback parameter, currently is union type
                 */
                virtual void HandleGattsEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) = 0;
            };

            template <class Controller>
            class ClientBluetoothHandlerInterface : public BaseBluetoothHandlerInterface<Controller>
            {
            public:
                /**
                 * @brief Pure virtual method to handle events from gatts interface (BLE stack) for Server side
                 *
                 * @param event     : Event type
                 * @param gatts_if  : GATT server access interface
                 * @param param     : Point to callback parameter, currently is union type
                 */
                virtual void HandleGattcEvent(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) = 0;
            };

        } // namespace Interface
    }     // namespace Bluetooth
} // namespace Greenhouse

#endif // BASE_BLUETOOTH_HANDLER_INTERFACE