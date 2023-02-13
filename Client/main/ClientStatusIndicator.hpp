#ifndef CLIENT_STATUS_INDICATOR_H
#define CLIENT_STATUS_INDICATOR_H

/* STD library */
#include <mutex>

/* Common components */
#include "Utility/Indicator/RGB.hpp"

/* SDK config file */
#include "sdkconfig.h"

#define RED_PIN CONFIG_RED_PIN
#define GREEN_PIN CONFIG_GREEN_PIN
#define BLUE_PIN CONFIG_BLUE_PIN

namespace Greenhouse
{
  enum class StateCode
  {
    BLUETOOTH_INIT_SUCCESSED,
    BLUETOOTH_INIT_FAILED,
    CLIENT_NOT_CONNECTED_TO_BLE_SERVER,
    CLIENT_CONNECTED_TO_BLE_SERVER

  };

  class ClientStatusIndicator
  {
  public:
    /**
     * @brief Static method to get instance of GreenhouseManager
     */
    static ClientStatusIndicator *GetInstance();

    /**
     * @brief Method to raise actual status of client during procession of different event
     *
     * @param[in] stateCode   : Client status code for indicator object
     */
    void RaiseState(StateCode stateCode);

    /**
     * @brief Method to clean statuc from indicator object
     */
    void CleanState();

  private:
    /**
     * @brief Class constructor
     */
    explicit ClientStatusIndicator();

    /**
     * @brief Class destructor
     */
    ~ClientStatusIndicator();

    /* Singleton instance of ClientStatusIndicator object */
    static ClientStatusIndicator *mIndicatorInstance;

    /* Singleton mutex to protect instance from multithread */
    static std::mutex mIndicatorMutex;

    /* Indicator object */
    Utility::Indicator::RGB *mLed;
  };
} // namespace Greenhouse

#endif // CLIENT_STATUS_INDICATOR_H