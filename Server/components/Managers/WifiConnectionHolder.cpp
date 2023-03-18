#include "WifiConnectionHolder.hpp"

/* ESP Log library */
#include <esp_log.h>

/* FreeRTOS*/
#include <freertos/task.h>

using namespace Greenhouse;

/**
 * @brief Class constructor
 */
WifiConnectionHolder::WifiConnectionHolder(Component::Driver::Network::WifiDriver *wifiDriver, const uint64_t cycleTimeout)
    : mWifiDriver(wifiDriver),
      mCycleTimeout(cycleTimeout)
{
  const esp_timer_create_args_t timerConfig = {
      .callback = &WifiConnectionHolder::TimerCallback,
      .arg = this,
      /* name is optional, but may help identify the timer when debugging */
      .name = "WifiConnectionHolder"};

  mTimerConfig = timerConfig;

  ESP_ERROR_CHECK(esp_timer_create(&mTimerConfig, &mTimer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(mTimer, cycleTimeout * 1000));
}

/**
 * @brief Class destructor
 */
WifiConnectionHolder::~WifiConnectionHolder()
{
}

/**
 * @brief Timer callback
 */
void WifiConnectionHolder::TimerCallback(void *arg)
{
  const auto wifiConnectionHolder = reinterpret_cast<WifiConnectionHolder *>(arg);

  if (wifiConnectionHolder->mWifiDriver->IsConnected() || wifiConnectionHolder->mWifiDriver->IsTryingToConnect())
    return;

  /* Create the task, storing the handle. */
  auto status = xTaskCreate(
      WifiConnectionHolder::ConnectToNetwork, /* Function that implements the task. */
      "WifiConnectionTask",                   /* Text name for the task. */
      4096,                                   /* Stack size in words, not bytes. */
      wifiConnectionHolder->mWifiDriver,      /* Parameter passed into the task. */
      tskIDLE_PRIORITY,                       /* Priority at which the task is created. */
      nullptr);                               /* Used to pass out the created task's handle. */

  if (status == pdPASS)
  {
    ESP_LOGI(WIFI_CONNECTION_HOLDER_TAG, "Task for connecting to network has been successfully created");
  }
}

/**
 * @brief Connect to network
 */
void WifiConnectionHolder::ConnectToNetwork(void *taskArg)
{
  auto wifiDriver = static_cast<Component::Driver::Network::WifiDriver *>(taskArg);
  if (!wifiDriver)
  {
    ESP_LOGE(WIFI_CONNECTION_HOLDER_TAG, "Unsuported type.");
    return;
  }

  printf("Task\n");
  wifiDriver->Connect();

  vTaskDelete(nullptr);
}