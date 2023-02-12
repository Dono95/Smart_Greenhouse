#ifndef WATER_PUMP_H
#define WATER_PUMP_H

/* ESP library */
#include "driver/gpio.h"

namespace Component
{
  namespace Driver
  {
    namespace Active
    {
      class WaterPump
      {
      public:
        /**
         * @brief Class constructor
         *
         * @param[in] pump : Pin to water pump
         */
        explicit WaterPump(gpio_num_t pump);

        /**
         * @brief Class constructor
         *
         * @param[in] pump : Pin to water pump
         */
        explicit WaterPump(int pump);

        /**
         * @brief Class destructor
         */
        ~WaterPump();

        /**
         * @brief Turn water pump on
         */
        void TurnOn();

        /**
         * @brief Turn wate rpump off
         */
        void TurnOff();

      private:
        /* Water pump gpio pin */
        gpio_num_t mWaterPump;
      };
    } // namespace Active

  } // namespace Driver
} // namespace Component

#endif