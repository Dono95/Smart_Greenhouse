#ifndef BLUETOOTH_CONNECTION_TRACKER_H
#define BLUETOOTH_CONNECTION_TRACKER_H

/* Tracker interface */
#include "Trackers/TrackerInterface.hpp"

namespace Component
{
  namespace Tracker
  {
    class BluetoothConnectionTracker : public TrackerInterface<bool>
    {
    public:
      /**
       * @brief Class constructor
       */
      BluetoothConnectionTracker(const bool &trackedObject);

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
    };
  } // namespace Tracker
} // namespace Component

#endif // BLUETOOTH_CONNECTION_TRACKER_H