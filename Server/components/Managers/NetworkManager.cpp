/* Project specific includes */
#include "NetworkManager.h"

/* ESP log library*/
#include <esp_log.h>

#include "sdkconfig.h"

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
{
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
}

/**
 * @brief Callback function when an event occurs
 */
void NetworkManager::MQTT_EventHandler(void *handlerArg, esp_event_base_t base,
                                       int32_t eventID, void *eventData)
{
    ESP_LOGD(NETWORK_MANAGER_TAG, "Event dispatched from event loop base=%s, event_id=%d", base, eventID);

    switch (static_cast<esp_mqtt_event_id_t>(eventID))
    {
    case (MQTT_EVENT_CONNECTED):
    {
        ESP_LOGI(NETWORK_MANAGER_TAG, "MQTT Client is connected to MQTT Broker.");
        break;
    }
    case (MQTT_EVENT_DISCONNECTED):
    {
        ESP_LOGI(NETWORK_MANAGER_TAG, "MQTT Client is disconnected from MQTT Broker.");
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

esp_err_t NetworkManager::ConnectTo_MQTT_Broker(const std::string &uri)
{
    if (uri.empty())
        return ESP_ERR_INVALID_ARG;

    mMQTT_Client = new MQTT_Client("mqtt://192.168.0.104:1883", "ESP_Client");
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

    // ESP_LOGE(MQTT_CLIENT_TAG, "IMPORTATNT %s", "TEST_VALUE".);
    return ESP_OK;
}

void NetworkManager::Send()
{
    ESP_LOGI(TAG, "Test send method");
}
