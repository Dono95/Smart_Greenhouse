/* Project specific includes */
#include "Trackers/BluetoothConnectionTracker.hpp"

/* ESP log library */
#include <esp_log.h>

#include "../Client/main/ClientStatusIndicator.hpp"

using namespace Component::Tracker;

/**
 * @brief Class constructor
 */
BluetoothConnectionTracker::BluetoothConnectionTracker(const bool &trackedObject) : TrackerInterface<bool>(trackedObject)
{
  const esp_timer_create_args_t timerConfig = {
      .callback = &BluetoothConnectionTracker::TrackerCallback,
      .arg = this,
      /* name is optional, but may help identify the timer when debugging */
      .name = "BluetoothConnectionTimer"};

  TrackerInterface<bool>::SetTimerConfig(timerConfig);
  TrackerInterface<bool>::CreateTimer();
}

/**
 * @brief Class destructor
 */
BluetoothConnectionTracker::~BluetoothConnectionTracker()
{
}

/**
 * @brief Tracker callback
 */
void BluetoothConnectionTracker::TrackerCallback(void *arg)
{
  auto tracker = reinterpret_cast<BluetoothConnectionTracker *>(arg);
  auto indicator = Greenhouse::ClientStatusIndicator::GetInstance();

  if (tracker->GetValueOfTrackedObject())
    indicator->RaiseState(Greenhouse::StateCode::CLIENT_CONNECTED_TO_BLE_SERVER);
  else
    indicator->RaiseState(Greenhouse::StateCode::CLIENT_NOT_CONNECTED_TO_BLE_SERVER);
}