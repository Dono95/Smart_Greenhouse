/* Project specific includes */
#include "BluetoothDataObserver.hpp"
#include "NetworkManager.h"
#include "SensorsData/SensorsData.hpp"

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
    auto bluetoothData = dynamic_cast<Component::Publisher::BluetoothEventData *>(eventData);
    if (!bluetoothData)
    {
        ESP_LOGE(BLUETOOTH_DATA_OBSERVER_TAG, "Unsuported data type.");
        return;
    }

    Manager::NetworkManager::GetInstance()->SendToServer(std::make_shared<SensorsData>(bluetoothData->GetTemperature(), bluetoothData->GetHumanity()));
}