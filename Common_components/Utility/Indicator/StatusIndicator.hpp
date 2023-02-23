#ifndef STATUS_INDICATOR_H
#define STATUS_INDICATOR_H

/* STD library */
#include <mutex>
#include <vector>

/* Indicator */
#include "RGB.hpp"

/* SDK config */
#include "sdkconfig.h"

#ifdef CONFIG_RED_PIN
#define RED_PIN CONFIG_RED_PIN
#else
#define RED_PIN 14
#endif

#ifdef CONFIG_GREEN_PIN
#define GREEN_PIN CONFIG_GREEN_PIN
#else
#define GREEN_PIN 26
#endif

#ifdef CONFIG_BLUE_PIN
#define BLUE_PIN CONFIG_BLUE_PIN
#else
#define BLUE_PIN 27
#endif

namespace Utility
{
  namespace Indicator
  {
    enum class StatusCode

    {
      BLUETOOTH_INIT_SUCCESSED,
      BLUETOOTH_INIT_FAILED,
      CLIENT_NOT_CONNECTED_TO_BLE_SERVER,
      CLIENT_CONNECTED_TO_BLE_SERVER
    };

    class StatusIndicator
    {
    public:
      /**
       * @brief Static method to get instance of StatusIndicator
       *
       * @return StatusIndicator  : Pointer to StatusIndicator
       */
      static StatusIndicator *GetInstance();

      /**
       * @brief Raise actual status for different event
       *
       * @param[in] stateCode   : Status code for indicator object
       */
      virtual void RaiseState(StatusCode stateCode);

      /**
       * @brief Method to clean statuc from indicator object
       */
      virtual void CleanState();

    protected:
      /**
       * @brief Class constructor
       */
      explicit StatusIndicator();

      /**
       * @brief Class destructor
       */
      virtual ~StatusIndicator();

    private:
      /* Singleton instance of StatusIndicator object */
      static StatusIndicator *mIndicatorInstance;

      /* Singleton mutex to protect instance from multithread */
      static std::mutex mIndicatorMutex;

      // Indicator component
      RGB *mLed;
    };
  } // namespace Indicator
} // namespace Utility

#endif // Common_components