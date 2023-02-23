/* Project specific includes */
#include "NetworkManager.h"
#include "EventManager.hpp"
#include "ComponentController.hpp"
#include "GreenhouseDefinitions.hpp"

/* ESP log library*/
#include <esp_log.h>

/* STD library */
#include <algorithm>

/* SDK config file */
#include "sdkconfig.h"

using namespace Greenhouse::Manager;

NetworkManager *NetworkManager::mInstance{nullptr};
std::mutex NetworkManager::mMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
NetworkManager::NetworkManager()
		: mWifiDriver(nullptr), mMQTT_Client(nullptr)
{
	mBluetoothObserver = new Observer::BluetoothDataObserver(EventManager::GetInstance());
}

/**
 * @brief Class destructor
 */
NetworkManager::~NetworkManager()
{
	if (mWifiDriver)
	{
		delete mWifiDriver;
		mWifiDriver = nullptr;
	}

	if (mMQTT_Client)
	{
		delete mMQTT_Client;
		mMQTT_Client = nullptr;
	}
}

/**
 * @brief Callback function when an event occurs
 */
void NetworkManager::MQTT_EventHandler(void *handlerArg, esp_event_base_t base,
																			 int32_t eventID, void *eventData)
{
	auto event = static_cast<esp_mqtt_event_handle_t>(eventData);

	switch (static_cast<esp_mqtt_event_id_t>(eventID))
	{
	case (MQTT_EVENT_CONNECTED):
	{
		auto network_manager = reinterpret_cast<NetworkManager *>(handlerArg);

		ESP_LOGI(NETWORK_MANAGER_TAG, "MQTT Client is connected to MQTT Broker.");
		// Send basic infor about board
		network_manager->SendInfoToServer();

		// Subscribe all topics in greenhouse_topics data structure
		network_manager->SubscribeTopics();
		break;
	}
	case (MQTT_EVENT_DISCONNECTED):
	{
		ESP_LOGI(NETWORK_MANAGER_TAG, "MQTT Client has been disconnected from MQTT Broker.");
		break;
	}
	case MQTT_EVENT_PUBLISHED:
	{
		ESP_LOGI(NETWORK_MANAGER_TAG, "Published data with message ID: %d", event->msg_id);
		break;
	}
	case MQTT_EVENT_SUBSCRIBED:
	{
		ESP_LOGI(NETWORK_MANAGER_TAG, "Subscribed with message ID %d", event->msg_id);
		break;
	}
	case MQTT_EVENT_DATA:
	{
		auto network_manager = reinterpret_cast<NetworkManager *>(handlerArg);
		network_manager->ProcessEventData(event);
		break;
	}
	default:
		ESP_LOGW(NETWORK_MANAGER_TAG, "Unhandled event in switch with ID: %d", eventID);
	}
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Static method to get instance of NetworkManager
 */
NetworkManager *NetworkManager::GetInstance()
{
	std::lock_guard<std::mutex> lock(mMutex);
	if (!mInstance)
		mInstance = new NetworkManager();

	return mInstance;
}

/**
 * @brief Method for connection to wifi
 */
bool NetworkManager::ConnectToWifi(const std::pair<std::string, std::string> &wifiLogin)
{
	using WiFiMode = Component::Driver::Network::WiFi_MODE;

	ESP_LOGI(NETWORK_MANAGER_TAG, "Trying to connect WiFi \"%s\"", wifiLogin.first.c_str());
	mWifiDriver = new WifiDriver(wifiLogin.first, wifiLogin.second, WiFiMode::MODE_STA);
	mWifiDriver->Enable();

	return mWifiDriver->Connect();
}

/**
 * @brief Get ESP board IP address
 */
esp_ip4_addr_t NetworkManager::GetIpAddress() const
{
	if (!mWifiDriver)
		return {};

	return mWifiDriver->GetIpAddress();
}

/**
 * @brief Get ESP board IP address as string
 *
 * @return string : Return IP address in string format
 */
std::string NetworkManager::GetIpAddressAsString(bool reverse) const
{
	auto ip_address = GetIpAddress().addr;

	// If no address set return 0.0.0.0
	if (!ip_address)
		return "0.0.0.0";

	// String IP address
	std::string ip_str;

	// IP block
	uint32_t block = reverse ? 0x000000FF : 0xFF000000;

	for (uint8_t i = 0; i < 4; ++i)
	{
		// Logical and to filter parts of IP address
		// Example 192.168.0.124 & 0x00FF0000 = 0.168.0.0
		auto ip_block_number = (ip_address & block);

		// Shift IP part to right edge
		ip_block_number = reverse ? (ip_block_number >> (i * 8)) : (ip_block_number >> (24 - (i * 8)));

		ip_str.append(std::to_string(ip_block_number) + '.');

		// Shift 8 bits to left or right base of reverse parameter
		block = reverse ? block << 8 : block >> 8;
	}

	// Remove last useless dot
	ip_str.pop_back();

	return ip_str;
}

esp_err_t NetworkManager::ConnectTo_MQTT_Broker(const std::string &uri)
{
	if (uri.empty())
		return ESP_ERR_INVALID_ARG;

	mMQTT_Client = new Utility::Network::MQTT_Client(uri);
	if (mMQTT_Client->RegisterEventHandler(esp_mqtt_event_id_t::MQTT_EVENT_ANY, NetworkManager::MQTT_EventHandler, this))
	{
		ESP_LOGE(NETWORK_MANAGER_TAG, "Registration event handler failed.");
		return ESP_FAIL;
	}

	if (mMQTT_Client->Start())
	{
		ESP_LOGE(NETWORK_MANAGER_TAG, "Failed to start MQTT client.");
		return ESP_FAIL;
	}

	return ESP_OK;
}
/**
 * @brief Method to send data to Server
 *
 * @param shared_ptr : Shared pointer to sensors data
 */
void NetworkManager::SendToServer(const std::shared_ptr<SensorsData> sensorsData)
{
	if (mMQTT_Client)
	{
		if (sensorsData->basic.position == Position::UNKNOWN)
			ESP_LOGE(NETWORK_MANAGER_TAG, "Sensor data does not contain sensor's position.");

		Publish(SENSOR_DATA, sensorsData);
	}
}

/**
 * @brief Send basic info about board to server
 */
void NetworkManager::SendInfoToServer() const
{
	if (!mMQTT_Client)
		return;

	auto root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "ID", CONFIG_Greenhouse_ID);
	cJSON_AddStringToObject(root, "Board", CONFIG_ESP_Board);

	cJSON_AddStringToObject(root, "IP address", GetIpAddressAsString(true).c_str());

	mMQTT_Client->Publish(INFO, cJSON_Print(root), 1);
	cJSON_Delete(root);
}

