#include "BluetoothDataObserver.hpp"

/* ESP log library */
#include "esp_log.h"

using namespace Greenhouse::Observer;

/**
 * @brief Class constructor
 */
BluetoothDataObserver::BluetoothDataObserver(Greenhouse::Manager::EventManager *manager)
    : ObserverInterface(Event_T::BLUETOOTH_DATA_RECEIVED)
{
    if (!manager)
        return;

    manager->Subscribe(GetObservedEvent(), this);
}

/**
 * @brief Class destructor
 */
BluetoothDataObserver::~BluetoothDataObserver()
{
}

void BluetoothDataObserver::Update(Component::Publisher::EventData *eventData)
{
    ESP_LOGI("Observer", "Update");

    auto bluetoothData = dynamic_cast<Component::Publisher::BluetoothEventData *>(eventData);
    if (!bluetoothData)
        ESP_LOGE("Observer", "unsuported data type");
}