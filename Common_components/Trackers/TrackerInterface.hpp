#ifndef TRACKER_INTERFACE_H
#define TRACKER_INTERFACE_H

/* ESP library */
#include <esp_timer.h>

namespace Component
{
  namespace Tracker
  {
    template <typename T>
    class TrackerInterface
    {
    public:
      /**
       * @brief Class constructor
       */
      TrackerInterface(const T &trackedObject) : mTrackedObject(trackedObject) {}

      /**
       * @brief Class destructor
       */
      virtual ~TrackerInterface() = default;

      /**
       * @brief Start object tracking
       *
       * @param[in] period  : Period for generate interrupt in ms
       */
      virtual void StartTracking(uint64_t period)
      {
        ESP_ERROR_CHECK(esp_timer_start_periodic(mTimer, period * 1000));
      }

      /**
       * @brief Stop object tracking
       */
      virtual void StopTracking()
      {
        ESP_ERROR_CHECK(esp_timer_stop(mTimer));
      }

    protected:
      /**
       * @brief Get content of tracked object
       *
       * @return T : Content of tracked object
       */
      T GetValueOfTrackedObject() const { return mTrackedObject; }

      /**
       * @brief Get timer
       *
       * @return esp_timer_handle_t : Pointer to esp timer
       */
      esp_timer_handle_t GetTimer() const { return mTimer; }

      /**
       * @brief Set config for timer
       *
       * @param[in] timerConfig : Timer configuration file
       */
      void SetTimerConfig(const esp_timer_create_args_t &timerConfig)
      {
        mTimerConfig = timerConfig;
      }

      /**
       * @brief Create timer
       */
      void CreateTimer()
      {
        ESP_ERROR_CHECK(esp_timer_create(&mTimerConfig, &mTimer));
      }

    private:
      /* Const reference to tracked object */
      const T &mTrackedObject;

      /* Timer for generating interrupt and periodically track state of object */
      esp_timer_create_args_t mTimerConfig;

      /* ESP timer */
      esp_timer_handle_t mTimer;
    };
  } // namespace Tracker
} // namespace Component

#endif // TRACKER_INTERFACE_H