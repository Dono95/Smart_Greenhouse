/* Project specific includes */
#include "GreenhouseManager.hpp"
#include "Managers/NetworkManager.h"

/* ESP logs library */
#include <esp_log.h>

/* STL library includes */
#include <utility>

/* SDK configuration file */
#include "sdkconfig.h"

using namespace Greenhouse;

GreenhouseManager *GreenhouseManager::mManagerInstance{nullptr};
std::mutex GreenhouseManager::mManagerMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
GreenhouseManager::GreenhouseManager()
    : mBluetoothController(new Bluetooth::ServerBluetoothController()),
      mBluetoothHandler(new Bluetooth::ServerBluetoothHandler(mBluetoothController))
{
}

/**
 * @brief Class destructor
 */
GreenhouseManager::~GreenhouseManager()
{
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Static method to get instance of GreenhouseManager
 */
GreenhouseManager *GreenhouseManager::GetInstance()
{
    std::lock_guard<std::mutex> lock(mManagerMutex);
    if (!mManagerInstance)
        mManagerInstance = new GreenhouseManager();

    return mManagerInstance;
}

bool GreenhouseManager::StartBluetoothServer()
{
    using BluetoothInitStatus = Component::Bluetooth::INIT_BLUETOOTH_RV;

    if (mBluetoothController->InitBluetoothController(ESP_BT_MODE_BLE) != BluetoothInitStatus::RV_BLUETOOTH_INIT_OK)
    {
        ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Initialization of bluetooth controller failed");
        return false;
    }

    if (!mBluetoothHandler->InitializeBluetoothProfiles())
    {
        ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Initialization of bluetooth profiles failed");
        return false;
    }

    if (mBluetoothController->RegisterCallbacks() != BluetoothInitStatus::RV_BLUETOOTH_INIT_OK)
    {
        ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Callbacks registration failed");
        return false;
    }

    return true;
}

/**
 * @brief Connect to WiFi network
 */
bool GreenhouseManager::ConnectToNetwork(void)
{
    return Manager::NetworkManager::GetInstance()->ConnectToWifi(std::make_pair(CONFIG_WiFi_SSID, CONFIG_WiFi_Password));
}

/**
 * @brief Connect to MQTT Broker
 */
bool GreenhouseManager::ConnectToMQTT(void)
{
    return Manager::NetworkManager::GetInstance()->ConnectTo_MQTT_Broker("mqtt://" CONFIG_MQTT_URI ":" CONFIG_MQTT_Port) == ESP_OK;
}

/**
 * @brief Getter for bluetooth handler
 */
GreenhouseManager::Shared_Bluetooth_Handler GreenhouseManager::GetHandler() const
{
    return mBluetoothHandler;
}