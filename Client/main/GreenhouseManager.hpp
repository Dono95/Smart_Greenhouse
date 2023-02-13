#ifndef GREENHOUSE_MANAGER_H
#define GREENHOUSE_MANAGER_H

/* Project specific includes */
#include "ClientBluetoothController.hpp"
#include "ClientBluetoothHandler.hpp"
#include "Common_components/Drivers/Sensor/SoilMoistureSensor.hpp"
#include "Common_components/Drivers/Communication/I2C.hpp"
#include "Drivers/Sensors/Sensor.hpp"

/* STD library */
#include <memory>
#include <mutex>
#include <math.h>

#define GREENHOUSE_MANAGER_TAG "Greenhouse Manager"

namespace Greenhouse
{
    class GreenhouseManager
    {
    public:
        /* Shared pointer to bluetooth events handler */
        using Shared_Bluetooth_Handler = std::shared_ptr<Bluetooth::ClientBluetoothHandler>;

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

        /**
         * @brief Method to trigger action for sending measurement data to BLE server
         */
        void SendDataToServer();

    private:
        /* Unique pointer to bluetooth controller */
        using Shared_Bluetooth_Controller = std::shared_ptr<Bluetooth::ClientBluetoothControlller>;

        /* Alias for component driver I2C */
        using I2C = Component::Driver::Communication::I2C;

        /* Alias for bluetooth data vector */
        using BluetoothDataVector = std::vector<uint8_t>;

        /**
         * @brief Class constructor
         */
        explicit GreenhouseManager();

        /**
         * @brief Class destructor
         */
        ~GreenhouseManager();

        /**
         * @brief Method to prepare data for sending to BLE server
         *
         * @param[out] data     : Prepared data vector
         */
        void PrepareData(BluetoothDataVector &data);

        /**
         * @brief Method to fill space inside bluetooth vector when some values is not defined from sensor
         *
         * @param[in] data      : Bluetooth data vector
         * @param[in] spaces    : Number of space wich will be filled
         */
        void FillSpace(BluetoothDataVector &data, const uint8_t spaces);

        /**
         * @brief Get exponent from float number
         *
         * @param[in] number
         */
        template <typename Type>
        Type GetExponent(const float number)
        {
            return static_cast<Type>(number);
        }

        /**
         * @brief Get mantisa from float number
         *
         * @param[in] number
         * @param[in] precision     :
         */
        template <typename Type>
        Type GetMantisa(const float number, uint8_t precision = 2)
        {
            return static_cast<Type>((number - static_cast<Type>(number)) * pow(10, precision));
        }

        /**
         * @brief Get position of client
         *
         * @return uint8_t
         */
        uint8_t GetPosition() const;

        /* Singleton instance of GreenhouseManager object */
        static GreenhouseManager *mManagerInstance;

        /* Singleton mutex to protect instance from multithread */
        static std::mutex mManagerMutex;

        /* Shared pointer of Bluetooth controller*/
        Shared_Bluetooth_Controller mBluetoothController;

        /* Shared pointer to client bluetooth handler*/
        Shared_Bluetooth_Handler mBluetoothHandler;

        /* Pointer to I2C driver */
        I2C *mI2C;

        /* Pointer to air's properties sensor*/
        Sensor::AirSensor_I *mAirSensor;

        /* Pointer to soil moisure sensor */
        Component::Driver::Sensor::SoilMoistureSensor *mSoilMoistureSensor;
    };
} // namespace Greenhouse

#endif // GREENHOUSE_MANAGER_H