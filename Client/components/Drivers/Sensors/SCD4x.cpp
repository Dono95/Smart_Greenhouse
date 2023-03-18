/* Project specific includes */
#include "SCD4x.hpp"
#include "Commands_Definition.hpp"

/* ESP log library */
#include "esp_log.h"

/* STD library */
#include <cmath>

#define PERFORMING_INTERVAL 60000 * 3

using namespace Sensor;

/**
 * @brief Class constructor
 */
SCD4x::SCD4x(uint8_t i2C_address, I2C *i2c)
    : AirSensor_I(i2C_address, i2c)
{
    // Sensor need 1000 ms to enter the idle state
    vTaskDelay(1000);

    mTemperature = new float();
    mHumanity = new float();
    mCO2 = new uint16_t();
}

/**
 * @brief Class destructor
 */
SCD4x::~SCD4x()
{
    if (mTemperature)
        delete mTemperature;
    if (mHumanity)
        delete mHumanity;
    if (mCO2)
        delete mCO2;
}

/**
 * @brief Get serial number of the sensor
 */
std::string SCD4x::SerialNumber() const
{
    std::vector<uint8_t> serial_number;
    if (SendCommand(GET_SERIAL_NUMBER, 1, 2, serial_number, 9) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SCD4x_TAG, "Failed to get sensor serial number");
        return std::string();
    }

    return FormatSerialNumber(serial_number);
}

/**
 * @brief Get temperature
 */
float SCD4x::GetTemperature() const
{
    return *mTemperature;
}

/**
 * @brief Get humanity
 */
float SCD4x::GetHumanity() const
{
    return *mHumanity;
}

/**
 * @brief Get CO2
 */
uint16_t SCD4x::GetCO2() const
{
    return *mCO2;
}

/**
 * @brief Start periodic measurement
 */
void SCD4x::StartPeriodicMeasurement(const bool enable_low_power) const
{
    auto command = enable_low_power ? START_LOW_POWER_PERIODIC_MEASUREMENT : START_PERIODIC_MEASUREMENT;

    if (SendCommand(command, 0, 2) != I2C_OperationResult::I2C_OK)
        ESP_LOGE(SCD4x_TAG, "Unable to start period measurement");
}

/**
 * @brief Stop periodic measurement
 */
void SCD4x::StopPeriodicMeasurement() const
{
    if (SendCommand(STOP_PERIODIC_MEASUREMENT, 500, 2) != I2C_OperationResult::I2C_OK)
        ESP_LOGE(SCD4x_TAG, "Unable to stop period measurement");
}

/**
 * @brief Trigger measurement on sensor (Single shot)
 */
void SCD4x::Measure()
{
    // Send command to measure air values
    if (SendCommand(MEASURE_SINGLE_SHOT, 5000, 2) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SCD4x_TAG, "Measurement failed");
        return;
    }

    std::vector<uint8_t> measured_values;
    // Send command to read measured values
    if (SendCommand(READ_MEASUREMENT, 1, 2, measured_values, 9) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SCD4x_TAG, "Failed to read measured values");
        return;
    }

    CalculateAirValues(measured_values);
}

/**
 * @brief Software reset
 */
void SCD4x::SoftReset() const
{
    if (SendCommand(REINIT, 1000, 2) != I2C_OperationResult::I2C_OK)
        ESP_LOGE(SCD4x_TAG, "Software reset of the sensor can not be achieved");
}

/**
 * @brief Perform forced recalibration
 */
void SCD4x::PerformForcedRecalibration()
{
    // Step 1 -> Start periodic measurement more than 3 minutes
    StartPeriodicMeasurement(true);
    vTaskDelay(PERFORMING_INTERVAL);

    // Step 2 -> Stop periodic measurement
    StopPeriodicMeasurement();
    vTaskDelay(500);

    // Send command with parameter to force recalibration
    std::vector<uint8_t> response;
    if (SendCommand(PERFORM_FORCED_RECALIBRATION, {0x01, 0xe0, 0xb4}, 400, 2, response, 3) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGI(SCD4x_TAG, "Forcing recalibration failed");
        return;
    }

    for (const auto &byte : response)
        printf("0x%x\n", byte);
}

/**
 * @brief Check if automatic self calibration is enabled
 */
bool SCD4x::IsAutomaticSelfCalibrationEnabled() const
{
    std::vector<uint8_t> response;
    if (SendCommand(GET_AUTOMATIC_SELF_CALIBRATION_ENABLED, 1, 2, response, 3) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SCD4x_TAG, "Failed to get sensor serial number");
        return false;
    }

    if (response.empty() && response.size() == 3)
        return false;

    for (const auto &byte : response)
        printf("0x%x\n", byte);

    printf("\n");
    return response.at(1);
}

