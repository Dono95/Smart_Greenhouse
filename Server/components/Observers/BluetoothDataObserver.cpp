#include "BluetoothDataObserver.hpp"

using namespace Greenhouse::Observer;

/**
 * @brief Class constructor
 */
BluetoothDataObserver::BluetoothDataObserver(Greenhouse::Manager::EventManager *manager)
    : ObserverInterface(Event_T::BLUETOOTH_DATA_RECEIVED)
{
    if (!manager)
        return;

    manager->Subscribe(GetObservedEvent(), this);
}

/**
 * @brief Class destructor
 */
BluetoothDataObserver::~BluetoothDataObserver()
{
}