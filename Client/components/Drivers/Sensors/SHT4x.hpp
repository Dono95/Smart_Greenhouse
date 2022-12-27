#ifndef SHT4x_H
#define SHT4x_H

/* Project specific includes */
#include "Commands_Definition.hpp"

/* Common components */
#include "Common_components/Drivers/Communication/I2C.hpp"

/* STD library */
#include <string>

#define SHT4x_TAG "SHT4x_Sensor"

namespace Sensor
{
    class SHT4x
    {
    public:
        enum class MeasurePrecision
        {
            HIGH = MEASURE_T_RH_HIGH_PRECISION,
            MEDIUM = MEASURE_T_RH_MEDIUM_PRECISION,
            LOW = MEASURE_T_RH_LOW_PRECISION
        };

        /**
         * @brief Class constructor
         */
        explicit SHT4x(uint8_t i2C_address, Component::Driver::Communication::I2C *i2c);

        /**
         * @brief Class destructor
         */
        ~SHT4x();

        /**
         * @brieg Get last measured humanity
         *
         * @return uint8_t
         */
        uint8_t GetHumanity() const;

        /**
         * @brief Get last measured temperature
         *
         * @return float
         */
        float GetTemperature() const;

        /**
         * @brief Measure temperature and humanity with given precision
         *
         * @param[in] precision : Recuired measurement accurancy
         */
        void Measure(MeasurePrecision precision);

        /**
         * @brief Get serial number of the sensor
         *
         * @return std::string  : The serial number of the sensor in string format.
         *                        Empty string if the serial number can be obtain from the sensor
         */
        std::string SerialNumber() const;

        /**
         * @brief Software reset
         */
        void SoftReset() const;

    private:
        // Type alias -> The methods results of I2C interface class
        using I2C_OperationResult = Component::Driver::Communication::I2C_Result;

        /**
         * @brief Output signal conversion
         *
         * @param[in] sensorData : The sensor outpus signals
         */
        void ConversionSignalOutput(const std::vector<uint8_t> sensorData);

        // I2C slave address of sensor
        uint8_t mI2C_address{0};

        // Pointer to I2C driver
        Component::Driver::Communication::I2C *mI2C{nullptr};

        // Humanity
        uint8_t mHumanity;

        // Temperature
        float mTemperature;
    };
} // namespace Sensor

/*#include "Drivers/Com_Interfaces/I2C_I.hpp"
#include "Commands_Definition.hpp"

#include <cstdint>
#include <string>

static const char *SHT4x_TAG = "SHT4x_Sensor";

namespace Sensor
{
    class SHT4x
    {
        using i2c_result_value = Interface::I2C_Result;

    public:


        SHT4x(uint8_t address);
        ~SHT4x();

        void TriggerMeasure(MeasurePrecision precision);
        const std::string SerialNumber(void) const;
        void SoftReset() const;

        uint8_t Humanity() const;
        float Temperature() const;

    private:
        const std::string FormatSerialNumber(const std::vector<uint8_t> &data) const;
        void CalculateValues(const std::vector<uint8_t> &data);

        uint8_t mI2C_Address;
        Interface::I2C_I *mI2C_I{nullptr};

        // Values from the last measurement
        uint8_t mHumanity;
        float mTemperature;
    };
} // namespace Interface*/

#endif /* SHT4x_H */