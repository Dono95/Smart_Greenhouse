#ifndef SHT4x_H
#define SHT4x_H

/* Interface */
#include "Sensor.hpp"

/* Project specific includes */
#include "Commands_Definition.hpp"

#define SHT4x_TAG "SHT4x_Sensor"

namespace Sensor
{
    class SHT4x : public AirSensor_I
    {
    public:
        /**
         * @brief Class constructor
         */
        explicit SHT4x(uint8_t i2C_address, I2C *i2c);

        /**
         * @brief Class destructor
         */
        virtual ~SHT4x() override;

        /**
         * @brief Get temperature
         *
         * @return float
         */
        float GetTemperature() const override;

        /**
         * @brief Get humanity
         *
         * @return float
         */
        virtual float GetHumanity() const override;

        /**
         * @brief Get serial number of the sensor
         *
         * @return std::string  : The serial number of the sensor in string format.
         *                        Empty string if the serial number can be obtain from the sensor
         */
        virtual std::string SerialNumber() const override;

        /**
         * @brief Trigger measurement on sensor (Single shot)
         */
        virtual void Measure() override;

        /**
         * @brief Software reset
         */
        virtual void SoftReset() const override;

        /**
         * @brief Activate highest heater power for 1s
         */
        void ActivateHeaterPower() const;

    private:
        /**
         * @brief Calculate values
         *
         * @param[in] sensorData : The sensor data
         */
        void CalculateAirValues(const std::vector<uint8_t> sensorData);
    };
}

#endif