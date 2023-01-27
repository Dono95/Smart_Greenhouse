#ifndef SCD4X_H
#define SCD4X_H

/* Interface */
#include "Sensor.hpp"

#define SCD4x_TAG "SCD4x_Sensor"

namespace Sensor
{
    class SCD4x : public AirSensor_I
    {
    public:
        /**
         * @brief Class constructor
         */
        explicit SCD4x(uint8_t i2C_address, I2C *i2c);

        /**
         * @brief Class destructor
         */
        virtual ~SCD4x() override;

        /**
         * @brief Get serial number of the sensor
         *
         * @return std::string  : The serial number of the sensor in string format.
         *                        Empty string if the serial number can be obtain from the sensor
         */
        virtual std::string SerialNumber() const override;

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
         * @brief Get CO2
         *
         * @return uint16_t
         */
        virtual uint16_t GetCO2() const override;

        /**
         * @brief Start periodic measurement
         *
         * @param[in] enable_low_power : True   : Periodic measurement will be low power
         *                               False  : regular periodic measurement without low power
         */
        void StartPeriodicMeasurement(const bool enable_low_power = true) const;

        /**
         * @brief Stop periodic measurement
         */
        void StopPeriodicMeasurement() const;

        /**
         * @brief Trigger measurement on sensor (Single shot)
         */
        virtual void Measure() override;

        /**
         * @brief Software reset
         */
        virtual void SoftReset() const override;

    private:
        /* Type alias -> 16 bit command convert to vector structure */
        using Command = std::vector<uint8_t>;

        /**
         * @brief Calculate values
         *
         * @param[in] sensorData : The sensor data
         */
        void CalculateAirValues(const std::vector<uint8_t> &sensorData);

        /**
         * @brief Format serial number
         *
         * @param[in] sensorData : The sensor data
         *
         * @return std::string
         */
        std::string FormatSerialNumber(const std::vector<uint8_t> &sensorData) const;
    };
} // namespace Sensor

#endif