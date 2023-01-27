#ifndef SENSORS_DATA
#define SENSORS_DATA

/* STD library*/
#include <cstdint>

/* Common compoennts */
#include "Common_components/Utility/DataType/DataTypeUtility.hpp"
#include "Common_components/Managers/TimeManager.hpp"

namespace Greenhouse
{
    // Alias for sensor data type
    template <typename T>
    using SensorValue = Utility::DataType::Value<T>;

    class SensorsData
    {
    public:
        enum class Position
        {
            UNKNOWN = 0x00,
            INSIDE = 0x01,
            OUTSIDE = 0x02
        };

        /**
         * @brief Class constructor
         */
        explicit SensorsData(Position position, float temperature, int8_t humanity, int8_t co2);

        /**
         * @brief Get Position
         *
         * @return Position
         */
        Position GetPosition() const;

        /**
         * @brief Class destructor
         */
        ~SensorsData();

        /**
         * @brief Get temperature
         *
         * @return float
         */
        float GetTemperature() const;

        /**
         * @brief Get humanity
         *
         * @return int8_t
         */
        int8_t GetHumanity() const;

        /**
         * @brief Get CO2
         *
         * @return int8_t
         */
        int8_t GetCO2() const;

        /**
         * @brief Get measure time
         *
         * @return time_t
         */
        time_t GetMeasureTime() const;

    private:
        // Position
        Position mPosition;

        // Temperature
        SensorValue<float> mTemperature;

        // Humanity
        SensorValue<int8_t> mHumanity;

        // CO2
        SensorValue<int8_t> mCO2;

        // Creation time (Measure time)
        time_t mMeasureTime;
    };
} // namespace Greenhouse

#endif