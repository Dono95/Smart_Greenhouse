/**
 * Definition of Greenhouse manager
 * 
 * @author Dominik Regec
 */
#ifndef GREENHOUSE_MANAGER
#define GREENHOUSE_MANAGER

/* Project specific includes */
#include "ClientBluetoothController.hpp"

/* STL includes */
#include <memory>

const static char* GREENHOUSE_MANAGER_TAG = "Greenhouse Manager";

namespace Greenhouse
{
    class GreenhouseManager
    {
    public:
        /**
         * @brief Static method to get instance of GreenhouseManager
         */
        static GreenhouseManager* GetInstance();

    private:
        /**
         * @brief Class constructor
         */
        explicit GreenhouseManager();

        /**
         * @brief Class destructor
         */ 
        ~GreenhouseManager();

        using Unique_Bluetooth_Controller = std::unique_ptr<Bluetooth::ClientBluetoothControlller>;

        /* Singleton instance of GreenhouseManager object */
        static GreenhouseManager* mManagerInstance;

        /* Unique pointer of Bluetooth controller*/
        Unique_Bluetooth_Controller mBluetoothController;
    };
} // namespace Greenhouse

#endif // GREENHOUSE_MANAGER