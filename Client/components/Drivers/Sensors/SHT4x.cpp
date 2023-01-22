#include "SHT4x.hpp"

/* ESP log library */
#include "esp_log.h"

/* STD library */
#include <cmath>

using namespace Sensor;

/**
 * @brief Class constructor
 */
SHT4x::SHT4x(uint8_t i2C_address, I2C *i2c) : Sensor_I(i2C_address, i2c)
{
}

/**
 * @brief Class destructor
 */
SHT4x::~SHT4x()
{
}

/**
 * @brief Get serial number of the sensor
 */
std::string SHT4x::SerialNumber() const
{
    // Get I2C driver
    auto i2c = GetI2C_Driver();

    auto i2c_result = i2c->Write(GetI2C_Address(), READ_SERIAL);

    if (i2c_result != I2C_OperationResult::WRITE_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the written command: %s", I2C_RESULT_TO_STRING(i2c_result));
        return {};
    }

    vTaskDelay(1);

    std::vector<uint8_t> data;
    i2c_result = i2c->Read(GetI2C_Address(), data, 6);

    if (i2c_result != I2C_OperationResult::READ_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the reading data: %s", I2C_RESULT_TO_STRING(i2c_result));
        return {};
    }

    return std::to_string(data.at(0)) + " " + std::to_string(data.at(1)) + " " + std::to_string(data.at(3)) + " " + std::to_string(data.at(4));
}

/**
 * @brief Get last measured humanity
 */
/*uint8_t SHT4x::GetHumanity() const
{
    return mHumanity;
}*/

#ifdef CONFIG_TEMPERATURE
/**
 * @brief Get last measured temperature
 */
/*float SHT4x::GetTemperature() const
{
    return mTemperature;
}*/

/**
 * @brief Measure temperature and humanity with given precision
 */
/*void SHT4x::Measure(MeasurePrecision precision)
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
}*/

/**
 * @brief Software reset
 */
/*void SHT4x::SoftReset() const
{
    auto i2c_result = mI2C->Write(mI2C_address, SOFT_RESET);

    if (i2c_result != I2C_OperationResult::WRITE_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the written command: %s", I2C_RESULT_TO_STRING(i2c_result));
    }
    vTaskDelay(1);
}*/

/**
 * @brief Output signal conversion
 */
/*void SHT4x::ConversionSignalOutput(const std::vector<uint8_t> sensorData)
{
#ifdef CONFIG_TEMPERATURE
    int16_t temperatureSensorData = (sensorData.at(0) << 8) | sensorData.at(1);
    mTemperature = (-45 + 175 * (temperatureSensorData / (pow(2, 16) - 1)));
#endif

#ifdef CONFIG_HUMANITY
    int16_t humanintySensorData = (sensorData.at(3) << 8) | sensorData.at(4);
    mHumanity = (-6 + 125 * (humanintySensorData / (pow(2, 16) - 1)));
}*/

/*#include "esp_log.h"
#include <cmath>


SHT4x::SHT4x(uint8_t address) : mI2C_Address(address)
{
    mI2C_I = Interface::I2C_I::GetInstance();
}

SHT4x::~SHT4x()
{
}

void SHT4x::TriggerMeasure(MeasurePrecision precision)
{
    auto i2c_result = mI2C_I->WriteByte(mI2C_Address, static_cast<uint8_t>(precision));
    if (i2c_result != i2c_result_value::WRITE_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the written command: %s", Interface::EnumToString(i2c_result));
        return;
    }

    // See datasheet https://www.mouser.com/datasheet/2/682/Datasheet_SHT4x-1917879.pdf Page 7
    switch (precision)
    {
    case (MeasurePrecision::HIGH):
        vTaskDelay(10);
        break;
    case (MeasurePrecision::MEDIUM):
        vTaskDelay(5.5);
        break;
    case (MeasurePrecision::LOW):
        vTaskDelay(2.5);
        break;
    default:
        vTaskDelay(10);
    }

    std::vector<uint8_t> sensorData;
    i2c_result = mI2C_I->ReadData(mI2C_Address, sensorData, 6);

    if (i2c_result != i2c_result_value::READ_DATA_SUCCESSFUL)
    {
        ESP_LOGE(SHT4x_TAG, "The result of the reading data: %s", Interface::EnumToString(i2c_result));
        return;
    }

    CalculateValues(sensorData);
}