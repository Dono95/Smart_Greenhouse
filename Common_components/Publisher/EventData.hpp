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
            BluetoothEventData() {}

            /**
             * @brief Class destructor
             */
            ~BluetoothEventData() {}
        };
    } // namespace Publisher
} // namespace Greenhouse

#endif // EVENT_DATA_H