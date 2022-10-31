/**
 * Definition of ClientBluetoothHandler to handle events from ESP bluetooth virtual layer
 *
 * @author Dominik Regec
 */
#ifndef CLIENT_BLUETOOTH_HANDLER
#define CLIENT_BLUETOOTH_HANDLER

/* Project specific includes */
#include "ClientBluetoothController.hpp"

/* Common components */
#include "Bluetooth/Interfaces/BaseBluetoothHandlerInterface.hpp"
#include "Bluetooth/BluetoothDefinitions.hpp"

/* STL includes */
#include <memory>
#include <string>

/* Define class log tag */
#define CLIENT_BLUETOOTH_HANDLER_TAG "ClientBluetoothHandler"

namespace Greenhouse
{
    namespace Bluetooth
    {
        class ClientBluetoothHandler : public Component::Bluetooth::Interface::ClientBluetoothHandlerInterface
        {
        public:
            /**
             * @brief Class constructor
             */
            explicit ClientBluetoothHandler();

            /**
             * @brief Class constructor with controller parameter
             */
            explicit ClientBluetoothHandler(std::weak_ptr<ClientBluetoothControlller> controller);

            /**
             * @brief Class destructor
             */
            ~ClientBluetoothHandler();

            /**
             * @brief Overriden method to initialize bluetooth profiles
             *
             * @return bool     true    : Initialization has been successful
             *                  false   : Otherwise
             */
            bool InitializeBluetoothProfiles() override;

            /**
             * @brief Setter to set pointer to bluetooth controller
             *
             * @param[in] controller    : Pointer to bluetooth controller
             */
            void SetBluetoothController(std::weak_ptr<ClientBluetoothControlller> controller);

            /**
             * @brief Overriden method to handle events from gatts interface (BLE stack)
             *
             * @param[in] event     : Event type
             * @param[in] gatts_if  : GATT client access interface
             * @param[in] param     : Point to callback parameter, currently is union type
             */
            void HandleGattcEvent(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) override;

            /**
             * @brief Overriden method to handle event that are pushed from BLE stack
             *
             * @param[in] event     -> Event from BLE stack
             * @param[in] param     -> Pointer to parameters of gatts
             */
            void HandleGapEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) override;

            /**
             * @brief Method to handle events for Greenhouse profile
             *
             * @param[in] event     : Event type
             * @param[in] gatts_if  : GATT client access interface
             * @param[in] param     : Point to callback parameter, currently is union type
             */
            void GreenhouseEventHandler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

        private:
            /**
             * @brief Method to check if incoming event is registration event
             *
             * @param[in] event : Event type
             *
             * @return bool     : true  -> Registration event has came
             *                  : false -> Otherwise
             */
            bool IsRegistrationEvent(esp_gattc_cb_event_t event) const;

            /**
             * @brief Handle registration event
             *
             * @param[in] gatts_if  : GATT server access interface
             * @param[in] param     : Point to callback parameter, currently is union type
             */
            bool HandleRegistrationEvent(esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

            /**
             * @brief Handle scanning result event
             *
             * @param[in] scanResult     : Point to callback parameter, currently is union type
             */
            void HandleScanResultEvent(esp_ble_gap_cb_param_t *scanResult);

            /**
             * @brief Return connection status to remote device
             * 
             * @return bool :   true  -> Client is connected to remote device
             *                  false -> Otherwise    
             */
            bool IsConnected() const;

            /* Profiles map */
            Component::Bluetooth::ClientProfileMap mProfilesMap;

            /* Pointer to bluetooth controller */
            std::weak_ptr<ClientBluetoothControlller> mBluetoothController;

            /* Remote device name*/
            std::string mRemoteDevice;

            /* Value stored connection status to remote device*/
            bool mConnected;
        };
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // CLIENT_BLUETOOTH_HANDLER