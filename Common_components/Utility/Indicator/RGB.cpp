#include "RGB.hpp"

/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace Utility::Indicator;

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Class destructor
 */
RGB::RGB(gpio_num_t red, gpio_num_t green, gpio_num_t blue)
    : mRed(red),
      mGreen(green),
      mBlue(blue),
      mColor(Color::WHITE)
{
  gpio_config_t gpio;
  gpio.pin_bit_mask = (1 << red) | (1 << green) | (1 << blue);
  gpio.mode = GPIO_MODE_OUTPUT;
  gpio.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio.pull_down_en = GPIO_PULLDOWN_ENABLE;
  gpio.intr_type = GPIO_INTR_DISABLE;

  gpio_config(&gpio);
}

/**
 * @brief Class constructor
 */
RGB::RGB(uint8_t red, uint8_t green, uint8_t blue)
    : RGB(static_cast<gpio_num_t>(red), static_cast<gpio_num_t>(green), static_cast<gpio_num_t>(blue))
{
}

/**
 * @brief Class destructor
 */
RGB::~RGB()
{
}

/**
 * @brief Switch led on
 */
Color RGB::SwitchOn()
{
  SetColor(mColor);
  return mColor;
}

/**
 * @brief Switch led on with setting color
 */
Color RGB::SwitchOn(Color color)
{
  if (color == Color::BLACK)
    return mColor;

  mColor = color;
  return SwitchOn();
}

/**
 * @brief Switch led off
 */
void RGB::SwitchOff() const
{
  SetColor(Color::BLACK);
}

/**
 * @brief Blick with led diode
 */
void RGB::Blick(uint32_t timeout)
{
  Blick(mColor, timeout);
}

/**
 * @brief Blick with led diode with specific color
 */
void RGB::Blick(Color color, uint32_t timeout)
{
  SwitchOn(color);

  vTaskDelay(timeout);

  SwitchOff();

  vTaskDelay(timeout);
}

/**
 * @brief Multiple blick with led diode
 */
void RGB::MultipleBlick(uint8_t amount, uint32_t timeout)
{
  MultipleBlick(mColor, amount, timeout);
}

/**
 * @brief Multiple blick with led diode with specific color
 */
void RGB::MultipleBlick(Color color, uint8_t amount, uint32_t timeout)
{
  for (uint8_t i = 0; i < amount; ++i)
  {
    Blick(color, timeout);
  }
}

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Set pins to specific color
 */
void RGB::SetColor(Color color) const
{
  switch (color)
  {
  case (Color::WHITE):
  {
    SetColor(1, 1, 1);
    break;
  }
  case (Color::RED):
  {
    SetColor(1, 0, 0);
    break;
  }
  case (Color::GREEN):
  {
    SetColor(0, 1, 0);
    break;
  }
  case (Color::BLUE):
  {
    SetColor(0, 0, 1);
    break;
  }
  case (Color::YELLOW):
  {
    SetColor(1, 1, 0);
    break;
  }
  case (Color::PURPLE):
  {
    SetColor(1, 0, 1);
    break;
  }
  case (Color::BLUE_LIGHT):
  {
    SetColor(0, 1, 1);
    break;
  }
  default:
  {
    SetColor(0, 0, 0);
    break;
  }
  }
}

/**
 * @brief Set pins to specific color
 */
void RGB::SetColor(uint8_t red, uint8_t green, uint8_t blue) const
{
  // Red pin
  SetPin(mRed, red);

  // Green pin
  SetPin(mGreen, green);

  // Blue pin
  SetPin(mBlue, blue);
}

/**
 * @brief Set pin to defined level
 */
void RGB::SetPin(gpio_num_t pin, uint8_t level) const
{
  gpio_set_level(pin, level);
}
