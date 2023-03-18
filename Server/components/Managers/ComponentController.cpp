#include "ComponentController.hpp"

using namespace Greenhouse::Manager;

#define MOTOR_MAX_ROTAION_FOR_WINDOW 180

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
      mWindowState{false},
      mWaterPump(new Component::Driver::Active::WaterPump(CONFIG_WATER_PUMP)),
      mIrrigationState{false}
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
  mWindowMotor->RotateCounterClockWise(MOTOR_MAX_ROTAION_FOR_WINDOW);
  mWindowState = true;
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
  mWindowMotor->RotateClockwise(MOTOR_MAX_ROTAION_FOR_WINDOW);
  mWindowState = false;
}

/**
 * @brief Check if window is open
 */
bool ComponentController::IsWindowOpen() const
{
  return mWindowState;
}

/**
 * @brief Get window current state
 */
bool ComponentController::WindowState() const
{
  return mWindowState;
}

/**
 * @brief Method to turn on irrigation
 */
void ComponentController::TurnOnIrrigation()
{
  mWaterPump->TurnOn();
  mIrrigationState = true;
}

/**
 * @brief Method to turn off irrigation
 */
void ComponentController::TurnOffIrrigation()
{
  mWaterPump->TurnOff();
  mIrrigationState = false;
}

/**
 * @brief Check if irrigation is turn on
 */
bool ComponentController::IsIrrigationTurnOn() const
{
  return mIrrigationState;
}

/**
 * @brief Get irrigation current state
 */
bool ComponentController::IrrigationState() const
{
  return mIrrigationState;
}