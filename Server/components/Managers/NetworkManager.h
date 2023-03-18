#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

/* STL library */
#include <utility>
#include <string>
#include <mutex>
#include <memory>

/* Common components includes */
#include <Drivers/Network/WiFiDriver.hpp>
#include <Utility/Network/MQTT_Client.hpp>
#include <Trackers/WifiConnectionTracker.hpp>

/* Project specific includes */
#include "Observers/BluetoothDataObserver.hpp"
#include "SensorsData/SensorsData.hpp"
#include "WifiConnectionHolder.hpp"

/* ESP MQTT library */
#include <mqtt_client.h>

/* ESP cJSON library */
#include <cJSON.h>

#define NETWORK_MANAGER_TAG "Network manager"

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
			 * @brief Enable wifi connection tracker
			 *
			 * @param period : Time period to generate interrupt
			 */
			void EnableWifiConnectionTracker(const uint64_t period);

			/**
			 * @brief Disable wifi connection tracker
			 */
			void DisableWifiConnectionTracker();

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

			/**
			 * @brief Method to subscribe all predefined topics
			 * @warning Method must be called only after MQTT connected event
			 */
			void SubscribeTopics();

			/**
			 * @brief Process event data
			 *
			 * @param[in] eventData  : Event data
			 */
			void ProcessEventData(esp_mqtt_event_handle_t eventData);

			/**
			 * @brief Hadnle event for window
			 *
			 * @param[in] json			: JSON data with current window state and requested
			 */
			void WindowEvent(const cJSON *const json);

			/**
			 * @brief Hadnle event for irrigation
			 *
			 * @param[in] json			: JSON data with current window state and requested
			 */
			void IrrigationEvent(const cJSON *const json);

			// Typedef to WiFi driver component
			using WifiDriver = Component::Driver::Network::WifiDriver;

			// Singleton instance of network manager
			static NetworkManager *mInstance;

			// Singleton mutex to protect instance from multithread
			static std::mutex mMutex;

			// Wifi driver
			WifiDriver *mWifiDriver;

			// Wifi connection tracker
			Component::Tracker::WifiConnectionTracker *mWifiConnectionTracker;

			// Wifi connection holder
			WifiConnectionHolder *mWifiConnectionHolder;

			// MQTT Client
			Utility::Network::MQTT_Client *mMQTT_Client;

			// BluetoothObserver
			Observer::BluetoothDataObserver *mBluetoothObserver;
		};
	} // namespace Manager
} // namespace Greenhouse

#endif // NETWORK_MANAGER_H