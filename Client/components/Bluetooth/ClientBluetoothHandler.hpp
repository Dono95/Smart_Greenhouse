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
#include "Bluetooth/BluetoothDefinitions.hpp"
#include "Bluetooth/Interfaces/BaseBluetoothHandlerInterface.hpp"

/* STL includes */
#include <memory>
#include <string>

/* Define class log tag */
#define CLIENT_BLUETOOTH_HANDLER_TAG "ClientBluetoothHandler"

namespace Greenhouse
{
    namespace Bluetooth
    {
        class ClientBluetoothHandler : public Component::Bluetooth::Interface::ClientBluetoothHandlerInterface<ClientBluetoothControlller>
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
             * @brief Get gattc profile data structure
             *
             * @param key   : Bluetooth profile ID
             *
             * @return Component::Bluetooth::ClientGattcProfile
             */
            Component::Bluetooth::ClientGattcProfile GetGattcProfile(const uint8_t key) const;

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

            /**
             * @brief Return connection status to remote device
             *
             * @return bool :   true  -> Client is connected to remote device
             *                  false -> Otherwise
             */
            bool IsConnected() const;

        private:
            /**
             * @brief Method to check if incoming event is registration event
             *
             * @param[in] event     : Event type
             *
             * @return bool         : true  -> Registration event has came
             *                      : false -> Otherwise
             */
            bool IsRegistrationEvent(esp_gattc_cb_event_t event) const;

            /**
             * @brief Check service source
             *
             * @param[in] source    : Service source
             */
            void CheckServiceSource(esp_service_source_t source) const;

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
             * @brief Handle service information
             *
             * @param[in] gatts_if      : GATT server access interface
             * @param[in] connectionID  : Connection ID
             */
            void HandleServiceInformation(esp_gatt_if_t gattc_if, uint16_t connectionID);

            /**
             * @brief Handle discoonection event
             *
             * @param[in] reason        : Disconnection reason
             */
            void HandleDisconnection(esp_gatt_conn_reason_t reason);

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