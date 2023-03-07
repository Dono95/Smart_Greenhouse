#include "StatusIndicator.hpp"

using namespace Utility::Indicator;

StatusIndicator *StatusIndicator::mIndicatorInstance{nullptr};
std::mutex StatusIndicator::mIndicatorMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/
/**
 * @brief Class constructor
 */
StatusIndicator::StatusIndicator()
    : mLed(new RGB(RED_PIN, GREEN_PIN, BLUE_PIN))
{
}

/**
 * @brief Class destructor
 */
StatusIndicator::~StatusIndicator()
{
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/
/**
 * @brief Static method to get instance of StatusIndicator
 */
StatusIndicator *StatusIndicator::GetInstance()
{
  std::lock_guard<std::mutex> lock(mIndicatorMutex);
  if (!mIndicatorInstance)
    mIndicatorInstance = new StatusIndicator();

  return mIndicatorInstance;
}

/**
 * @brief Raise actual status for different event
 */
void StatusIndicator::RaiseState(StatusCode stateCode)
{
  switch (stateCode)
  {
  case (StatusCode::BLUETOOTH_INIT_SUCCESSED):
  {
    mLed->MultipleBlick(Color::GREEN, 3, 200);
    break;
  }
  case (StatusCode::BLUETOOTH_INIT_FAILED):
  {
    mLed->SwitchOn(Color::RED);
    break;
  }
  case (StatusCode::CLIENT_NOT_CONNECTED_TO_BLE_SERVER):
  {
    mLed->Blick(Color::RED, 100);
    break;
  }
  case (StatusCode::CLIENT_CONNECTED_TO_BLE_SERVER):
  {
    mLed->Blick(Color::BLUE_LIGHT, 100);
    break;
  }
  case (StatusCode::CLIENT_CONNECTING_TO_NETWORK):
  {
    mLed->SwitchOn(Color::YELLOW);
    break;
  }
  case (StatusCode::CLIENT_CONNECTION_FAILED):
  {
    mLed->SwitchOn(Color::RED);
    break;
  }
  case (StatusCode::CLIENT_CONNECTION_ESTABLISHED):
  {
    mLed->MultipleBlick(Color::BLUE, 5, 100);
    break;
  }
  default:
    break;
  }
}

/**
 * @brief Method to clean statuc from indicator object
 */
void StatusIndicator::CleanState()
{
  mLed->SwitchOff();
}