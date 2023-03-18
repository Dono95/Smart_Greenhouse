#ifndef WIFI_CONNECTION_HOLDER_H
#define WIFI_CONNECTION_HOLDER_H

/* ESP Timer library */
#include <esp_timer.h>

/* Wifi driver */
#include <Drivers/Network/WiFiDriver.hpp>

#define WIFI_CONNECTION_HOLDER_TAG "Wifi connection holder"

namespace Greenhouse
{
  class WifiConnectionHolder
  {
  public:
    /**
     * @brief Class constructor
     *
     * @param[in] wifiDriver        : Wifi driver
     * @param[in] cycleTimeout      : Cycle timeout
     */
    explicit WifiConnectionHolder(Component::Driver::Network::WifiDriver *wifiDriver, const uint64_t cycleTimeout);

    /**
     * @brief Class destructor
     */
    ~WifiConnectionHolder();

  private:
    /**
     * @brief Timer callback
     *
     * @param[in] arg : Callback argument
     */
    static void TimerCallback(void *arg);

    /**
     * @brief Connect to network
     *
     * @param[in] taskArg : Wifi driver
     */
    static void ConnectToNetwork(void *taskArg);

    /* Wifi driver */
    Component::Driver::Network::WifiDriver *mWifiDriver;

    /* ESP Timer */
    esp_timer_handle_t mTimer;

    /* Timer cycle timeout */
    uint64_t mCycleTimeout;

    /* Timer configuration */
    esp_timer_create_args_t mTimerConfig;
  };
}; // namespace Greenhouse

#endif // WIFI_CONNECTION_HOLDER_H