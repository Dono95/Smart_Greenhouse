/* Project specific includes */
#include "GreenhouseManager.hpp"

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