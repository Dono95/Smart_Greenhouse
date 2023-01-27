/* Project specific includes */
#include "Sensor.hpp"

/* C library*/
#include <cfloat>

/* ESP log library */
#include "esp_log.h"

/* Common components */
#include "Common_components/Convertors/DataType/ByteConvertor.hpp"

using namespace Sensor;

/**
 * @brief Class constructor
 */
Sensor_I::Sensor_I(uint8_t i2c_address, I2C *i2c)
    : mI2C_address(i2c_address), mI2C(i2c)
{
}

/**
 * @brief Class destructor
 */
Sensor_I::~Sensor_I()
{
}

/**
 * @brief Get I2C address
 *
 * @return uint8_t
 */
uint8_t Sensor_I::GetI2C_Address() const
{
    return mI2C_address;
}

/**
 * @brief Get I2C driver
 *
 * @return Pointer to I2C driver
 */
I2C *Sensor_I::GetI2C_Driver() const
{
    return mI2C;
}

/**
 * @brief Method to send command to sensor
 */
I2C_OperationResult Sensor_I::SendCommand(const uint32_t command, const uint32_t timeout, const uint8_t command_length) const
{
    auto i2c_result = mI2C->Write(mI2C_address, command, command_length);
    if (i2c_result != I2C_OperationResult::WRITE_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SENSOR_TAG, "Failed to send command[0x%x]: %s", command, I2C_RESULT_TO_STRING(i2c_result));
        return i2c_result;
    }

    vTaskDelay(timeout);

    return I2C_OperationResult::I2C_OK;
}

/**
 * @brief Method to send command to sensor with expecting sensor response
 */
I2C_OperationResult Sensor_I::SendCommand(const uint32_t command, const uint32_t timeout, const uint8_t command_length,
                                          std::vector<uint8_t> &sensor_response, const uint8_t expected_data_length) const
{
    // Write to sensor specific command and if expected_data_length is 0 -> return from method
    auto i2c_result = SendCommand(command, timeout, command_length);
    if (i2c_result != I2C_OperationResult::I2C_OK || !expected_data_length)
        return i2c_result;

    // Reservce space in data structure with expected data length
    sensor_response.reserve(expected_data_length);

    // Read sensor response
    i2c_result = mI2C->Read(mI2C_address, sensor_response, expected_data_length);
    if (i2c_result != I2C_OperationResult::READ_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SENSOR_TAG, "Failed to read sensor response: %s", I2C_RESULT_TO_STRING(i2c_result));
        return i2c_result;
    }

    return I2C_OperationResult::I2C_OK;
}

/**
 * @brief Method to calculate checksum CRC8
 */
uint8_t Sensor_I::CalculateChecksum_CRC_8() const
{
    auto convertor = new Component::Convertor::ByteConvertor();

    auto bytes = convertor->ConvertTo<uint8_t>(0xBEEF, 1);

    for (const auto &byte : bytes)
    {
        printf("Value: %ld\n", static_cast<long>(byte));
    }

    return 0;
}

/**
 * @brief Class constructor
 */
AirSensor_I::AirSensor_I(uint8_t i2c_address, I2C *i2c)
    : Sensor_I(i2c_address, i2c), mTemperature(nullptr), mHumanity(nullptr), mCO2(nullptr)
{
}

/**
 * @brief Class destructor
 */
AirSensor_I::~AirSensor_I()
{
}

/**
 * @brief Get temperature
 */
float AirSensor_I::GetTemperature() const
{
    return FLT_MAX;
}

/**
 * @brief Get humanity
 */
float AirSensor_I::GetHumanity() const
{
    return FLT_MAX;
}

/**
 * @brief Get CO2
 */
uint16_t AirSensor_I::GetCO2() const
{
    return UINT16_MAX;
}