/**
 * @brief Enable automatic self calibration
 */
void SCD4x::EnableAutomaticSelfCalibration() const
{
    // Send command with parameter
    if (SendCommand(SET_AUTOMATIC_SELF_CALIBRATION_ENABLED, {0x01, 0xB0}, 1, 2) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGI(SCD4x_TAG, "Enabling self automatic calibration failed");
        return;
    }

    ESP_LOGI(SCD4x_TAG, "Self automatic calibration is enabled");
    SavePersistSettings();
}

/**
 * @brief Disable automatic self calibration
 */
void SCD4x::DisableAutomaticSelfCalibration() const
{
    // Send command with parameter
    if (SendCommand(SET_AUTOMATIC_SELF_CALIBRATION_ENABLED, {0x00, 0x81}, 1, 2) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGI(SCD4x_TAG, "Enabling self automatic calibration failed");
        return;
    }

    ESP_LOGI(SCD4x_TAG, "Self automatic calibration is disabled");
    SavePersistSettings();
}

/**
 * @brief Get sensor altitude
 */
uint16_t SCD4x::GetSensorAltitude() const
{
    std::vector<uint8_t> response;
    // Send command to read measured values
    if (SendCommand(GET_SENSOR_ALTITUDE, 1, 2, response, 3) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SCD4x_TAG, "Failed to read sensor altitude");
        return 0;
    }

    if (response.empty() || response.size() != 3)
        return 0;

    uint16_t altitude = response.at(0);
    altitude <<= 8;
    altitude += response.at(1);

    return altitude;
}

/**
 * @brief Set sensor altitude
 */
void SCD4x::SetSensorAltitude(const uint16_t sensorAltitude) const
{
    // Send command with parameter
    if (SendCommand(SET_SENSOR_ALTITUDE, {0x02, 0xAC, 0xD9}, 1, 2) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SCD4x_TAG, "Setting sensor altitude failed");
        return;
    }

    // Save persist settings
    SavePersistSettings();
}

/**
 * @brief Get temperature offset
 */
uint16_t SCD4x::GetTemperatureOffset() const
{
    std::vector<uint8_t> response;
    // Send command to read measured values
    if (SendCommand(GET_TEMPERATURE_OFFSET, 1, 2, response, 3) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SCD4x_TAG, "Failed to read temperature offset");
        return 0;
    }

    if (response.empty() || response.size() != 3)
        return 0;

    uint16_t offset = response.at(0);
    offset <<= 8;
    offset += response.at(1);

    return (175 * offset) / pow(2, 16);
}

/**
 * @brief Set temperature offset
 */
void SCD4x::SetTemperatureOffset(const uint16_t sensorAltitude) const
{
    // Send command with parameter
    if (SendCommand(SET_TEMPERATURE_OFFSET, {0x00, 0x00, 0x81}, 1, 2) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGI(SCD4x_TAG, "Setting temperature offset failed");
        return;
    }

    SavePersistSettings();
}

/**
 * @brief Calculate values
 */
void SCD4x::CalculateAirValues(const std::vector<uint8_t> &sensorData)
{
    if (sensorData.empty() || sensorData.size() < 9)
        return;

    // CO2
    *mCO2 = (sensorData.at(0) << 8) | sensorData.at(1);

    // Temperature
    uint16_t temp = (sensorData.at(3) << 8) | sensorData.at(4);
    *mTemperature = -45 + 175 * temp / pow(2, 16);

    // Humanity
    uint16_t hum = (sensorData.at(6) << 8) | sensorData.at(7);
    *mHumanity = 100 * hum / pow(2, 16);
}

/**
 * @brief Format serial number
 */
std::string SCD4x::FormatSerialNumber(const std::vector<uint8_t> &sensorData) const
{
    if (sensorData.empty() || sensorData.size() < 9)
        return std::string("\"Data corrupted\"");

    uint8_t crc_index{0};
    std::string serial_number;

    for (const auto &value : sensorData)
    {
        if (crc_index++ >= 2)
        {
            crc_index = 0;
            // CRC 8
            continue;
        }

        serial_number.append(std::to_string(value) + " ");
    }

    // If serial number string is not empty remove last space
    if (!serial_number.empty())
        serial_number.pop_back();

    return serial_number;
}

/**
 * @brief Save persist setttings
 */
void SCD4x::SavePersistSettings() const
{
    // Send command to save settings
    if (SendCommand(PERSIST_SETTINGS, 800, 2) != I2C_OperationResult::I2C_OK)
    {
        ESP_LOGE(SCD4x_TAG, "Saving settings failed");
        return;
    }

    ESP_LOGI(SCD4x_TAG, "Saving persist settings have been successful");
}
