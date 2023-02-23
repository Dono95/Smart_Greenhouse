#ifndef EVENT_DATA_H
#define EVENT_DATA_H

/* STD library*/
#include <cstdint>

/* Common components */
#include "Common_components/Utility/DataType/DataTypeUtility.hpp"

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
            ClientBluetoothEventData_Greenhouse(uint8_t clientID, uint8_t position)
                : mClientID(clientID),
                  mPosition(position),
                  mTemperature(nullptr),
                  mHumidity(nullptr),
                  mCO2(nullptr),
                  mSoilMoisture(nullptr)
            {
            }

            /**
             * @brief Class destructor
             */
            ~ClientBluetoothEventData_Greenhouse()
            {
                if (mTemperature)
                {
                    delete mTemperature;
                    mTemperature = nullptr;
                }

                if (mHumidity)
                {
                    delete mHumidity;
                    mHumidity = nullptr;
                }

                if (mCO2)
                {
                    delete mCO2;
                    mCO2 = nullptr;
                }

                if (mSoilMoisture)
                {
                    delete mSoilMoisture;
                    mSoilMoisture = nullptr;
                }
            }

            /**
             * @brief Get client ID
             *
             * @return uint8_t      : Client ID value
             */
            uint8_t GetClientID() const { return mClientID.Get(); }

            /**
             * @brief Get position
             *
             * @return uint8_t      : Position value
             */
            uint8_t GetPosition() const { return mPosition.Get(); }

            /**
             * @brief Check if temperature is set
             *
             * @return bool     : True  - temperature is set
             *                    False - otherwise
             */
            bool IsTemperatureSet() const { return mTemperature != nullptr; }

            /**
             * @brief Get temperature
             *
             * @return float    : Temperature value
             */
            float GetTemperature() const { return mTemperature->Get(); }

            /**
             * @brief Set temperature
             *
             * @param[in] temperature    : Temperature value
             */
            void SetTemperature(float temperature) { mTemperature = new Value<float>(temperature); }

            /**
             * @brief Check if humidity is set
             *
             * @return bool     : True  - humidity is set
             *                    False - otherwise
             */
            bool IsHumiditySet() const { return mHumidity != nullptr; }

            /**
             * @brief Get humidity
             *
             * @return float    : Humidity value
             */
            float GetHumidity() const { return mHumidity->Get(); }

            /**
             * @brief Set humidity
             *
             * @param[in] humidity    : Humidity value
             */
            void SetHumidity(float humidity) { mHumidity = new Value<float>(humidity); }

            /**
             * @brief Check if co2 is set
             *
             * @return bool     : True  - co2 is set
             *                    False - otherwise
             */
            bool IsCO2Set() const { return mCO2 != nullptr; }

            /**
             * @brief Get co2
             *
             * @return uint16_t    : CO2 value
             */
            uint16_t GetCO2() const { return mCO2->Get(); }

            /**
             * @brief Set co2
             *
             * @param[in] co2    : CO2 value
             */
            void SetCO2(uint16_t co2) { mCO2 = new Value<uint16_t>(co2); }

            /**
             * @brief Check if soil moisture is set
             *
             * @return bool     : True  - soil moisture is set
             *                    False - otherwise
             */
            bool IsSoilMoistureSet() const { return mSoilMoisture != nullptr; }

            /**
             * @brief Get soil moisture
             *
             * @return float    : Soil moisture value
             */
            float GetSoilMoisture() const { return mSoilMoisture->Get(); }

            /**
             * @brief Set soil moisture
             *
             * @param[in] soilMoisture    : Soil moisture value
             */
            void SetSoilMoisture(float soilMoisture) { mSoilMoisture = new Value<float>(soilMoisture); }

        private:
            // Alias for Value utility
            template <typename T>
            using Value = Utility::DataType::Value<T>;

            /* Client ID */
            const Value<uint8_t> mClientID;

            // Position
            const Value<uint8_t> mPosition;

            // Temperature
            Value<float> *mTemperature;

            // Humidity
            Value<float> *mHumidity;

            // CO2
            Value<uint16_t> *mCO2;

            // Soil moisture
            Value<float> *mSoilMoisture;
        };
    } // namespace Publisher
} // namespace Greenhouse

#endif // EVENT_DATA_H