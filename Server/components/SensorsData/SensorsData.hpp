#ifndef SENSORS_DATA
#define SENSORS_DATA

/* STD library*/
#include <cstdint>

namespace Greenhouse
{
    template <class T>
    class SensorValue
    {
    public:
        /**
         * @brief Class constructor
         */
        explicit SensorValue(T value = 0) : mSensorValue(value) {}

        /**
         * @brief Class destructor
         */
        ~SensorValue() {}

        /**
         * @brief Set sensor value
         *
         * @param[in] value : Value
         */
        void Set(T value) { mSensorValue = value; }

        /**
         * @brief Get sensor value
         *
         * @return float
         */
        T Get() const { return mSensorValue; }

    private:
        T mSensorValue;
    };

    class SensorsData
    {
    public:
        /**
         * @brief Class constructor
         */
        explicit SensorsData(float temperature, uint8_t humanity);

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
         * @return uint8_t
         */
        uint8_t GetHumanity() const;

    private:
        // Temperature
        SensorValue<float> mTemperature;

        // Humanity
        SensorValue<uint8_t> mHumanity;
    };
} // namespace Greenhouse

#endif