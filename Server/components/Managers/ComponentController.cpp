#include "ComponentController.hpp"

using namespace Greenhouse::Manager;

ComponentController *ComponentController::mControllerInstance{nullptr};
std::mutex ComponentController::mControllerMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/
/**
 * @brief Class constructor
 */
ComponentController::ComponentController()
    : mWindowMotor(new Component::Driver::Motor::StepMotor(
          CONFIG_COIL_A,
          CONFIG_COIL_B,
          CONFIG_COIL_C,
          CONFIG_COIL_D)),
      mWaterPump(new Component::Driver::Active::WaterPump(CONFIG_WATER_PUMP))
{
}

/**
 * @brief Class destructor
 */
ComponentController::~ComponentController()
{
  if (mWindowMotor)
  {
    delete mWindowMotor;
    mWindowMotor = nullptr;
  }

  if (mWaterPump)
  {
    delete mWaterPump;
    mWaterPump = nullptr;
  }
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/
/**
 * @brief Static method to get singleton instance of component controller
 */
ComponentController *ComponentController::GetInstance()
{
  std::lock_guard<std::mutex> lock(mControllerMutex);
  if (!mControllerInstance)
    mControllerInstance = new ComponentController();

  return mControllerInstance;
}

/**
 * @brief Open window method
 */
void ComponentController::OpenWindow()
{
  mWindowMotor->RotateClockwise();
}

/**
 * @brief Open window
 */
void ComponentController::OpenWindow(uint8_t percentage)
{
}

/**
 * @brief Close window method
 */
void ComponentController::CloseWindow()
{
  mWindowMotor->RotateCounterClockWise();
}

/**
 * @brief Method to turn on irrigation
 */
void ComponentController::TurnOnIrrigation()
{
  mWaterPump->TurnOn();
}

/**
 * @brief Method to turn off irrigation
 */
void ComponentController::TurnOffIrrigation()
{
  mWaterPump->TurnOff();
}