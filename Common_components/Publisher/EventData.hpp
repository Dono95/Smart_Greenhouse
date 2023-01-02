#ifndef EVENT_DATA_H
#define EVENT_DATA_H

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

        class BluetoothEventData : public EventData
        {
        public:
            /**
             * @brief Class constructor
             */
            BluetoothEventData(float temperature, uint8_t humanity) : mTemperature(temperature), mHumanity(humanity) {}

            /**
             * @brief Class destructor
             */
            ~BluetoothEventData() {}

            /**
             * @brief Get temperature
             *
             * @return float    : Temperature value
             */
            float GetTemperature() const { return mTemperature; }

            /**
             * @brief Get humanity
             *
             * @return uint8_t    : Humanity value
             */
            uint8_t GetHumanity() const { return mHumanity; }

        private:
            // Temperature
            float mTemperature;

            // Humanity
            uint8_t mHumanity;
        };
    } // namespace Publisher
} // namespace Greenhouse

#endif // EVENT_DATA_H