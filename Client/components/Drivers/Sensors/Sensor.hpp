#ifndef SENSOR_H
#define SENSOR_H

/* STD library */
#include <cstdint>
#include <string>

/* Common components */
#include <Common_components/Drivers/Communication/I2C.hpp>

#define SENSOR_TAG "Sensor"

namespace Sensor
{
    /* Type alias -> I2C communication driver */
    using I2C = Component::Driver::Communication::I2C;

    //  Type alias -> The methods results of I2C interface class
    using I2C_OperationResult = Component::Driver::Communication::I2C_Result;

    class Sensor_I
    {
    protected:
        /**
         * @brief Class constructor
         */
        explicit Sensor_I(uint8_t i2c_address, I2C *i2c);

        /**
         * @brief Class destructor
         */
        virtual ~Sensor_I();

        /**
         * @brief Get I2C address
         *
         * @return uint8_t
         */
        uint8_t GetI2C_Address() const;

        /**
         * @brief Get I2C driver
         *
         * @return Pointer to I2C driver
         */
        I2C *GetI2C_Driver() const;

        /**
         * @brief Method to send command to sensor
         *
         * @param[in] command           : Sensor commmand
         * @param[in] timeout           : Timeout to complete sensor commmand
         * @param[in] command_length    : Command length in bytes (Default is 1 byte)
         */
        virtual I2C_OperationResult SendCommand(const uint32_t command, const uint32_t timeout, const uint8_t command_length = 1) const;

        /**
         * @brief Method to send command to sensor with expecting sensor response
         *
         * @param[in]  command              : Sensor command
         * @param[in]  timeout              : Timeout to complete sensor command and start to read sensor response
         * @param[in]  command_length       : Command length in bytes
         * @param[out] sensor_response      : Sensor response data
         * @param[in]  expected_data_length : Expected length of sensor data response
         */
        virtual I2C_OperationResult SendCommand(const uint32_t command, const uint32_t timeout, const uint8_t command_length,
                                                std::vector<uint8_t> &sensor_response, const uint8_t expected_data_length) const;

        /**
         * @brief Method to calculate checksum CRC8
         *
         * @return uint8_t
         */
        virtual uint8_t CalculateChecksum_CRC_8() const;

    public:
        /**
         * @brief Get serial number of the sensor
         *
         * @return std::string  : The serial number of the sensor in string format.
         *                        Empty string if the serial number can be obtain from the sensor
         */
        virtual std::string SerialNumber() const = 0;

        /**
         * @brief Trigger measurement on sensor (Single shot)
         */
        virtual void Measure() = 0;

        /**
         * @brief Software reset
         */
        virtual void SoftReset() const = 0;

    private:
        /* I2C slave address */
        uint8_t mI2C_address;

        /* I2C driver */
        I2C *mI2C;
    };

    class AirSensor_I : public Sensor_I
    {
    protected:
        /**
         * @brief Class constructor
         */
        explicit AirSensor_I(uint8_t i2c_address, I2C *i2c);

        /**
         * @brief Class destructor
         */
        virtual ~AirSensor_I() override;

    public:
        /**
         * @brief Get temperature
         *
         * @return float
         */
        virtual float GetTemperature() const;

        /**
         * @brief Get humanity
         *
         * @return float
         */
        virtual float GetHumanity() const;

        /**
         * @brief Get CO2
         *
         * @return uint16_t
         */
        virtual uint16_t GetCO2() const;

    protected:
        // Temperature
        float *mTemperature;

        // Humanity
        float *mHumanity;

        // CO2
        uint16_t *mCO2;
    };
}

#endif