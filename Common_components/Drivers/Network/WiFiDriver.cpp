/* Project specific includes */
#include "WiFiDriver.hpp"

/* ESP library includes */
#include <esp_log.h>

using namespace Component::Driver::Network;

/**
 * @brief Class constructor
 */
WifiDriver::WifiDriver(const std::string &ssid, const std::string &password,
                       WiFi_MODE mode)
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

    // Create esp event loop for WiFi events
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize the underlying TCP/IP stack.
    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        [](void *arg, esp_event_base_t eventBase,
           int32_t eventID, void *eventData)
        {
            auto &wifiManager = WiFiDriverManager::GetInstance();
            if (wifiManager.GetWiFiDriver())
                wifiManager.GetWiFiDriver()->WifiEventHandler(arg, eventBase, eventID, eventData);
        },
        nullptr,
        nullptr));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        [](void *arg, esp_event_base_t eventBase,
           int32_t eventID, void *eventData)
        {
            auto &wifiManager = WiFiDriverManager::GetInstance();
            if (wifiManager.GetWiFiDriver())
            {
                ip_event_got_ip_t *ipData = (ip_event_got_ip_t *)eventData;
                wifiManager.GetWiFiDriver()->SetIpAddress(ipData->ip_info.ip);
            }
        },
        NULL,
        nullptr));

    mIP_Address.addr = 0;

    ESP_ERROR_CHECK(esp_wifi_init(&mInitConfig));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &mConfig));

    Enable();
    ESP_ERROR_CHECK(esp_wifi_start());
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

/**
 * @brief Method to connect ot WiFi
 */
bool WifiDriver::Connect()
{
    // Unable to continue if wifi driver is not enabled
    if (!IsEnabled())
    {
        ESP_LOGE(WIFI_DRIVER_TAG, "WiFi driver has not been enabled.");
        return false;
    }

    if (IsConnected())
        return true;

    ESP_LOGI(WIFI_DRIVER_TAG, "Connecting to \"%s\" ... ", GetWifiName().c_str());
    esp_wifi_connect();

    // Wait to connect to AP
    uint16_t timeoutAttempts = DEFAULT_MAX_TIMEOUT / 100;
    for (uint8_t i = 0; i < timeoutAttempts; ++i)
    {
        if (mAttempts >= MAX_NUMBER_OF_ATTEMPTS || IsConnected())
            break;

        vTaskDelay(100);
    }

    return IsConnected();
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

/**
 * @brief Get IP address
 *
 * @return esp_ip4_addr_t
 */
esp_ip4_addr_t WifiDriver::GetIpAddress() const
{
    return mIP_Address;
}

/**
 * @brief Set IP address
 */
void WifiDriver::SetIpAddress(esp_ip4_addr_t &ip_address)
{
    mIP_Address = ip_address;
    ESP_LOGI(WIFI_DRIVER_TAG, "IP address set to " IPSTR, IP2STR(&mIP_Address));
}

void WifiDriver::WifiEventHandler(void *arg, esp_event_base_t eventBase,
                                  int32_t eventID, void *eventData)
{
    ESP_LOGW(WIFI_DRIVER_TAG, "Event %d.", eventID);
    if (eventBase == WIFI_EVENT)
    {
        switch (eventID)
        {
        case (WIFI_EVENT_STA_START):
        {
            break;
        }
        case (WIFI_EVENT_STA_DISCONNECTED):
        {
            if (mAttempts >= MAX_NUMBER_OF_ATTEMPTS)
            {
                mAttempts = 0;
                mConnected = false;
                ESP_LOGE(WIFI_DRIVER_TAG, "Maximal number of attempts reached. Failed to connect to \"%s\" !", GetWifiName().c_str());
                break;
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
    else
    {
        ESP_LOGW(WIFI_DRIVER_TAG, "Unhandled event base [%s].", eventBase);
        return;
    }
}

/*********************************************
 *              PRIVATE API                  *
 ********************************************/
