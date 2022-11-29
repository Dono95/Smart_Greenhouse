/**
 * Definition of Greenhouse manager
 *
 * @author Dominik Regec
 */
#ifndef GREENHOUSE_MANAGER
#define GREENHOUSE_MANAGER

/* Project specific includes */
#include "ServerBluetoothController.hpp"
#include "ServerBluetoothHandler.hpp"

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
        using Shared_Bluetooth_Handler = std::shared_ptr<Bluetooth::ServerBluetoothHandler>;

        /**
         * @brief Static method to get instance of GreenhouseManager
         *
         * @return GreenhouseManager    : Pointer to Greenhouse manager
         */
        static GreenhouseManager *GetInstance();

        /**
         * @brief Method to start bluetooth; Consist of initialize bluetooth controller
         *
         * @return  true    : Start sequence was successful
         *          false   : Otherwise
         */
        bool StartBluetoothServer(void);

        /**
         * @brief Connect to WiFi network
         *
         * @note Parameter void of method is temporary and more parameters will be
         *       add in next version. Use default wifi parameter for now.
         */
        bool ConnectToNetwork(void);

        /**
         * @brief Getter for bluetooth handler
         *
         * @return Shared_Bluetooth_Handler
         */
        Shared_Bluetooth_Handler GetHandler() const;

    private:
        /* Shared pointer to Bluetooth controller*/
        using Shared_Bluetooth_Controller = std::shared_ptr<Bluetooth::ServerBluetoothController>;

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

        /* Shared pointer of Bluetooth controller*/
        Shared_Bluetooth_Controller mBluetoothController;

        /* Shared pointer of Bluetooth events handler*/
        Shared_Bluetooth_Handler mBluetoothHandler;
    };
} // namespace Greenhouse

#endif // GREENHOUSE_MANAGER