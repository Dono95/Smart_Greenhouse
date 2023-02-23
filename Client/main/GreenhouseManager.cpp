/* Project specific includes */
#include "GreenhouseManager.hpp"
#include "ClientStatusIndicator.hpp"
#include "Drivers/Sensors/SHT4x.hpp"
#include "Drivers/Sensors/SCD4x.hpp"

/* ESP logs library */
#include "esp_log.h"

/* C library */
#include <cstring>

/* STD library */
#include <limits>

#define TIMER_DIVIDER 16

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
			mBluetoothHandler(new Bluetooth::ClientBluetoothHandler(mBluetoothController)),
			mI2C(new I2C(GPIO_NUM_21, GPIO_NUM_22)),
			mSoilMoistureSensor(new Component::Driver::Sensor::SoilMoistureSensor(adc2_channel_t::ADC2_CHANNEL_3, ADC_WIDTH_12Bit, ADC_ATTEN_11db)),
			mBluetoothConnectionTracker(new Component::Tracker::BluetoothConnectionTracker(mBluetoothHandler->GetReferenceToConnectionState()))
{
	mI2C->SetMode(i2c_mode_t::I2C_MODE_MASTER, I2C_NUM_0, I2C_400_kHz);

	if (mI2C->Activate() != ESP_OK)
		ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Activation of I2C failed");

#ifdef CONFIG_CO2
	mAirSensor = new Sensor::SCD4x(0x62, mI2C);
#else
	mAirSensor = new Sensor::SHT4x(0x44, mI2C);
#endif
}

/**
 * @brief Class destructor
 */
GreenhouseManager::~GreenhouseManager()
{
}

/**
 * @brief Method to prepare data for sending to BLE server
 */
void GreenhouseManager::PrepareData(BluetoothDataVector &data)
{
	// Client ID and position
	data.emplace_back((CONFIG_CLIENT_ID << 2) | GetPosition());
	// Data content fill up during incialization data strucutre with sensors values
	data.emplace_back(0x00);

	// Temperature
#ifdef CONFIG_TEMPERATURE
	auto temperature = mAirSensor->GetTemperature();
	ESP_LOGI(GREENHOUSE_MANAGER_TAG, "Temperature is %.2f °C", temperature);
	data.emplace_back(GetExponent<uint8_t>(temperature));
	ESP_LOGD(GREENHOUSE_MANAGER_TAG, "Temperature exponent: %d", data.back());
	data.emplace_back(GetMantisa<uint8_t>(temperature, 2));
	ESP_LOGD(GREENHOUSE_MANAGER_TAG, "Temperature mantisa: %d", data.back());

	data.at(1) |= 0x80;
#endif
	// Humanity
#ifdef CONFIG_HUMANITY
	auto humanity = mAirSensor->GetHumanity();
	ESP_LOGI(GREENHOUSE_MANAGER_TAG, "Humanity is %.2f %%", humanity);
	data.emplace_back(GetExponent<uint8_t>(humanity));
	ESP_LOGD(GREENHOUSE_MANAGER_TAG, "Humanity exponent: %d", data.back());
	data.emplace_back(GetMantisa<uint8_t>(humanity, 2));
	ESP_LOGD(GREENHOUSE_MANAGER_TAG, "Humanity mantisa: %d", data.back());

	data.at(1) |= 0x40;
#endif
// CO2
#ifdef CONFIG_CO2
	auto co2 = mAirSensor->GetCO2();
	ESP_LOGI(GREENHOUSE_MANAGER_TAG, "CO2 is %d ppm\n", co2);
	data.emplace_back((co2 >> 8) & 0xFF); // H
	ESP_LOGD(GREENHOUSE_MANAGER_TAG, "CO2 H register: %d", data.back());
	data.emplace_back(co2 & 0xFF); // L
	ESP_LOGD(GREENHOUSE_MANAGER_TAG, "CO2 L register: %d", data.back());

	data.at(1) |= 0x20;
#endif
// Soil moisture
#ifdef CONFIG_SOIL_MOISURE
	auto soilMoisure = mSoilMoistureSensor->Measure();
	ESP_LOGI(GREENHOUSE_MANAGER_TAG, "Soil moisure is %.2f %%", soilMoisure);
	data.emplace_back(GetExponent<uint8_t>(soilMoisure));
	ESP_LOGD(GREENHOUSE_MANAGER_TAG, "Soil exponent: %d", data.back());
	data.emplace_back(GetMantisa<uint8_t>(soilMoisure, 2));
	ESP_LOGD(GREENHOUSE_MANAGER_TAG, "Soil mantisa: %d", data.back());

	data.at(1) |= 0x10;
#endif
}

/**
 * @brief Method to fill space inside bluetooth vector when some values is not defined from sensor
 */
void GreenhouseManager::FillSpace(BluetoothDataVector &data, const uint8_t spaces)
{
	data.insert(data.end(), spaces, std::numeric_limits<BluetoothDataVector::value_type>::max());
}

/**
 * @brief Get position of client
 */
uint8_t GreenhouseManager::GetPosition() const
{
#ifdef CONFIG_INSIDE
	return 0x01;
#elif CONFIG_OUTSIDE
	return 0x02;
#else
	return 0x00;
#endif
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
 * @brief Start tracking bluetooth connection
 */
void GreenhouseManager::StartTrackBluetoothConnection()
{
	mBluetoothConnectionTracker->StartTracking(10000);
}

/**
 * @brief Getter for bluetooth handler
 */
GreenhouseManager::Shared_Bluetooth_Handler GreenhouseManager::GetHandler(void) const
{
	return mBluetoothHandler;
}

/**
 * @brief Method to trigger action for sending measurement data to BLE server
 */
void GreenhouseManager::SendDataToServer()
{
	if (!mBluetoothHandler->IsConnected())
	{
		ESP_LOGE(GREENHOUSE_MANAGER_TAG, "Client in not connected to server. Unable to send data");
		ClientStatusIndicator::GetInstance()->RaiseState(StateCode::CLIENT_NOT_CONNECTED_TO_BLE_SERVER);
		return;
	}

	// Perform measurement
	mAirSensor->Measure();

	auto profile = mBluetoothHandler->GetGattcProfile(GREENHOUSE_PROFILE);

	BluetoothDataVector data;
	PrepareData(data);

	mBluetoothController->WriteCharacteristic(profile.gattc_if, profile.conn_id, profile.char_handle, data,
																						ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
}