/* Project specific includes */
#include "GreenhouseManager.hpp"
#include "Drivers/Sensors/SHT4x.hpp"
#include "Drivers/Sensors/SCD4x.hpp"

/* ESP logs library */
#include "esp_log.h"

#define I2C_400_kHz 400000u

using namespace Greenhouse;

GreenhouseManager *GreenhouseManager::mManagerInstance{nullptr};
std::mutex GreenhouseManager::mManagerMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
GreenhouseManager::GreenhouseManager()
		: mBluetoothController(new Bluetooth::ClientBluetoothControlller()),
			mBluetoothHandler(new Bluetooth::ClientBluetoothHandler(mBluetoothController))
{
	// Inicialize I2C
	mI2C = new I2C(GPIO_NUM_21, GPIO_NUM_22);
	mI2C->SetMode(i2c_mode_t::I2C_MODE_MASTER, I2C_NUM_0, I2C_400_kHz);

	if (mI2C->Activate() != ESP_OK)
		ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Activation of I2C failed");

#ifdef CONFIG_CO2
	mAirSensor = new Sensor::SCD4x(0x62, mI2C);
#else
	mAirSensor = new Sensor::SHT4x(0x44, mI2C);
#endif

	ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Serial number is %s\n", mAirSensor->SerialNumber().c_str());
	mAirSensor->Measure();

#ifdef CONFIG_TEMPERATURE
	ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Temperature is %.2f °C", mAirSensor->GetTemperature());
#endif

#ifdef CONFIG_HUMANITY
	ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Humanity is %.2f %%", mAirSensor->GetHumanity());
#endif

#ifdef CONFIG_CO2
	ESP_LOGE(GREENHOUSE_MANAGER_TAG, "CO2 is %d ppm\n", mAirSensor->GetCO2());
#endif
}

/**
 * @brief Class destructor
 */
GreenhouseManager::~GreenhouseManager()
{
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Static method to get instance of GreenhouseManager
 */
GreenhouseManager *GreenhouseManager::GetInstance()
{
	std::lock_guard<std::mutex> lock(mManagerMutex);
	if (!mManagerInstance)
		mManagerInstance = new GreenhouseManager();

	return mManagerInstance;
}

/**
 * @brief Method to inicialize controller and handler for bluetooth
 */
bool GreenhouseManager::StartBluetooth(void)
{
	using BluetoothInitStatus = Component::Bluetooth::INIT_BLUETOOTH_RV;

	if (mBluetoothController->InitBluetoothController(ESP_BT_MODE_BLE) != BluetoothInitStatus::RV_BLUETOOTH_INIT_OK)
	{
		ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Initialization of bluetooth controller failed");
		return false;
	}

	if (!mBluetoothHandler->InitializeBluetoothProfiles())
	{
		ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Initialization of bluetooth profiles failed");
		return false;
	}

	if (mBluetoothController->RegisterCallbacks() != BluetoothInitStatus::RV_BLUETOOTH_INIT_OK)
	{
		ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Callbacks registration failed");
		return false;
	}

	return true;
}

/**
 * @brief Getter for bluetooth handler
 */
GreenhouseManager::Shared_Bluetooth_Handler GreenhouseManager::GetHandler(void) const
{
	return mBluetoothHandler;
}