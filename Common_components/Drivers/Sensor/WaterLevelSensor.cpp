#include "WaterLevelSensor.hpp"

using namespace Component::Driver::Sensor;

/**
 * @brief Class constructor
 */
WaterLevelSensor::WaterLevelSensor(uint8_t i2c_address, Component::Driver::Communication::I2C *i2c)
    : mWaterLevel(0u), mI2C_Address(i2c_address), mI2C(i2c)
{
}

/**
 * @brief Class destructor
 */
WaterLevelSensor::~WaterLevelSensor()
{
}

/**
 * @brief Measure water level
 */
uint8_t WaterLevelSensor::Measure()
{
  return 0;
}

/**
 * @brief Get last measured water level
 */
uint8_t WaterLevelSensor::WaterLevel() const
{
  return mWaterLevel;
}