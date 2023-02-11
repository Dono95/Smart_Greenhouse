#include "WaterPump.hpp"

using namespace Component::Driver::Active;

/**
 * @brief Class constructor
 */
WaterPump::WaterPump(gpio_num_t pump) : mWaterPump{pump}
{
  gpio_config_t gpio;
  gpio.pin_bit_mask = (1 << mWaterPump);
  gpio.mode = GPIO_MODE_OUTPUT;
  gpio.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio.pull_down_en = GPIO_PULLDOWN_ENABLE;
  gpio.intr_type = GPIO_INTR_DISABLE;

  gpio_config(&gpio);
}
/**
 * @brief Class constructor
 */
WaterPump::WaterPump(int pump) : WaterPump(static_cast<gpio_num_t>(pump))
{
}

/**
 * @brief Class destructor
 */
WaterPump::~WaterPump()
{
}

/**
 * @brief Turn water pump on
 */
void WaterPump::TurnOn()
{
  gpio_set_level(mWaterPump, 1);
}

/**
 * @brief Turn wate rpump off
 */
void WaterPump::TurnOff()
{
  gpio_set_level(mWaterPump, 0);
}