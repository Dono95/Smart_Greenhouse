#ifndef RGB_H
#define RGB_H

/* STD library */
#include "stdint.h"

/* ESP library */
#include "driver/gpio.h"

#define TIMEOUT 500

namespace Utility
{
  namespace Indicator
  {
    enum class Color
    {
      BLACK,
      WHITE,
      RED,
      GREEN,
      BLUE,
      YELLOW,
      PURPLE,
      BLUE_LIGHT
    };

    class RGB
    {
    public:
      /**
       * @brief Class destructor
       *
       * @param red   : Red color pin
       * @param green : Green color pin
       * @param blue  : Blue color pin
       */
      explicit RGB(gpio_num_t red, gpio_num_t green, gpio_num_t blue);
      /**
       * @brief Class constructor
       *
       * @param red   : Red color pin
       * @param green : Green color pin
       * @param blue  : Blue color pin
       */
      explicit RGB(uint8_t red, uint8_t green, uint8_t blue);

      /**
       * @brief Class destructor
       */
      ~RGB();

      /**
       * @brief Switch led on
       *
       * @return Color  : Actual set color
       */
      Color SwitchOn();

      /**
       * @brief Switch led on with setting color
       *
       * @param[in] color : Color to set
       *
       * @return Color  : Actual set color
       */
      Color SwitchOn(Color color);

      /**
       * @brief Switch led off
       */
      void SwitchOff() const;

      /**
       * @brief Blick with led diode
       * @param[in] timeout : Timeout between flashes
       */
      void Blick(uint32_t timeout = TIMEOUT);

      /**
       * @brief Blick with led diode with specific color
       *
       * @param[in] color : Color to set
       * @param[in] timeout : Timeout between flashes
       */
      void Blick(Color color, uint32_t timeout = TIMEOUT);

      /**
       * @brief Multiple blick with led diode
       *
       * @param[in] amount  : Number of flashes
       * @param[in] timeout : Timeout between flashes
       */
      void MultipleBlick(uint8_t amount, uint32_t timeout = TIMEOUT);

      /**
       * @brief Multiple blick with led diode with specific color
       *
       * @param[in] color  : Color to set
       * @param[in] amount : Number of flashes
       * @param[in] amount : Timeout between flashes
       */
      void MultipleBlick(Color color, uint8_t amount, uint32_t timeout = TIMEOUT);

    private:
      /**
       * @brief Set pins to specific color
       *
       * @param[in] color   : Specific color
       */
      void SetColor(Color color) const;

      /**
       * @brief Set pins to specific color
       *
       * @param[in] red     : Red part
       * @param[in] green   : Green part
       * @param[in] blue    : Blue part
       */
      void SetColor(uint8_t red, uint8_t green, uint8_t blue) const;

      /**
       * @brief Set pin to defined level
       *
       * @param[in] pin
       * @param[in] level
       */
      void SetPin(gpio_num_t pin, uint8_t level) const;

      /* Red pin */
      gpio_num_t mRed;

      /* Green pin */
      gpio_num_t mGreen;

      /* Blue pin */
      gpio_num_t mBlue;

      /* Actual RGB led color except for Black color */
      Color mColor;
    };
  } // namespace Indicator
} // namespace Component

#endif