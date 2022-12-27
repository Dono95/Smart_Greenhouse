#ifndef BLUETOOTH_DATA_OBSERVER_H
#define BLUETOOTH_DATA_OBSERVER_H

/* Common components includes */
#include "Observer/ObserverInterface.hpp"
#include "Publisher/EventData.hpp"

/* Project specific includes */
#include "Managers/EventManager.hpp"

namespace Greenhouse
{
    namespace Observer
    {
        class BluetoothDataObserver : public Component::Observer::Interface::ObserverInterface
        {
        public:
            /**
             * @brief Class constructor
             */
            BluetoothDataObserver(Greenhouse::Manager::EventManager *manager);

            /**
             * @brief Class destructor
             */
            ~BluetoothDataObserver();

            void Update(Component::Publisher::EventData *eventData) override;
        };
    } // namespace Observer
} // namespace Greenhouse

#endif