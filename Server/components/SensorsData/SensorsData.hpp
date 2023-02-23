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
    using Value = Utility::DataType::Value<T>;

    enum class Position
    {
        UNKNOWN = 0x00,
        INSIDE = 0x01,
        OUTSIDE = 0x02
    };

    struct BasicData
    {
        // Client ID
        uint8_t clientID;
        // Position
        Position position{Position::UNKNOWN};

        // Time
        time_t time;
    };

    struct Air
    {
        // Sensor temperature
        Value<float> temperature;

        // Sensor humidity
        Value<float> humidity;

        // Sensor CO2
        Value<uint16_t> co2;
    };

    struct Soil
    {
        // Sensor soil moisture
        Value<float> soilMoisture;
    };

    struct SensorsData
    {
        /**
         * @brief Struct constructor
         */
        explicit SensorsData()
        {
            basic.time = Component::Manager::TimeManager::GetInstance()->GetRawTime();
        }

        ~SensorsData() {}

        // Basic information about sensors data
        BasicData basic;

        // Sensors values from air
        Air air;

        // Sensors values from soil
        Soil soil;
    };

} // namespace Greenhouse

#endif