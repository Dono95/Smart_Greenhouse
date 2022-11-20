#ifndef BLUETOOTH_DATA_OBSERVER_H
#define BLUETOOTH_DATA_OBSERVER_H

/* Common components includes */
#include "Observer/ObserverInterface.hpp"

namespace Greenhouse
{
    namespace Observer
    {
        class BluetoothDataObserver : public Component::Observer::Interface::ObserverInterface
        {
        };
    } // namespace Observer
} // namespace Greenhouse

#endif