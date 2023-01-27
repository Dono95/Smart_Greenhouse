/* Project specific includes */
#include "SHT4x.hpp"

/* ESP log library */
#include "esp_log.h"

/* STD library */
#include <cmath>

using namespace Sensor;

/**
 * @brief Class constructor
 */
SHT4x::SHT4x(uint8_t i2C_address, I2C *i2c)
    : AirSensor_I(i2C_address, i2c)
{
    // Sensor need 1000 ms to enter the idle state
    vTaskDelay(1000);

    mTemperature = new float();
    mHumanity = new float();
}

/**
 * @brief Class destructor
 */
SHT4x::~SHT4x()
{
    if (mTemperature)
        delete mTemperature;
    if (mHumanity)
        delete mHumanity;
}

/**
 * @brief Get temperature
 */
float SHT4x::GetTemperature() const
{
    return *mTemperature;
}

/**
 * @brief Get humanity
 */
float SHT4x::GetHumanity() const
{
    return *mHumanity;
}

/**
 * @brief Get serial number of the sensor
 */
std::string SHT4x::SerialNumber() const
{
    std::vector<uint8_t> serial_number;

    if (SendCommand(READ_SERIAL, 1, 1, serial_number, 6) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SHT4x_TAG, "Failed to get sensor serial number");
        return std::string();
    }

    return std::to_string(serial_number.at(0)) + " " + std::to_string(serial_number.at(1)) + " " + std::to_string(serial_number.at(3)) + " " + std::to_string(serial_number.at(4));
}

/**
 * @brief Trigger measurement on sensor (Single shot)
 */
void SHT4x::Measure()
{
    std::vector<uint8_t> sensorData;
    if (SendCommand(MEASURE_T_RH_HIGH_PRECISION, 10, 1, sensorData, 6) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SHT4x_TAG, "Measurement failed");
        return;
    }

    CalculateAirValues(sensorData);
}

/**
 * @brief Software reset
 */
void SHT4x::SoftReset() const
{
    if (SendCommand(SOFT_RESET, 1) != I2C_OperationResult::I2C_OK)
        ESP_LOGE(SHT4x_TAG, "Software reset of the sensor can not be achieved");
}

/**
 * @brief Activate highest heater power for 1s
 */
void SHT4x::ActivateHeaterPower() const
{
    if (SendCommand(ACTIVE_HEATER_POWER_1S, 1200) != I2C_OperationResult::I2C_OK)
        ESP_LOGE(SHT4x_TAG, "Activation heater power for 1s failed");
}

/**
 * @brief Calculate values
 */
void SHT4x::CalculateAirValues(const std::vector<uint8_t> sensorData)
{
    if (sensorData.empty() || sensorData.size() < 6)
        return;

    int16_t temperatureSensorData = (sensorData.at(0) << 8) | sensorData.at(1);
    *mTemperature = (-45 + 175 * (temperatureSensorData / (pow(2, 16) - 1)));

    int16_t humanintySensorData = (sensorData.at(3) << 8) | sensorData.at(4);
    *mHumanity = (-6 + 125 * (humanintySensorData / (pow(2, 16) - 1)));
}