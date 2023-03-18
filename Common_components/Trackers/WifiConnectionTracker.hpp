#ifndef WIFI_CONNECTION_TRACKER_H
#define WIFI_CONNECTION_TRACKER_H

/* Tracker interface */
#include "Trackers/TrackerInterface.hpp"

/* Wifi driver */
#include <Drivers/Network/WiFiDriver.hpp>

#define WIFI_CONNECTION_TRACKER_TAG "WifiConnectionTracker"

namespace Component
{
  namespace Tracker
  {
    class WifiConnectionTracker : public TrackerInterface<Driver::Network::WifiDriver>
    {
    public:
      /**
       * @brief Class constructor
       *
       * @param[in] wifiDriver : Pointer to wifi driver
       */
      WifiConnectionTracker(Driver::Network::WifiDriver &wifiDriver);

      /**
       * @brief Class destructor
       */
      virtual ~WifiConnectionTracker();

    private:
      /**
       * @brief Tracker callback
       *
       * @param[in] arg : Callback argument
       */
      static void TrackerCallback(void *arg);
    };
  } // namespace Tracker
} // namespace Component

#endif // BLUETOOTH_CONNECTION_TRACKER_H