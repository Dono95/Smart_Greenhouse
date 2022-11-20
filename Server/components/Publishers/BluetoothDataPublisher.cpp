/* Project specific includes */
#include "BluetoothDataPublisher.hpp"

/* ESP log library includes */
#include "esp_log.h"

/* STL library includes */
#include <algorithm>

using namespace Greenhouse::Publisher;

BluetoothDataPublisher *BluetoothDataPublisher::mPublisherInstance{nullptr};
std::mutex BluetoothDataPublisher::mPublisherMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
BluetoothDataPublisher::BluetoothDataPublisher()
{
}

/**
 * @brief Class destructor
 */
BluetoothDataPublisher::~BluetoothDataPublisher()
{
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/
/**
 * @brief Static method to get singleton instance of Bluetooth data publisher
 */
BluetoothDataPublisher *BluetoothDataPublisher::GetInstance()
{
    std::lock_guard<std::mutex> lock(mPublisherMutex);
    if (!mPublisherInstance)
        mPublisherInstance = new BluetoothDataPublisher();

    return mPublisherInstance;
}

/**
 * @brief Method to subscribe observer to Bluetooth data publisher
 *
 * @param[in] observer  : Pointer to observer object
 */
void BluetoothDataPublisher::Subscribe(Observer_I *observer)
{
    if (!observer)
        return;

    mObservers.emplace_back(observer);
    ESP_LOGD(BLUETOOTH_DATA_PUBLISHER_TAG, "Observer is now subscribing.");
}

/**
 * @brief Method to subscribe observer from Bluetooth data publisher
 *
 * @param[in] observer  : Pointer to observer object
 */
void BluetoothDataPublisher::Unsubscribe(Observer_I *observer)
{
    if (!observer)
        return;

    auto observerItr = std::find(mObservers.begin(), mObservers.end(), observer);
    if (observerItr == mObservers.end())
    {
        ESP_LOGD(BLUETOOTH_DATA_PUBLISHER_TAG, "Provided observer not found. No need to unsubscribe.");
        return;
    }

    mObservers.erase(observerItr);
    ESP_LOGD(BLUETOOTH_DATA_PUBLISHER_TAG, "Observer is no longer subscribe.");
}

/**
 * @brief Method to notify observer about new data from bluetooth handler
 */
void BluetoothDataPublisher::Notify()
{
}