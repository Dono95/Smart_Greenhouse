#ifndef WATER_LEVEL_SENSOR
#define WATER_LEVEL_SENSOR

/* STD library */
#include <stdint.h>

/* Communication */
#include "I2C.hpp"

namespace Component
{
  namespace Driver
  {
    namespace Sensor
    {
      class WaterLevelSensor
      {
      public:
        /**
         * @brief Class constructor
         */
        explicit WaterLevelSensor(uint8_t i2c_address, Component::Driver::Communication::I2C *i2c) __attribute__((nonnull));

        /**
         * @brief Class destructor
         */
        ~WaterLevelSensor();

        /**
         * @brief Measure water level
         *
         * @return uint8_t  : Measured water level
         */
        uint8_t Measure();

        /**
         * @brief Get last measured water level
         *
         * @return uint8_t  : Water level
         */
        uint8_t WaterLevel() const;

      private:
        /* Water level */
        uint8_t mWaterLevel;

        /* I2C address */
        uint8_t mI2C_Address;

        /* I2C Driver*/
        Component::Driver::Communication::I2C *mI2C;
      };
    } // namespace Sensor

  } // namespace Driver
} // namespace Component

#endif