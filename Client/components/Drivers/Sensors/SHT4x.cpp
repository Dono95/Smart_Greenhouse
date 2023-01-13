#include "SHT4x.hpp"

/* ESP log library */
#include "esp_log.h"

/* STD library */
#include <cmath>

using namespace Sensor;

/**
 * @brief Class constructor
 */
SHT4x::SHT4x(uint8_t i2C_address, Component::Driver::Communication::I2C *i2c)
    : mI2C_address(i2C_address), mI2C(i2c)
{
}

/**
 * @brief Class destructor
 */
SHT4x::~SHT4x()
{
}

#ifdef CONFIG_HUMANITY
/**
 * @brief Get last measured humanity
 */
uint8_t SHT4x::GetHumanity() const
{
    return mHumanity;
}
#endif

#ifdef CONFIG_TEMPERATURE
/**
 * @brief Get last measured temperature
 */
float SHT4x::GetTemperature() const
{
    return mTemperature;
}
#endif

/**
 * @brief Measure temperature and humanity with given precision
 */
void SHT4x::Measure(MeasurePrecision precision)
{
    // Write command to start the measurement
    auto i2c_result = mI2C->Write(mI2C_address, static_cast<uint8_t>(precision));
    if (i2c_result != I2C_OperationResult::WRITE_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the written command: %s", I2C_RESULT_TO_STRING(i2c_result));
        return;
    }

    // Wait predefined time for operation complete
    vTaskDelay(precisionTimeouts.at(static_cast<uint8_t>(precision)));

    // Read measurement values
    std::vector<uint8_t> sensorData;
    i2c_result = mI2C->Read(mI2C_address, sensorData, 6);

    if (i2c_result != I2C_OperationResult::READ_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the reading data: %s", I2C_RESULT_TO_STRING(i2c_result));
        return;
    }

    ConversionSignalOutput(sensorData);
}

/**
 * @brief Get serial number of the sensor
 */
std::string SHT4x::SerialNumber() const
{
    auto i2c_result = mI2C->Write(mI2C_address, READ_SERIAL);

    if (i2c_result != I2C_OperationResult::WRITE_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the written command: %s", I2C_RESULT_TO_STRING(i2c_result));
        return {};
    }

    vTaskDelay(1);

    std::vector<uint8_t> data;
    i2c_result = mI2C->Read(mI2C_address, data, 6);

    if (i2c_result != I2C_OperationResult::READ_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the reading data: %s", I2C_RESULT_TO_STRING(i2c_result));
        return {};
    }

    return std::to_string(data.at(0)) + " " + std::to_string(data.at(1)) + " " + std::to_string(data.at(3)) + " " + std::to_string(data.at(4));
}

/**
 * @brief Software reset
 */
void SHT4x::SoftReset() const
{
    auto i2c_result = mI2C->Write(mI2C_address, SOFT_RESET);

    if (i2c_result != I2C_OperationResult::WRITE_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the written command: %s", I2C_RESULT_TO_STRING(i2c_result));
    }
    vTaskDelay(1);
}

/**
 * @brief Output signal conversion
 */
void SHT4x::ConversionSignalOutput(const std::vector<uint8_t> sensorData)
{
#ifdef CONFIG_TEMPERATURE
    int16_t temperatureSensorData = (sensorData.at(0) << 8) | sensorData.at(1);
    mTemperature = (-45 + 175 * (temperatureSensorData / (pow(2, 16) - 1)));
#endif

#ifdef CONFIG_HUMANITY
    int16_t humanintySensorData = (sensorData.at(3) << 8) | sensorData.at(4);
    mHumanity = (-6 + 125 * (humanintySensorData / (pow(2, 16) - 1)));
#endif
}