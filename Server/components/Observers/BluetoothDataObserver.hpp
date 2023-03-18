#ifndef BLUETOOTH_DATA_OBSERVER_H
#define BLUETOOTH_DATA_OBSERVER_H

/* Common components includes */
#include "Observer/ObserverInterface.hpp"
#include "Publisher/EventData.hpp"

/* Project specific includes */
#include "Managers/EventManager.hpp"

#define BLUETOOTH_DATA_OBSERVER_TAG "Bluetooth data observer"

namespace Greenhouse
{
    namespace Observer
    {
        class BluetoothDataObserver : public Component::Observer::Interface::ObserverInterface
        {
        public:
            /**
             * @brief Class constructor
             *
             * @param[in] manager : Pointer to event manager
             */
            BluetoothDataObserver(Greenhouse::Manager::EventManager *manager);

            /**
             * @brief Class destructor
             */
            ~BluetoothDataObserver();

            /**
             * @brief Method which is called be event manager for notify client
             *
             * @param[in] eventData : Pointer to event data
             */
            void Update(Component::Publisher::EventData *eventData) override;

        private:
            /**
             * @brief Handle bluetooth data
             *
             * @param[in] event_data : Bluetooth data
             */
            static void HandleBluetoothData(void *event_data);
        };
    } // namespace Observer
} // namespace Greenhouse

#endif