#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

/* STL library */
#include <utility>
#include <string>
#include <mutex>

/* Common components includes */
#include "Drivers/Network/WiFiDriver.hpp"

/* Project specific includes */
#include "Observers/BluetoothDataObserver.hpp"

#define NETWORK_MANAGER_TAG "Network manager"

namespace Greenhouse
{
    namespace Manager
    {
        class NetworkManager
        {
        public:
            /**
             * @brief Static method to get instance of NetworkManager
             *
             * @return NetworkManager    : Pointer to NetworkManager manager
             */
            static NetworkManager *GetInstance();

            /**
             * @brief Method for connection to wifi
             *
             * @param[in] wifiLogin : Reference to login wifi parameteres as name and password
             *
             * @return bool     true    : Device is connected to wifi network
             *                          : Connection to wifi network failed
             */
            bool ConnectToWifi(const std::pair<std::string, std::string> &wifiLogin);

        private:
            /**
             * @brief Class constructor
             */
            explicit NetworkManager();

            /**
             * @brief Class destructor
             */
            ~NetworkManager();

            /* Typedef to WiFi driver component */
            using WifiDriver = Component::Driver::Network::WifiDriver;

            /* Singleton instance of network manager */
            static NetworkManager *mInstance;

            /* Singleton mutex to protect instance from multithread*/
            static std::mutex mMutex;

            /* Wifi driver */
            WifiDriver *mWifiDriver;

            /* Data bluetooth  */
        };
    } // namespace Manager
} // namespace Greenhouse

#endif // NETWORK_MANAGER_H