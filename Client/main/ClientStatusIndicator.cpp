/* Project specific includes */
#include "ClientStatusIndicator.hpp"

/* ESP library */
#include <driver/gpio.h>

using namespace Greenhouse;

ClientStatusIndicator *ClientStatusIndicator::mIndicatorInstance{nullptr};
std::mutex ClientStatusIndicator::mIndicatorMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
ClientStatusIndicator::ClientStatusIndicator()
    : mLed(new Utility::Indicator::RGB(RED_PIN, GREEN_PIN, BLUE_PIN))
{
}

/**
 * @brief Class destructor
 */
ClientStatusIndicator::~ClientStatusIndicator()
{
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Static method to get instance of ClientStatusIndicator
 */
ClientStatusIndicator *ClientStatusIndicator::GetInstance()
{
  std::lock_guard<std::mutex> lock(mIndicatorMutex);
  if (!mIndicatorInstance)
    mIndicatorInstance = new ClientStatusIndicator();

  return mIndicatorInstance;
}

/**
 * @brief Method to raise actual status of client during procession of different event
 *
 * @param[in] stateCode   : Client status code for indicator object
 */
void ClientStatusIndicator::RaiseState(StateCode stateCode)
{
  // Alias for RGB led color enum
  using LedColor = Utility::Indicator::Color;

  switch (stateCode)
  {
  case (StateCode::BLUETOOTH_INIT_SUCCESSED):
  {
    mLed->MultipleBlick(LedColor::GREEN, 3, 200);
    break;
  }
  case (StateCode::BLUETOOTH_INIT_FAILED):
  {
    mLed->SwitchOn(LedColor::RED);
    break;
  }
  case (StateCode::CLIENT_NOT_CONNECTED_TO_BLE_SERVER):
  {
    mLed->Blick(LedColor::RED, 100);
    break;
  }
  case (StateCode::CLIENT_CONNECTED_TO_BLE_SERVER):
  {
    mLed->Blick(LedColor::BLUE_LIGHT, 100);
    break;
  }
  default:
    break;
  }
}

/**
 * @brief Method to clean statuc from indicator object
 */
void ClientStatusIndicator::CleanState()
{
  mLed->SwitchOff();
}