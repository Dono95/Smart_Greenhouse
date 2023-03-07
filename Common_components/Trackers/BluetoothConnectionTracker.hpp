#ifndef BLUETOOTH_CONNECTION_TRACKER_H
#define BLUETOOTH_CONNECTION_TRACKER_H

/* Tracker interface */
#include "Trackers/TrackerInterface.hpp"

#define BLUETOOTH_CONNECTION_TRACKER_TAG "BluetoothConnectionTracker"

namespace Component
{
  namespace Tracker
  {
    class BluetoothConnectionTracker : public TrackerInterface<bool>
    {
    public:
      /**
       * @brief Class constructor
       *
       * @param[in] trackedObject : Reference to tracked object
       */
      BluetoothConnectionTracker(const bool &trackedObject);

      /**
       * @brief Class constructor
       *
       * @param[in] trackedObject : Reference to tracked object
       * @param[in] timerConfig   : Timer configuration
       */
      BluetoothConnectionTracker(const bool &trackedObject, const esp_timer_create_args_t &timerConfig);

      /**
       * @brief Class destructor
       */
      virtual ~BluetoothConnectionTracker();

    private:
      /**
       * @brief Tracker callback
       *
       * @param[in] arg : Callback argument
       */
      static void TrackerCallback(void *arg);

      /* Timer configuration */
      esp_timer_create_args_t mTimerConfig;
    };
  } // namespace Tracker
} // namespace Component

#endif // BLUETOOTH_CONNECTION_TRACKER_H