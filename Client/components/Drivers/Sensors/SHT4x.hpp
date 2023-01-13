#ifndef SHT4x_H
#define SHT4x_H

/* Project specific includes */
#include "Commands_Definition.hpp"

/* Common components */
#include "Common_components/Drivers/Communication/I2C.hpp"

/* STD library */
#include <string>

/* SDK */
#include "sdkconfig.h"

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

#ifdef CONFIG_HUMANITY
        /**
         * @brief Get last measured humanity
         *
         * @return uint8_t
         */
        uint8_t GetHumanity() const;
#endif

#ifdef CONFIG_TEMPERATURE
        /**
         * @brief Get last measured temperature
         *
         * @return float
         */
        float GetTemperature() const;
#endif

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

#ifdef CONFIG_HUMANITY
        // Humanity
        uint8_t mHumanity;
#endif

#ifdef CONFIG_TEMPERATURE
        // Temperature
        float mTemperature;
#endif
    };
} // namespace Sensor

#endif /* SHT4x_H */