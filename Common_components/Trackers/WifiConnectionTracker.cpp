/* Project specific includes */
#include "Trackers/WifiConnectionTracker.hpp"

/* ESP log library */
#include <esp_log.h>

/* Indicator */
#include <Utility/Indicator/StatusIndicator.hpp>

using namespace Component::Tracker;

/**
 * @brief Class constructor
 */
WifiConnectionTracker::WifiConnectionTracker(Driver::Network::WifiDriver &wifiDriver)
    : TrackerInterface<Driver::Network::WifiDriver>(wifiDriver)
{
  const esp_timer_create_args_t timerConfig = {
      .callback = &WifiConnectionTracker::TrackerCallback,
      .arg = this,
      /* name is optional, but may help identify the timer when debugging */
      .name = "WiFiConnectionTimer"};

  TrackerInterface<Driver::Network::WifiDriver>::SetTimerConfig(timerConfig);
  TrackerInterface<Driver::Network::WifiDriver>::CreateTimer();
}

/**
 * @brief Class destructor
 */
WifiConnectionTracker::~WifiConnectionTracker()
{
}

/**
 * @brief Tracker callback
 */
void WifiConnectionTracker::TrackerCallback(void *arg)
{
  auto tracker = reinterpret_cast<WifiConnectionTracker *>(arg);

  const auto &wifiDriver = tracker->GetReferenceOfTrackedObject();
  auto indicator = Utility::Indicator::StatusIndicator::GetInstance();

  if (wifiDriver.IsConnected())
    indicator->RaiseState(Utility::Indicator::StatusCode::CLIENT_CONNECTION_ESTABLISHED);
  else
    indicator->RaiseState(Utility::Indicator::StatusCode::CLIENT_CONNECTION_NOT_ESTABLISHED);
}