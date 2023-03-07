#ifndef CONNECTION_HOLDER_H
#define CONNECTION_HOLDER_H

/* Project specific includes*/
#include "ClientBluetoothController.hpp"

/* ESP Timer library */
#include <esp_timer.h>

/* STD library */
#include <stdint.h>
#include <memory>

namespace Greenhouse
{
  namespace Bluetooth
  {
    class ConnectionHolder
    {
    public:
      /**
       * @brief Class constructor with controller parameter
       *
       * @param[in] controller        : Bluetooth controller
       * @param[in] connectionStatus  : Connection status from ClientBluetoothHandler
       * @param[in] cycleTimeout      : Cycle for timer to generate interrupt
       */
      explicit ConnectionHolder(const std::weak_ptr<ClientBluetoothControlller> controller,
                                const bool &connectionStatus,
                                uint64_t cycleTimeout);

      /**
       * @brief Class destructor
       */
      ~ConnectionHolder();

    private:
      /**
       * @brief Timer callback
       *
       * @param[in] arg : Callback argument
       */
      static void TimerCallback(void *arg);

      /* Pointer to bluetooth controller */
      std::weak_ptr<ClientBluetoothControlller> mBluetoothController;

      /* Connection status */
      const bool &mConnectionStatus;

      /* ESP Timer */
      esp_timer_handle_t mTimer;

      /* Timer cycle timeout */
      uint64_t mCycleTimeout;

      /* Timer configuration */
      esp_timer_create_args_t mTimerConfig;
    };
  }; // namespace Bluetooth

}; // namespace Greenhouse

#endif // CONNECTION_HOLDER_H