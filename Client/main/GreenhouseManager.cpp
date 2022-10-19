/* Project specific includes */
#include "GreenhouseManager.hpp"

/* ESP logs library */
#include "esp_log.h"

using namespace Greenhouse;

GreenhouseManager *GreenhouseManager::mManagerInstance{nullptr};

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
GreenhouseManager::GreenhouseManager() : mBluetoothController(new BluetoothController())
{
    if (mBluetoothController->InitBluetoothController(ESP_BT_MODE_BLE) != BluetoothController::INIT_BLUETOOTH_RV::RV_BLUETOOTH_INIT_OK) {
        ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Initialization of bluetooth controller failed");
    }
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
    if (!mManagerInstance)
        mManagerInstance = new GreenhouseManager();

    return mManagerInstance;
}