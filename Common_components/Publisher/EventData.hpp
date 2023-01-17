#ifndef EVENT_DATA_H
#define EVENT_DATA_H

/* STD library*/
#include <cstdint>

/* Common components */
#include "Common_components/Utility/DataTypeUtility.hpp"

namespace Component
{
    namespace Publisher
    {
        class EventData
        {
        public:
            /**
             * @brief Class constructor
             */
            EventData() {}

            /**
             * @brief Class destructor
             */
            virtual ~EventData() {}
        };

        class ClientBluetoothEventData_Greenhouse : public EventData
        {
        public:
            /**
             * @brief Class constructor
             */
            ClientBluetoothEventData_Greenhouse(float temperature, int8_t humanity, int8_t co2) : mTemperature(temperature), mHumanity(humanity), mCO2(co2) {}

            /**
             * @brief Class destructor
             */
            ~ClientBluetoothEventData_Greenhouse() {}

            /**
             * @brief Set position
             *
             * @param[in] uint8_t    : Position
             */
            void SetPosition(uint8_t position) { mPosition.Set(position); }

            /**
             * @brief Get position
             *
             * @return uint8_t    : CO2 value
             */
            uint8_t GetPosition() const { return mPosition.Get(); }

            /**
             * @brief Get temperature
             *
             * @return float    : Temperature value
             */
            float GetTemperature() const { return mTemperature.Get(); }

            /**
             * @brief Get humanity
             *
             * @return int8_t    : Humanity value
             */
            int8_t GetHumanity() const { return mHumanity.Get(); }

            /**
             * @brief Get CO2
             *
             * @return int8_t    : CO2 value
             */
            int8_t GetCO2() const { return mCO2.Get(); }

        private:
            // Alias for Value utility
            template <typename T>
            using Value = Utility::DataType::Value<T>;

            // Temperature
            Value<float> mTemperature;

            // Humanity
            Value<int8_t> mHumanity;

            // CO2
            Value<int8_t> mCO2;

            // Position
            Value<uint8_t> mPosition;
        };
    } // namespace Publisher
} // namespace Greenhouse

#endif // EVENT_DATA_H