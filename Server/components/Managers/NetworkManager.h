#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

/* STL library */
#include <utility>
#include <string>
#include <mutex>
#include <memory>

/* Common components includes */
#include "Drivers/Network/WiFiDriver.hpp"

/* Project specific includes */
#include "Observers/BluetoothDataObserver.hpp"
#include "SensorsData/SensorsData.hpp"

/* ESP MQTT library */
#include <mqtt_client.h>

#define NETWORK_MANAGER_TAG "Network manager"
#define MQTT_CLIENT_TAG "MQTT client"

#define DEFAULT_CLIENT_NAME "ESP_Client"

namespace Greenhouse
{
    namespace Manager
    {
        class NetworkManager
        {
        public:
            /**
             * @brief Static method to get instance of NetworkManager
             *
             * @return NetworkManager    : Pointer to NetworkManager manager
             */
            static NetworkManager *GetInstance();

            /**
             * @brief Method for connection to wifi
             *
             * @param[in] wifiLogin : Reference to login wifi parameteres as name and password
             *
             * @return bool     true    : Device is connected to wifi network
             *                          : Connection to wifi network failed
             */
            bool ConnectToWifi(const std::pair<std::string, std::string> &wifiLogin);

            /**
             * @brief Get ESP board IP address
             *
             * @return esp_ip4_addr_t : If ESP board is connected to network. Method return assigned IP address.
             *                          Otherwise is returned empty structure.
             */
            esp_ip4_addr_t GetIpAddress() const;

            /**
             * @brief Get ESP board IP address as string
             *
             * @param[in] reverse : Reverse order
             *
             * @return string : Return IP address in string format
             */
            std::string GetIpAddressAsString(bool reverse = false) const;

            /**
             * @brief Connect to MQTT broker with provided uri
             *
             * @param[in] uri   : MQTT Broker URI
             *
             * @return esp_err_t    ESP_OK              : Success
             *                      ESP_ERR_INVALID_ARG : Empty string in argument
             *                      ESP_FAIL            : Failure
             */
            esp_err_t ConnectTo_MQTT_Broker(const std::string &uri);

            /**
             * @brief Method to send data to Server
             *
             * @param shared_ptr : Shared pointer to sensors data
             */
            void SendToServer(const std::shared_ptr<SensorsData> sensorsData);

            /**
             * @brief Send basic info about board to server
             */
            void SendInfoToServer() const;

        private:
            /**
             * @brief Class constructor
             */
            explicit NetworkManager();

            /**
             * @brief Class destructor
             */
            ~NetworkManager();

            /**
             * @brief Callback function when an event occurs
             *
             * @param[in] handlerArg    : Pointer to handler arguments
             * @param[in] base          : Event base
             * @param[in] eventID       : Event ID
             * @param[in] eventData     : Event data
             */
            static void MQTT_EventHandler(void *handlerArg, esp_event_base_t base,
                                          int32_t eventID, void *eventData);

            /**
             * @brief Method to publish sensors data to MQTT server
             *
             * @param[in] topic      : MQTT Topic
             * @param[in] shared_ptr : Shared pointer to sensors data
             */
            void Publish(const std::string &topic, const std::shared_ptr<SensorsData> sensorsData);

            class MQTT_Client
            {
            public:
                /**
                 * @brief Class constructor
                 */
                explicit MQTT_Client(const std::string &uri, const std::string &clientName = DEFAULT_CLIENT_NAME);

                /**
                 * @brief Class destructor
                 */
                ~MQTT_Client();

                /**
                 * @brief Register MQTT event
                 *
                 * @param[in] event             : MQTT event type
                 * @param[in] eventHandler      : Handler callback
                 * @param[in] eventHandlerArg   : Handler context data
                 *
                 * @return esp_err_t    ESP_OK              : Success
                 *                      ESP_ERR_INVALID_ARG : Wrong inicialization
                 *                      ESP_ERR_NO_MEM      : Failed to allocate
                 */
                esp_err_t RegisterEventHandler(esp_mqtt_event_id_t event, esp_event_handler_t eventHandler, void *eventHandlerArg) const;

                /**
                 * @brief Start MQTT client
                 *
                 * @return esp_err_t    ESP_OK              : Success
                 *                      ESP_ERR_INVALID_ARG : Wrong inicialization
                 *                      ESP_FAIL            : Client is in invalid state
                 */
                esp_err_t Start() const;

                /**
                 * @brief Stop MQTT client
                 *
                 * @return esp_err_t    ESP_OK              : Success
                 *                      ESP_ERR_INVALID_ARG : Wrong inicialization
                 *                      ESP_FAIL            : Client is in invalid state
                 */
                esp_err_t Stop() const;

                /**
                 * @brief Client publish message to MQTT broker
                 *
                 * @param[in] topic  : MQTT topic
                 * @param[in] data   : Data
                 * @param[in] QoS    : Quality of Service
                 * @param[in] retain : Retain flag (Default false)
                 *
                 * @return int  : Message ID
                 */
                int Publish(const std::string &topic, const std::string &data, int QoS, bool retain = false);

            private:
                // Pointer to client MQTT configuration file
                esp_mqtt_client_config_t *mConfig;

                // MQTT client
                esp_mqtt_client_handle_t mClient;
            };

            // Typedef to WiFi driver component
            using WifiDriver = Component::Driver::Network::WifiDriver;

            // Singleton instance of network manager
            static NetworkManager *mInstance;

            // Singleton mutex to protect instance from multithread
            static std::mutex mMutex;

            // Wifi driver
            WifiDriver *mWifiDriver;

            // MQTT Client
            MQTT_Client *mMQTT_Client;

            // BluetoothObserver
            Observer::BluetoothDataObserver *mBluetoothObserver;
        };
    } // namespace Manager
} // namespace Greenhouse

#endif // NETWORK_MANAGER_H