/**
 * Definition of ServerBluetoothhandler to handle events from ESP bluetooth virtual layer
 *
 * @author Dominik Regec
 */
#ifndef SERVER_BLUETOOTH_HANDLER
#define SERVER_BLUETOOTH_HANDLER

/* Project specific includes */
#include "BaseBluetoothDefinitions.hpp"
#include "BaseBluetoothHandlerInterface.hpp"

/* STL includes */
#include <map>

/*************         DEFINES        *************/
#define SERVER_BLUETOOTH_HANDLER_TAG "ServerBluetoothHandler"

#define BLUETOOTH_NAME "Greenhouse"

namespace Greenhouse
{
    namespace Bluetooth
    {
        class ServerBluetoothHandler : public BaseBluetoothHandlerInterface
        {
        public:
            /**
             * @brief Default class constructor
             */
            explicit ServerBluetoothHandler();

            /**
             * @brief Class destructor
             */
            ~ServerBluetoothHandler();

            /**
             * @brief Method to initialize bluetooth profiles on server side
             * 
             * @return bool     true    : Initialization has been successful
             *                  false   : Otherwise
             */ 
            bool InitializeBluetoothProfiles() override;

            /**
             * @brief Method to handle events from gatts interface on server side
             *
             * @param event     : Event type
             * @param gatts_if  : GATT server access interface
             * @param param     : Point to callback parameter, currently is union type
             */
            void HandleGattsEvent(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) override;

            /**
             * @brief Method to handle event that are pushed from BLE stack
             *
             * @param event     -> Event from BLE stack
             * @param param     -> Pointer to parameters of gatts
             */
            virtual void HandleGapEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) override;
        private:
            /**
             * @brief Checking eventy for comparison on ESP_GATTS_REG_EVT
             *
             * @param event     : Event type
             *
             * @return bool     true    : Event type is ESP_GATTS_REG_ETV
             *                  false   : Otherwise
             */
            bool IsRegistrationEvent(esp_gatts_cb_event_t event) const;

            /**
             * @brief Handle registration event
             *
             * @param gatts_if  : GATT server access interface
             * @param param     : Point to callback parameter, currently is union type
             *
             * @return bool     true    : Gatts interface is stored into profile table
             *                  false   : Could not store Gatts interface
             */
            bool HandleRegistrationEvent(esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

            /**
             * @brief Method to handle events for Greenhouse profile
             *
             * @param event     : Event type
             * @param gatts_if  : GATT server access interface
             * @param param     : Point to callback parameter, currently is union type
             */
            void GreenhouseEventHandler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

            /* Map with profile ID as key map and structure of gatts progile*/
            using ProfileMap = std::map<uint8_t, GattsProfile_I>;

            ProfileMap mProfileMap;
        };
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // SERVER_BLUETOOTH_HANDLER