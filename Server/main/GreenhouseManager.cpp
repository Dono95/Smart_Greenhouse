/* Project specific includes */
#include "GreenhouseManager.hpp"

/* ESP logs library */
#include "esp_log.h"

using namespace Greenhouse;

GreenhouseManager *GreenhouseManager::mManagerInstance{nullptr};
std::mutex GreenhouseManager::mManagerMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
GreenhouseManager::GreenhouseManager() : mBluetoothController(new Bluetooth::BluetoothController())
{
    mBluetoothHandler = std::shared_ptr<Bluetooth::ServerBluetoothHandler>(new Bluetooth::ServerBluetoothHandler());
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

bool GreenhouseManager::StartBluetooth()
{
    using Bluetooth_Initialization_RV = Bluetooth::BluetoothController::INIT_BLUETOOTH_RV;

    if (!mBluetoothHandler->InitializeBluetoothProfiles())
    {
        ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Initialization of bluetooth profiles failed");
        return false;
    }

    if (mBluetoothController->InitBluetoothController(ESP_BT_MODE_BLE) != Bluetooth_Initialization_RV::RV_BLUETOOTH_INIT_OK)
    {
        ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Initialization of bluetooth controller failed");
        return false;
    }

    return true;
}

GreenhouseManager::Shared_BaseHandlerInterface GreenhouseManager::GetBluetoothHandler() const
{
    return mBluetoothHandler;
}