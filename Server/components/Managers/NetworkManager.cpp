/* Project specific includes */
#include "NetworkManager.h"
#include "EventManager.hpp"
#include "Common_components/Convertors/Convertor_JSON.hpp"

/* ESP log library*/
#include <esp_log.h>

/* STD library */
#include <algorithm>

/* SDK config file */
#include "sdkconfig.h"

/* ESP cJSON library */
#include <cJSON.h>

using namespace Greenhouse::Manager;

NetworkManager *NetworkManager::mInstance{nullptr};
std::mutex NetworkManager::mMutex;

static const char *TAG = "MQTT_EXAMPLE";

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
    ESP_LOGD(NETWORK_MANAGER_TAG, "Event dispatched from event loop base=%s, event_id=%d", base, eventID);

    auto event = static_cast<esp_mqtt_event_handle_t>(eventData);

    switch (static_cast<esp_mqtt_event_id_t>(eventID))
    {
    case (MQTT_EVENT_CONNECTED):
    {
        ESP_LOGI(NETWORK_MANAGER_TAG, "MQTT Client is connected to MQTT Broker.");
        NetworkManager::GetInstance()->SendInfoToServer();
        break;
    }
    case (MQTT_EVENT_DISCONNECTED):
    {
        ESP_LOGI(NETWORK_MANAGER_TAG, "MQTT Client is disconnected from MQTT Broker.");
        break;
    }
    case MQTT_EVENT_PUBLISHED:
    {
        ESP_LOGI(NETWORK_MANAGER_TAG, "MQTT Client publish data. Message ID: %d", event->msg_id);
        break;
    }
    default:
        ESP_LOGW(NETWORK_MANAGER_TAG, "Unhandled event in switch with ID: %d", eventID);
    }
}

/************   MQTT Client     ************/
/**
 * @brief Class constructor
 */
NetworkManager::MQTT_Client::MQTT_Client(const std::string &uri, const std::string &clientName)
{
    // Create clear config structure
    mConfig = (esp_mqtt_client_config_t *)calloc(1, sizeof(esp_mqtt_client_config_t));

    // Allocate memory and set URI to config structure
    mConfig->uri = (const char *)malloc(uri.size());
    mConfig->uri = uri.c_str();

    // Allocate memory and set client name
    mConfig->client_id = (const char *)malloc(clientName.size());
    mConfig->client_id = clientName.c_str();

    mClient = esp_mqtt_client_init(mConfig);
}

/**
 * @brief Class destructor
 */
NetworkManager::MQTT_Client::~MQTT_Client()
{
    // Free memory
    free((char *)mConfig->uri);
    free((char *)mConfig->client_id);

    free(mConfig);

    esp_mqtt_client_destroy(mClient);
}

/**
 * @brief Register MQTT event
 */
esp_err_t NetworkManager::MQTT_Client::RegisterEventHandler(esp_mqtt_event_id_t event,
                                                            esp_event_handler_t eventHandler,
                                                            void *eventHandlerArg) const
{
    return esp_mqtt_client_register_event(mClient, event, eventHandler, eventHandlerArg);
}

/**
 * @brief Start MQTT client
 */
esp_err_t NetworkManager::MQTT_Client::Start() const
{
    return esp_mqtt_client_start(mClient);
}

/**
 * @brief Stop MQTT client
 */
esp_err_t NetworkManager::MQTT_Client::Stop() const
{
    return esp_mqtt_client_stop(mClient);
}

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
int NetworkManager::MQTT_Client::Publish(const std::string &topic, const std::string &data, int QoS, bool retain)
{
    return esp_mqtt_client_publish(mClient, topic.c_str(), data.c_str(), data.size(), QoS, retain);
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

    mMQTT_Client = new MQTT_Client(uri, CONFIG_MQTT_CLIENT_NAME);
    if (mMQTT_Client->RegisterEventHandler(esp_mqtt_event_id_t::MQTT_EVENT_ANY, NetworkManager::MQTT_EventHandler, nullptr))
    {
        ESP_LOGE(MQTT_CLIENT_TAG, "Registration event handler failed.");
        return ESP_FAIL;
    }

    if (mMQTT_Client->Start())
    {
        ESP_LOGE(MQTT_CLIENT_TAG, "Failed to start MQTT client.");
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
        Publish(sensorsData);
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

    auto ip_address = GetIpAddressAsString();
    std::reverse(ip_address.begin(), ip_address.end());

    cJSON_AddStringToObject(root, "IP address", GetIpAddressAsString(true).c_str());

    mMQTT_Client->Publish("Greenhouse_info", Component::Convertor::Convertor_JSON::GetInstance()->ToString(root).c_str(), 1);
}

/**
 * @brief Method to publish sensors data to MQTT server
 *
 * @param shared_ptr : Shared pointer to sensors data
 */
void NetworkManager::Publish(const std::shared_ptr<SensorsData> sensorsData)
{
    auto root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "Temperature", sensorsData->GetTemperature());
    cJSON_AddNumberToObject(root, "Humanity", sensorsData->GetHumanity());

    mMQTT_Client->Publish("SensorsData_IN", Component::Convertor::Convertor_JSON::GetInstance()->ToString(root).c_str(), 1);
}