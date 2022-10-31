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

const static char *GREENHOUSE_MANAGER_TAG = "Greenhouse Manager";

namespace Greenhouse
{
    class GreenhouseManager
    {
    public:
        /* Shared pointer to bluetooth events handler*/
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
        /**
         * @brief Class constructor
         */
        explicit GreenhouseManager();

        /**
         * @brief Class destructor
         */
        ~GreenhouseManager();

        /* Unique pointer to bluetooth controller */
        using Unique_Bluetooth_Controller = std::unique_ptr<Bluetooth::ClientBluetoothControlller>;

        /* Singleton instance of GreenhouseManager object */
        static GreenhouseManager *mManagerInstance;

        /* Unique pointer of Bluetooth controller*/
        Unique_Bluetooth_Controller mBluetoothController;

        /* Shared pointer to client bluetooth handler*/
        Shared_Bluetooth_Handler mBluetoothHandler;
    };
} // namespace Greenhouse

#endif // GREENHOUSE_MANAGER