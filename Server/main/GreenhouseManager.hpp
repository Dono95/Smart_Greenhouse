/**
 * Definition of Greenhouse manager
 *
 * @author Dominik Regec
 */
#ifndef GREENHOUSE_MANAGER
#define GREENHOUSE_MANAGER

/* Project specific includes */
#include "Bluetooth/BaseBluetoothHandlerInterface.hpp"
#include "Bluetooth/ServerBluetoothHandler.hpp"
#include "Bluetooth/BluetoothController.hpp"

/* STL includes */
#include <memory>
#include <mutex>

#define GREENHOUSE_MANAGER_TAG "Greenhouse Manager"

namespace Greenhouse
{
    class GreenhouseManager
    {
    public:
        /* Shared pointer to base bluetooth */
        using Shared_BaseHandlerInterface = std::shared_ptr<Bluetooth::BaseBluetoothHandlerInterface>;

        /**
         * @brief Static method to get instance of GreenhouseManager
         */
        static GreenhouseManager *GetInstance();

        /**
         * @brief Method to start bluetooth; Consist of initialize bluetooth controller
         *
         * @return  true    : Start sequence was successful
         *          false   : Otherwise
         */
        bool StartBluetooth();

        Shared_BaseHandlerInterface GetBluetoothHandler() const;

    private:
        /**
         * @brief Class constructor
         */
        explicit GreenhouseManager();

        /**
         * @brief Class destructor
         */
        ~GreenhouseManager();

        /* Singleton instance of GreenhouseManager object */
        static GreenhouseManager *mManagerInstance;

        /* Singleton mutex to protect instance from multithread*/
        static std::mutex mManagerMutex;

        /* Unique pointer of Bluetooth controller*/
        std::unique_ptr<BluetoothController> mBluetoothController;

        /* Shared pointer of Bluetooth events handler*/
        Shared_BaseHandlerInterface mBluetoothHandler;
    };
} // namespace Greenhouse

#endif // GREENHOUSE_MANAGER