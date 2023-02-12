/* Project specific includes */
#include "Bluetooth/BluetoothDefinitions.hpp"
#include "ClientBluetoothController.hpp"
#include "GreenhouseManager.hpp"

/* STL includes*/
#include <iostream>

/* ESP logs library */
#include "esp_log.h"

using namespace Greenhouse::Bluetooth;

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Class constructor
 */
ClientBluetoothControlller::ClientBluetoothControlller()
{
}

/**
 * @brief Class destructor
 */
ClientBluetoothControlller::~ClientBluetoothControlller()
{
}

/**
 * @brief Method to register gap and gatts callback
 */
ClientBluetoothControlller::INIT_BLE_STATUS ClientBluetoothControlller::RegisterCallbacks(void)
{
	esp_err_t registerResult = esp_ble_gap_register_callback([](esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
																													 {
        const auto manager = GreenhouseManager::GetInstance();

        if (manager)
            manager->GetHandler()->HandleGapEvent(event, param); });

	if (registerResult)
	{
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "gap register error, error code = %x", registerResult);
		return INIT_BLE_STATUS::RV_REGISTER_CALLBACK_FAILED;
	}

	registerResult = esp_ble_gattc_register_callback([](esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
																									 {
        const auto manager = GreenhouseManager::GetInstance();

        if (manager)
            manager->GetHandler()->HandleGattcEvent(event, gattc_if, param); });

	if (registerResult)
	{
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "gatts register error, error code = %x", registerResult);
		return INIT_BLE_STATUS::RV_REGISTER_CALLBACK_FAILED;
	}

	registerResult = esp_ble_gattc_app_register(GREENHOUSE_PROFILE);
	if (registerResult)
	{
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "gatts app register failed, error code = %x", registerResult);
		return INIT_BLE_STATUS::RV_REGISTER_APP_FAILED;
	}
	return INIT_BLE_STATUS::RV_BLUETOOTH_INIT_OK;
}

/**
 * @brief Start device scanning in periodic sequences
 */
esp_err_t ClientBluetoothControlller::StartScanning(uint32_t duration)
{
	auto result = esp_ble_gap_start_scanning(duration);
	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Failed to start scan with result code %d", result);

	return result;
}

/**
 * @brief Stop device scanning
 */
esp_err_t ClientBluetoothControlller::StopScanning()
{
	auto result = esp_ble_gap_stop_scanning();
	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Failed to stop scan with result code %d", result);

	return result;
}

/**
 * @brief Set scan parameters
 */
esp_err_t ClientBluetoothControlller::SetScanParameters(esp_ble_scan_params_t *params)
{
	if (params)
		ble_scan_params = *params;

	esp_err_t result = esp_ble_gap_set_scan_params(&ble_scan_params);
	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Failed to set scan parameters with result code %d", result);

	return result;
}

/**
 * @brief Open a virtual connection to remote device
 */
esp_err_t ClientBluetoothControlller::OpenConnection(esp_gatt_if_t gattc_if, esp_bd_addr_t remoteAddress, esp_ble_addr_type_t removeAddressType, bool isDirect)
{
	esp_err_t result = esp_ble_gattc_open(gattc_if, remoteAddress, removeAddressType, isDirect);
	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Failed to open connection to remote device with result code %d", result);

	return result;
}

/**
 * @brief Close a virtual connection to remote device
 */
esp_err_t ClientBluetoothControlller::CloseConnection(esp_gatt_if_t gattc_if, uint16_t connectionID)
{
	esp_err_t result = esp_ble_gattc_close(gattc_if, connectionID);
	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Failed to close connection to remote device with result code %d", result);

	return result;
}

/**
 * @brief Configure the MTU size in the GATT channel.
 */
esp_err_t ClientBluetoothControlller::Send_MTU_Request(esp_gatt_if_t gattc_if, uint16_t connectionID)
{
	esp_err_t result = esp_ble_gattc_send_mtu_req(gattc_if, connectionID);
	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Failed to configure the MTU size");

	return result;
}

/**
 * @brief Serach service from local cache
 */
esp_err_t ClientBluetoothControlller::SearchService(esp_gatt_if_t gattc_if, uint16_t connectionID, esp_bt_uuid_t *serviceFilter)
{
	esp_err_t result = esp_ble_gattc_search_service(gattc_if, connectionID, serviceFilter);
	if (result)
	{
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Failed to seatch service with UUID");
		if (serviceFilter->len == ESP_UUID_LEN_128)
		{
			ESP_LOG_BUFFER_HEX_LEVEL(GREENHOUSE_MANAGER_TAG, serviceFilter->uuid.uuid128, ESP_UUID_LEN_128, ESP_LOG_ERROR);
			return result;
		}

		auto service_uuid{0};

		if (serviceFilter->len == ESP_UUID_LEN_16)
			service_uuid = serviceFilter->uuid.uuid16;
		else if (serviceFilter->len == ESP_UUID_LEN_32)
			service_uuid = serviceFilter->uuid.uuid16;

		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "0x%x", service_uuid);
	}

	return result;
}

/**
 * @brief Get attribute count
 */
esp_err_t ClientBluetoothControlller::GetAttributeCount(esp_gatt_if_t gattc_if, uint16_t connectionID, esp_gatt_db_attr_type_t type,
																												uint16_t startHandle, uint16_t endHandle, uint16_t charHandle, uint16_t *count)
{
	esp_err_t result = esp_ble_gattc_get_attr_count(gattc_if, connectionID, type, startHandle, endHandle, charHandle, count);
	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Unable to get number of attribute");

	return result;
}

/**
 * @brief Get characteristic by UUID
 */
esp_err_t ClientBluetoothControlller::GetCharacteristicByUUID(esp_gatt_if_t gattc_if, uint16_t connectionID, uint16_t startHandle, uint16_t endHandle,
																															esp_bt_uuid_t characteristicUUID, esp_gattc_char_elem_t *characteristicElement, uint16_t *count)
{
	esp_err_t result = esp_ble_gattc_get_char_by_uuid(gattc_if, connectionID, startHandle, endHandle,
																										characteristicUUID, characteristicElement, count);
	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Unable to get characteristic by UUID");

	return result;
}

/**
 * @brief Write characteristic value
 */
esp_err_t ClientBluetoothControlller::WriteCharacteristic(esp_gatt_if_t gattc_if, uint16_t connectionID,
																													uint16_t characterissticHandle, std::vector<uint8_t> &value,
																													esp_gatt_write_type_t writeType, esp_gatt_auth_req_t authenticationRequest)
{
	auto result = esp_ble_gattc_write_char(gattc_if, connectionID, characterissticHandle,
																				 value.size(), value.data(), writeType, authenticationRequest);

	if (result)
		ESP_LOGE(CLIENT_BLUETOOTH_CONTROLLER_TAG, "Writing characteristic value failed");

	return result;
}
