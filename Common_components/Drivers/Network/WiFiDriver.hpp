#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

/* ESP library includes */
#include <esp_wifi.h>

/* STL library includes */
#include <string>

#include <freertos/event_groups.h>

#define WIFI_DRIVER_TAG "WiFi driver"

#define DEFAULT_MAX_TIMEOUT 30000 // 30s
#define MAX_NUMBER_OF_ATTEMPTS 10 // Maximam numbers of attempts

namespace Component
{
    namespace Driver
    {
        namespace Network
        {
            enum class WiFi_MODE
            {
                MODE_AP, // Soft-AP mode
                MODE_STA // Station mode
            };

            class WifiDriver
            {
            public:
                /**
                 * @brief Class constructor
                 *
                 * @param[in]   ssid        : Servise set identifier
                 * @param[in]   password    : Password
                 * @param[in]   mode        : Wifi mode
                 */
                explicit WifiDriver(const std::string &ssid, const std::string &password, WiFi_MODE mode);

                /**
                 * @brief Class destructor
                 */
                ~WifiDriver();

                /**
                 * @brieg Enable the use of the Wifi driver
                 */
                void Enable();

                /**
                 * @brief Disable the use of the Wifi driver
                 */
                void Disable();

                /**
                 * @brief Check if the Wifi driver is enabled to use
                 */
                bool IsEnabled() const;

                /**
                 * @brief Method to connect ot WiFi
                 */
                bool Connect();

                /**
                 * @brief Check if the ESP module is connected to AP
                 */
                bool IsConnected() const;

                /**
                 * @brief Method to get wifi name of connnected WiFi network
                 *
                 * @return std::string  : Name of connected WiFi network
                 */
                std::string GetWifiName() const;

                /**
                 * @brief Get IP address
                 *
                 * @return esp_ip4_addr_t
                 */
                esp_ip4_addr_t GetIpAddress() const;

                /**
                 * @brief Set IP address
                 *
                 * @param[in] ip_address : Reference to IP address structure
                 */
                void SetIpAddress(esp_ip4_addr_t &ip_address);

                /**
                 * @brief Method to handle wifi events
                 *
                 * @param[in] arg       : Event arguments
                 * @param[in] eventBase : Event base
                 * @param[in] eventID   : Event ID
                 * @param[in] eventData : Event data
                 */
                void WifiEventHandler(void *arg, esp_event_base_t eventBase,
                                      int32_t eventID, void *eventData);

            private:
                /* Inicialization config passed to esp_wifi_init call */
                wifi_init_config_t mInitConfig;

                /* Config parameteres structure */
                wifi_config_t mConfig;

                /* Service set identifier */
                std::string mSSID;

                /* Store value if Wifi driver is enabled to use*/
                bool mEnabled;

                /* Store value if WiFi has IP address and is connected to AP*/
                volatile bool mConnected;

                /* IP address */
                esp_ip4_addr_t mIP_Address;

                /* Number of attempts */
                volatile uint8_t mAttempts;

            private:
                class WiFiDriverManager
                {

                public:
                    /**
                     * @brief Static method to get instance of WiFiDriverManager
                     *
                     * @return WiFiDriverManager : Pointer to WiFiDriverManager
                     */
                    static WiFiDriverManager &GetInstance()
                    {
                        static WiFiDriverManager manager;
                        return manager;
                    }

                    /**
                     * @brief Method to set active WiFi driver
                     *
                     * @param[in] wifiDriver    : Pointer to WiFi driver
                     */
                    void SetWiFiDriver(WifiDriver *wifiDriver)
                    {
                        if (!wifiDriver && (wifiDriver != mWifiDriver))
                            return;

                        mWifiDriver = wifiDriver;
                    }

                    /**
                     * @brief Method to get active WiFi driver
                     *
                     * @return WiFiDriver   : Pointer to WiFi driver
                     */
                    WifiDriver *GetWiFiDriver() const
                    {
                        return mWifiDriver;
                    }

                private:
                    /**
                     * @brief Class constructor
                     */
                    WiFiDriverManager()
                    {
                    }

                    /**
                     * @brief Class destructor
                     */
                    ~WiFiDriverManager()
                    {
                    }

                    /* Pointer to WiFi driver */
                    WifiDriver *mWifiDriver;
                };
            };
        } // namespace Network

    } // namespace Driver
} // namespace Component

#endif