/**
 * @brief Method to publish sensors data to MQTT server
 *
 * @param shared_ptr : Shared pointer to sensors data
 */
void NetworkManager::Publish(const std::string &topic, const std::shared_ptr<SensorsData> sensorsData)
{
	auto root = cJSON_CreateObject();

	cJSON_AddNumberToObject(root, "ID", CONFIG_Greenhouse_ID);
	cJSON_AddNumberToObject(root, "position", static_cast<uint8_t>(sensorsData->basic.position));

	auto data = cJSON_AddObjectToObject(root, "Data");

	cJSON_AddNumberToObject(data, "measure_time", sensorsData->basic.time);

	if (sensorsData->air.temperature.IsValueSet())
		cJSON_AddNumberToObject(data, "temperature", sensorsData->air.temperature.Get());

	if (sensorsData->air.humidity.IsValueSet())
		cJSON_AddNumberToObject(data, "humidity", sensorsData->air.humidity.Get());

	if (sensorsData->air.co2.IsValueSet())
		cJSON_AddNumberToObject(data, "CO2", sensorsData->air.co2.Get());

	if (sensorsData->soil.soilMoisture.IsValueSet())
		cJSON_AddNumberToObject(data, "soil_moisture", sensorsData->soil.soilMoisture.Get());

	mMQTT_Client->Publish(topic, cJSON_Print(root), 1);
}

/**
 * @brief Method to subscribe all predefined topics
 */
void NetworkManager::SubscribeTopics()
{
	for (const auto &topic : greenhouse_topics)
		mMQTT_Client->Subscribe(topic, 1);
}

/**
 * @brief Process event data
 */
void NetworkManager::ProcessEventData(esp_mqtt_event_handle_t eventData)
{
	if (!eventData)
		return;

	std::string topic;
	topic.assign(eventData->topic, eventData->topic_len);

	std::string data;
	data.assign(eventData->data, eventData->data_len);

	auto json_data = cJSON_Parse(data.c_str());
	if (!json_data)
		return;

	if ((topic.compare(WINDOW) == 0) || (topic.compare(WINDOW_ID) == 0))
	{
		WindowEvent(json_data);
		return;
	}

	if ((topic.compare(IRRIGATION) == 0) || (topic.compare(IRRIGATION_ID) == 0))
	{
		IrrigationEvent(json_data);
		return;
	}
}

/**
 * @brief Hadnle event for window
 */
void NetworkManager::WindowEvent(const cJSON *const json)
{
	auto controller = Manager::ComponentController::GetInstance();

	if (cJSON_HasObjectItem(json, "requested"))
	{
		bool requested = static_cast<bool>(cJSON_GetNumberValue(cJSON_GetObjectItem(json, "requested")));
		if (requested == controller->WindowState())
		{
			ESP_LOGI(NETWORK_MANAGER_TAG, "Request state is same with current window state");
			return;
		}

		if (requested)
			controller->OpenWindow();
		else
			controller->CloseWindow();
	}
}

/**
 * @brief Handle event for irrigation
 */
void NetworkManager::IrrigationEvent(const cJSON *const json)
{
	auto controller = Manager::ComponentController::GetInstance();

	if (cJSON_HasObjectItem(json, "requested"))
	{
		bool requested = static_cast<bool>(cJSON_GetNumberValue(cJSON_GetObjectItem(json, "requested")));
		if (requested == controller->IrrigationState())
		{
			ESP_LOGI(NETWORK_MANAGER_TAG, "Request state is same with current irrigation state");
			return;
		}

		if (requested)
			controller->TurnOnIrrigation();
		else
			controller->TurnOffIrrigation();
	}
}