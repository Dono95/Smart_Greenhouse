#ifndef SHT4x_H
#define SHT4x_H

#include "Drivers/Com_Interfaces/I2C_I.hpp"
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
        enum class MeasurePrecision
        {
            HIGH = MEASURE_T_RH_HIGH_PRECISION,
            MEDIUM = MEASURE_T_RH_MEDIUM_PRECISION,
            LOW = MEASURE_T_RH_LOW_PRECISION
        };

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
} // namespace Interface

#endif /* SHT4x_H */