#ifndef SENSORS_DATA
#define SENSORS_DATA

namespace Greenhouse
{
    template <class T>
    class Temperature
    {
    public:
        /**
         * @brief Class constructor
         */
        explicit Temperature(T temperature = 0) : mTemperature(temperature) {}

        /**
         * @brief Class destructor
         */
        ~Temperature() {}

        /**
         * @brief Set temperature value
         *
         * @param[in] temperature : Temperature value
         */
        void SetTemperature(T temperature) { mTemperature = temperature; }

        /**
         * @brief Get temperature value
         *
         * @return float
         */
        T GetTemperature() const { return mTemperature; }

    private:
        T mTemperature;
    };

    class SensorsData
    {
    public:
        /**
         * @brief Class constructor
         */
        explicit SensorsData() : mTemperature(0) {}

        /**
         * @brief Class destructor
         */
        ~SensorsData() {}

        /**
         * @brief Set temperature
         *
         * @param[in] temperature : Temperature value
         */
        void SetTemperature(float temperature) { mTemperature.SetTemperature(temperature); }

        /**
         * @brief Get temperature
         *
         * @return float
         */
        float GetTemperature() const { return mTemperature.GetTemperature(); }

    private:
        Temperature<float> mTemperature;
    };
} // namespace Greenhouse

#endif