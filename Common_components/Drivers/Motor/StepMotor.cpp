#include "StepMotor.hpp"

/* FreeRTOS include */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace Component::Driver::Motor;

/**
 * @brief Class constructor
 */
StepMotor::StepMotor(gpio_num_t coilA, gpio_num_t coilB, gpio_num_t coilC, gpio_num_t coilD)
{
  gpio_config_t gpio;
  gpio.pin_bit_mask = (1 << coilA) | (1 << coilB) | (1 << coilC) | (1 << coilD);
  gpio.mode = GPIO_MODE_OUTPUT;
  gpio.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio.pull_down_en = GPIO_PULLDOWN_ENABLE;
  gpio.intr_type = GPIO_INTR_DISABLE;

  gpio_config(&gpio);

  mCoils.emplace_back(coilA);
  mCoils.emplace_back(coilB);
  mCoils.emplace_back(coilC);
  mCoils.emplace_back(coilD);
}

/**
 * @brief Class constructor
 */
StepMotor::StepMotor(int coilA, int coilB, int coilC, int coilD)
    : StepMotor(
          static_cast<gpio_num_t>(coilA),
          static_cast<gpio_num_t>(coilB),
          static_cast<gpio_num_t>(coilC),
          static_cast<gpio_num_t>(coilD))
{
}

/**
 * @brief Class destructor
 */
StepMotor::~StepMotor()
{
}

/**
 * @brief Rotate motor in clockwise direction
 */
void StepMotor::RotateClockwise(uint16_t rotateAngle)
{
  uint16_t steps = CalculateSteps(CLOCKWISE_STEPS, rotateAngle);

  for (uint16_t i = 0; i < steps; ++i)
    DoOneStep<std::vector<std::bitset<4>>::iterator>(mMovement.begin(), mMovement.end());
}

/**
 * @brief Rotate motor in counter clockwise direction
 */
void StepMotor::RotateCounterClockWise(uint16_t rotateAngle)
{
  uint16_t steps = CalculateSteps(COUNTER_CLOCKWISE_STEPS, rotateAngle);

  for (uint16_t i = 0; i < steps; ++i)
    DoOneStep<std::vector<std::bitset<4>>::reverse_iterator>(mMovement.rbegin(), mMovement.rend());
}

/**
 * @brief Do one step with motor with specific iterator
 */
template <typename Itr>
void StepMotor::DoOneStep(const Itr begin, const Itr end)
{
  for (auto moveItr = begin; moveItr != end; ++moveItr)
  {
    uint8_t itrSize = moveItr->size();
    for (uint8_t i = 0; i < itrSize; ++i)
    {
      gpio_set_level(mCoils[i], moveItr->test((itrSize - 1) - i));
    }
    vTaskDelay(TIME_DELAY);
  }
  vTaskDelay(TIME_DELAY * 2);
  TurnOffCoils();
}

/**
 * @brief Turn off all coils
 */
void StepMotor::TurnOffCoils()
{
  for (auto i = mCoils.begin(); i != mCoils.end(); ++i)
    gpio_set_level(*i, 0);
}

/**
 * @brief Inline method to calculate steps based on input rotation angle
 */
inline uint16_t StepMotor::CalculateSteps(uint16_t fullCircleSteps, uint16_t rotateAngle)
{
  return fullCircleSteps / (FULL_CIRCLE_ANGLE / rotateAngle);
}

/**
 * @brief Check operation condition
 */
bool StepMotor::CheckOperationConditions()
{
  if (mCoils.empty())
    return false;

  return true;
}