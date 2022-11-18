/**
 * Definition of Greenhouse manager
 *
 * @author Dominik Regec
 */
#ifndef GREENHOUSE_MANAGER
#define GREENHOUSE_MANAGER

/* Project specific includes */
#include "ClientBluetoothController.hpp"
#include "ClientBluetoothHandler.hpp"

/* STL includes */
#include <memory>
#include <mutex>

#define GREENHOUSE_MANAGER_TAG "Greenhouse Manager"

namespace Greenhouse
{
    class GreenhouseManager
    {
    public:
        /* Shared pointer to bluetooth events handler */
        using Shared_Bluetooth_Handler = std::shared_ptr<Component::Bluetooth::Interface::ClientBluetoothHandlerInterface>;

        /**
         * @brief Static method to get instance of GreenhouseManager
         */
        static GreenhouseManager *GetInstance();

        /**
         * @brief Method to inicialize controller and handler for bluetooth
         *
         * @return bool     true    : Initialization was successful
         *                  false   : Otherwise
         */

        bool StartBluetooth(void);

        /**
         * @brief Getter for bluetooth handler
         *
         * @return Shared_Bluetooth_Handler
         */
        Shared_Bluetooth_Handler GetHandler(void) const;

    private:
        /* Unique pointer to bluetooth controller */
        using Shared_Bluetooth_Controller = std::shared_ptr<Bluetooth::ClientBluetoothControlller>;

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

        /* Singleton mutex to protect instance from multithread */
        static std::mutex mManagerMutex;

        /* Shared pointer of Bluetooth controller*/
        Shared_Bluetooth_Controller mBluetoothController;

        /* Shared pointer to client bluetooth handler*/
        Shared_Bluetooth_Handler mBluetoothHandler;
    };
} // namespace Greenhouse

#endif // GREENHOUSE_MANAGER