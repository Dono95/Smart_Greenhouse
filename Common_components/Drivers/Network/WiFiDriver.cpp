/* Project specific includes */
#include "WiFiDriver.hpp"

/* ESP library includes */
#include <esp_log.h>

using namespace Component::Driver::Network;

/**
 * @brief Class constructor
 */
WifiDriver::WifiDriver(const std::string &ssid, const std::string &password,
                       WiFi_MODE mode, bool connnect)
    : mSSID(ssid),
      mEnabled(false),
      mConnected(false),
      mAttempts(0)
{
    // Clear config structure
    mConfig = {};
    // Set SSID and password to config structure -
    sprintf(reinterpret_cast<char *>(mConfig.sta.ssid), ssid.c_str());
    sprintf(reinterpret_cast<char *>(mConfig.sta.password), password.c_str());
    // Authentication mode
    mConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    mConfig.sta.pmf_cfg = {
        .capable = true,
        .required = false};

    mInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    mWiFiEventGroup = xEventGroupCreate();

    if (connnect)
    {
        Enable();
        Connect();
    }
}

/**
 * @brief Class destructor
 */
WifiDriver::~WifiDriver()
{
}

/**
 * @brieg Enable the use of the Wifi driver
 */
void WifiDriver::Enable()
{
    WiFiDriverManager::GetInstance().SetWiFiDriver(this);
    mEnabled = true;
}

/**
 * @brief Disable the use of the Wifi driver
 */
void WifiDriver::Disable()
{
    WiFiDriverManager::GetInstance().SetWiFiDriver(nullptr);
    mEnabled = false;
}

/**
 * @brief Check if the Wifi driver is enabled to use
 */
bool WifiDriver::IsEnabled() const
{
    return mEnabled;
}

bool WifiDriver::Connect()
{
    if (!IsEnabled())
    {
        ESP_LOGE(WIFI_DRIVER_TAG, "WiFi driver has not been enabled.");
        return false;
    }

    ESP_LOGI(WIFI_DRIVER_TAG, "Connecting to \"%s\" ... ", GetWifiName().c_str());

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    auto wifiEvent = [](void *arg, esp_event_base_t eventBase,
                        int32_t eventID, void *eventData)
    {
        auto &wifiManager = WiFiDriverManager::GetInstance();
        if (wifiManager.GetWiFiDriver())
            wifiManager.GetWiFiDriver()->WifiEventHandler(arg, eventBase, eventID, eventData);
    };

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        wifiEvent,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        wifiEvent,
                                                        NULL,
                                                        &instance_got_ip));

    ESP_ERROR_CHECK(esp_wifi_init(&mInitConfig));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &mConfig));
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(mWiFiEventGroup,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    CheckEventBits(bits);

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    vEventGroupDelete(mWiFiEventGroup);

    return true;
}

/**
 * @brief Check if the ESP module is connected to AP
 */
bool WifiDriver::IsConnected() const
{
    return mConnected;
}

/**
 * @brief Method to get wifi name of connnected WiFi network
 */
std::string WifiDriver::GetWifiName() const
{
    return mSSID;
}

void WifiDriver::WifiEventHandler(void *arg, esp_event_base_t eventBase,
                                  int32_t eventID, void *eventData)
{
    if (eventBase == WIFI_EVENT)
    {
        switch (eventID)
        {
        case (WIFI_EVENT_STA_START):
        {
            esp_wifi_connect();
            break;
        }
        case (WIFI_EVENT_STA_DISCONNECTED):
        {
            if (mAttempts >= MAX_NUMBER_OF_ATTEMPTS)
            {
                xEventGroupSetBits(mWiFiEventGroup, WIFI_FAIL_BIT);
                ESP_LOGE(WIFI_DRIVER_TAG, "Failed to connect to \"%s\" !", GetWifiName().c_str());
                return;
            }

            ESP_LOGI(WIFI_DRIVER_TAG, "Retry to connect to the AP ... ");
            esp_wifi_connect();
            ++mAttempts;

            break;
        }
        case (WIFI_EVENT_STA_CONNECTED):
        {
            mAttempts = 0;
            mConnected = true;

            break;
        }
        default:
            ESP_LOGW(WIFI_DRIVER_TAG, "Unhandled WiFi event with code %d.", eventID);
            break;
        }
    }
    else if (eventBase == IP_EVENT)
    {
        if (eventID == IP_EVENT_STA_GOT_IP)
        {
            ip_event_got_ip_t *ipData = (ip_event_got_ip_t *)eventData;
            mIP_Address = ipData->ip_info.ip;
            ESP_LOGI(WIFI_DRIVER_TAG, "ESP module got IP: " IPSTR, IP2STR(&mIP_Address));

            xEventGroupSetBits(mWiFiEventGroup, WIFI_CONNECTED_BIT);
        }
    }
    else
    {
        ESP_LOGW(WIFI_DRIVER_TAG, "Unhandled event base [%s].", eventBase);
        return;
    }
}

/*********************************************
 *              PRIVATE API                  *
 ********************************************/
/**
 * @brief Method to check event bits
 */
void WifiDriver::CheckEventBits(EventBits_t bits) const
{
    if (bits & WIFI_CONNECTED_BIT)
        ESP_LOGI(WIFI_DRIVER_TAG, "Successfully connected to AP \"%s\"", GetWifiName().c_str());
    else if (bits & WIFI_FAIL_BIT)
        ESP_LOGI(WIFI_DRIVER_TAG, "Failed to connect to SSID: \"%s\"", GetWifiName().c_str());
    else
        ESP_LOGE(WIFI_DRIVER_TAG, "Unexpected bits.");
}