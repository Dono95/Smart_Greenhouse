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
    auto bluetoothData = dynamic_cast<Component::Publisher::ClientBluetoothEventData_Greenhouse *>(eventData);
    if (!bluetoothData)
    {
        ESP_LOGE(BLUETOOTH_DATA_OBSERVER_TAG, "Unsuported data type.");
        return;
    }

    auto sensorData = std::make_shared<SensorsData>();
    // Set basic data
    sensorData->basic.clientID = bluetoothData->GetClientID();
    sensorData->basic.position = static_cast<Position>(bluetoothData->GetPosition());

    // Air values
    if (bluetoothData->IsTemperatureSet())
        sensorData->air.temperature.Set(bluetoothData->GetTemperature());

    if (bluetoothData->IsHumiditySet())
        sensorData->air.humidity.Set(bluetoothData->GetHumidity());

    if (bluetoothData->IsCO2Set())
        sensorData->air.co2.Set(bluetoothData->GetCO2());

    // Soil values
    if (bluetoothData->IsSoilMoistureSet())
        sensorData->soil.soilMoisture.Set(bluetoothData->GetSoilMoisture());

    Manager::NetworkManager::GetInstance()->SendToServer(sensorData);
}