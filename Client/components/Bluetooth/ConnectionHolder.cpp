#include "ConnectionHolder.hpp"

/* ESP Log library */
#include <esp_log.h>

using namespace Greenhouse::Bluetooth;

/**
 * @brief Class constructor with controller parameter
 */
ConnectionHolder::ConnectionHolder(const std::weak_ptr<ClientBluetoothControlller> controller, const bool &connectionStatus, uint64_t cycleTimeout)
    : mBluetoothController(controller),
      mConnectionStatus(connectionStatus),
      mCycleTimeout(cycleTimeout)
{
  const esp_timer_create_args_t timerConfig = {
      .callback = &ConnectionHolder::TimerCallback,
      .arg = this,
      /* name is optional, but may help identify the timer when debugging */
      .name = "BluetoothConnectionHolder"};

  mTimerConfig = timerConfig;

  ESP_ERROR_CHECK(esp_timer_create(&mTimerConfig, &mTimer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(mTimer, cycleTimeout * 1000));
}

/**
 * @brief Class destructor
 */
ConnectionHolder::~ConnectionHolder()
{
}

/**
 * @brief Timer callback
 */
void ConnectionHolder::TimerCallback(void *arg)
{
  const auto connectionHolder = reinterpret_cast<ConnectionHolder *>(arg);

  if (!connectionHolder->mConnectionStatus)
  {
    ESP_LOGE("ConnectionHolder", "Connection to Bluetooth server is not established. Trying to connect ... ");
    if (const auto controller = connectionHolder->mBluetoothController.lock())
      controller->StartScanning(30);
  }
}