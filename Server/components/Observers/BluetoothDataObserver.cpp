/* Project specific includes */
#include "BluetoothDataObserver.hpp"
#include "NetworkManager.h"
#include "SensorsData/SensorsData.hpp"

/* ESP log library */
#include "esp_log.h"

/* FreeRTOS*/
#include <freertos/task.h>

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
    /* Create the task, storing the handle. */
    auto status = xTaskCreate(
        BluetoothDataObserver::HandleBluetoothData, /* Function that implements the task. */
        "BluetoothDataTask",                        /* Text name for the task. */
        4096,                                       /* Stack size in words, not bytes. */
        eventData,                                  /* Parameter passed into the task. */
        tskIDLE_PRIORITY,                           /* Priority at which the task is created. */
        nullptr);                                   /* Used to pass out the created task's handle. */

    if (status == pdPASS)
    {
        ESP_LOGI(BLUETOOTH_DATA_OBSERVER_TAG, "Task for handling bluetooth data has been successfully created");
    }
}

/**
 * @brief Handle bluetooth data
 */
void BluetoothDataObserver::HandleBluetoothData(void *event_data)
{
    auto bluetoothData = static_cast<Component::Publisher::ClientBluetoothEventData_Greenhouse *>(event_data);
    if (!bluetoothData)
    {
        ESP_LOGE(BLUETOOTH_DATA_OBSERVER_TAG, "Unsuported data type.");
        return;
    }

    ESP_LOGD(BLUETOOTH_DATA_OBSERVER_TAG, "Start processing bluetooth event data");
    auto sensorData = std::make_shared<SensorsData>();
    // Set basic data
    sensorData->basic.clientID = bluetoothData->GetClientID();
    sensorData->basic.position = static_cast<Position>(bluetoothData->GetPosition());
    ESP_LOGD(BLUETOOTH_DATA_OBSERVER_TAG, "Data for client %d on position %d", bluetoothData->GetClientID(), bluetoothData->GetPosition());

    // Air values
    if (bluetoothData->IsTemperatureSet())
    {
        sensorData->air.temperature.Set(bluetoothData->GetTemperature());
        ESP_LOGD(BLUETOOTH_DATA_OBSERVER_TAG, "Temperature: %.2f", sensorData->air.temperature.Get());
    }

    if (bluetoothData->IsHumiditySet())
    {
        sensorData->air.humidity.Set(bluetoothData->GetHumidity());
        ESP_LOGD(BLUETOOTH_DATA_OBSERVER_TAG, "Humidity: %.2f", sensorData->air.humidity.Get());
    }

    if (bluetoothData->IsCO2Set())
    {
        sensorData->air.co2.Set(bluetoothData->GetCO2());
        ESP_LOGD(BLUETOOTH_DATA_OBSERVER_TAG, "CO2: %d", sensorData->air.co2.Get());
    }

    // Soil values
    if (bluetoothData->IsSoilMoistureSet())
    {
        sensorData->soil.soilMoisture.Set(bluetoothData->GetSoilMoisture());
        ESP_LOGD(BLUETOOTH_DATA_OBSERVER_TAG, "Soil moisture: %.2f", sensorData->soil.soilMoisture.Get());
    }

    Manager::NetworkManager::GetInstance()->SendToServer(sensorData);

    vTaskDelete(nullptr);
}