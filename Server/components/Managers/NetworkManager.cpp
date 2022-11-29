/* Project specific includes */
#include "NetworkManager.h"

/* ESP log library*/
#include <esp_log.h>

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