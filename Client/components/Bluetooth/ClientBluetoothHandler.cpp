/* Project specific includes */
#include "ClientBluetoothHandler.hpp"
#include "GreenhouseManager.hpp"
#include "ClientStatusIndicator.hpp"

/* ESP log library*/
#include "esp_log.h"

/* STD library includes */
#include <algorithm>
#include <cstring>

/* SDK config */
#include "sdkconfig.h"

using namespace Greenhouse::Bluetooth;

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Class constructor
 */
ClientBluetoothHandler::ClientBluetoothHandler() : mRemoteDevice{CONFIG_BLUETOOTH_SERVER}, mConnected{false}
{
}

/**
 * @brief Class constructor with controller parameter
 */
ClientBluetoothHandler::ClientBluetoothHandler(std::weak_ptr<ClientBluetoothControlller> controller)
		: mBluetoothController(controller), mRemoteDevice{CONFIG_BLUETOOTH_SERVER}, mConnected{false}
{
}

/**
 * @brief Class destructor
 */
ClientBluetoothHandler::~ClientBluetoothHandler()
{
}

/**
 * @brief Get gattc profile data structure
 */
Component::Bluetooth::ClientGattcProfile ClientBluetoothHandler::GetGattcProfile(const uint8_t key) const
{
	auto profile = mProfilesMap.find(key);
	if (profile == mProfilesMap.end())
		return Component::Bluetooth::ClientGattcProfile();

	return profile->second;
}

/**
 * @brief Overriden method to initialize bluetooth profiles
 */
bool ClientBluetoothHandler::InitializeBluetoothProfiles()
{
	mProfilesMap.insert({GREENHOUSE_PROFILE, {.gattc_cb = [](esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
																						{
																							const auto manager = GreenhouseManager::GetInstance();

																							if (!manager)
																								return;

																							const auto clientHandler = std::dynamic_pointer_cast<ClientBluetoothHandler>(manager->GetHandler());
																							if (clientHandler)
																								clientHandler->GreenhouseEventHandler(event, gattc_if, param);
																						},
																						.gattc_if = ESP_GATT_IF_NONE}});

	ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Initialization of bluetooth profiles was successful.");
	return true;
}

/**
 * @brief Setter to set pointer to bluetooth controller
 */
void ClientBluetoothHandler::SetBluetoothController(std::weak_ptr<ClientBluetoothControlller> controller)
{
	mBluetoothController = controller;
}

/**
 * @brief Overriden method to handle events from gatts interface (BLE stack)
 */
void ClientBluetoothHandler::HandleGattcEvent(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
	if (mBluetoothController.expired())
	{
		ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Bluetooth controller is invalid. Unable to continue in handling event");
		return;
	}

	if (IsRegistrationEvent(event))
	{
		if (!HandleRegistrationEvent(gattc_if, param))
		{
			ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Unable to handle registration event.");
			return;
		}
	}

	for (const auto &profile : mProfilesMap)
	{
		if (gattc_if == ESP_GATT_IF_NONE || gattc_if == profile.second.gattc_if)
		{
			if (profile.second.gattc_cb)
				profile.second.gattc_cb(event, gattc_if, param);
		}
	}
}

/**
 * @brief Overriden method to handle event that are pushed from BLE stack
 */
void ClientBluetoothHandler::HandleGapEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
	ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "[%s] Event: %s", __func__, Component::Bluetooth::EnumToString(event).c_str());

	switch (event)
	{
	case (ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT):
	{
		if (const auto controller = mBluetoothController.lock())
			controller->StartScanning(30);

		break;
	}
	case (ESP_GAP_BLE_SCAN_START_COMPLETE_EVT):
	{
		// Scan start complete event to indicate scan start successfully or failed
		if (param->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
		{
			ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Scan start failed, error status = %x", param->scan_start_cmpl.status);
			return;
		}

		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Scanning start successfully");
		break;
	}
	case (ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT):
	{
		if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS)
		{
			ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Scan stop failed, error status = %x", param->adv_stop_cmpl.status);
			return;
		}

		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Scanning stop successfully");
		break;
	}
	case (ESP_GAP_BLE_SCAN_RESULT_EVT):
	{
		HandleScanResultEvent(param);
		break;
	}
	default:
		break;
	}
}

/**
 * @brief Method to handle events for Greenhouse profile
 */
void ClientBluetoothHandler::GreenhouseEventHandler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
	ESP_LOGD(CLIENT_BLUETOOTH_HANDLER_TAG, "[%s] Event: %s.", __func__, Component::Bluetooth::EnumToString(event).c_str());

	// Create shared_pointer from weak_ptr mBluetooth controller
	const auto controller = mBluetoothController.lock();
	if (!controller)
	{
		ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Unable to lock weak_ptr in middle of %s function", __func__);
		return;
	}

	switch (event)
	{
	case (ESP_GATTC_REG_EVT):
	{
		// Set scanning parameteres
		controller->SetScanParameters();
		break;
	}
	case (ESP_GATTC_OPEN_EVT):
	{
		if (param->open.status != ESP_GATT_OK)
		{
			ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Open virtual connection failed, status %d", param->open.status);
			return;
		}

		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Virtual connection was successfully open");
		break;
	}
	case (ESP_GATTC_WRITE_CHAR_EVT):
	{
		if (param->write.status != ESP_GATT_OK)
		{
			ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Write operation failed, error status = 0x%x", param->write.status);
			return;
		}

		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Write operation was successfully");
		break;
	}
	case (ESP_GATTC_CLOSE_EVT):
	{
		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Virtual connection was closed");
		break;
	}
	case (ESP_GATTC_SEARCH_CMPL_EVT):
	{
		if (param->search_cmpl.status != ESP_GATT_OK)
		{
			ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Search service failed, error status = %x", param->search_cmpl.status);
			return;
		}

		CheckServiceSource(param->search_cmpl.searched_service_source);

		HandleServiceInformation(gattc_if, param->search_cmpl.conn_id);
		break;
	}
	case (ESP_GATTC_SEARCH_RES_EVT):
	{
		if (param->search_res.srvc_id.uuid.len == ESP_UUID_LEN_16 && param->search_res.srvc_id.uuid.uuid.uuid16 == REMOTE_SERVICE_UUID)
		{
			mProfilesMap.at(GREENHOUSE_PROFILE).service_start_handle = param->search_res.start_handle;
			mProfilesMap.at(GREENHOUSE_PROFILE).service_end_handle = param->search_res.end_handle;
			ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Service found with UUID16: 0x%x", param->search_res.srvc_id.uuid.uuid.uuid16);
		}

		break;
	}
	case (ESP_GATTC_CFG_MTU_EVT):
	{
		if (param->cfg_mtu.status != ESP_GATT_OK)
		{
			ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Configuration of MTU failed, error status = 0x%x", param->cfg_mtu.status);
			return;
		}

		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "MTU configuration was successful, MTU set to %d", param->cfg_mtu.mtu);
		break;
	}
	case (ESP_GATTC_CONNECT_EVT):
	{
		mProfilesMap.at(GREENHOUSE_PROFILE).conn_id = param->connect.conn_id;
		memcpy(mProfilesMap.at(GREENHOUSE_PROFILE).remote_bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));

		SetConnectionStatus(true);

		controller->Send_MTU_Request(gattc_if, mProfilesMap.at(GREENHOUSE_PROFILE).conn_id);
		break;
	}
	case (ESP_GATTC_DISCONNECT_EVT):
	{
		HandleDisconnection(param->disconnect.reason);
		break;
	}
	case (ESP_GATTC_DIS_SRVC_CMPL_EVT):
	{
		if (param->dis_srvc_cmpl.status != ESP_GATT_OK)
		{
			ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Service discovery failed, status %d", param->dis_srvc_cmpl.status);
			return;
		}

		controller->SearchService(gattc_if, param->cfg_mtu.conn_id, &Component::Bluetooth::remote_filter_service_uuid);
		break;
	}
	default:
		ESP_LOGW(CLIENT_BLUETOOTH_HANDLER_TAG, "Unhandled event %s", Component::Bluetooth::EnumToString(event).c_str());
		break;
	}
}

/**
 * @brief Return connection status to remote device
 */
bool ClientBluetoothHandler::IsConnected() const
{
	return mConnected;
}

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Method to check if incoming event is registration event
 */
bool ClientBluetoothHandler::IsRegistrationEvent(esp_gattc_cb_event_t event) const
{
	return event == ESP_GATTC_REG_EVT;
}

/**
 * @brief Check service source
 */
void ClientBluetoothHandler::CheckServiceSource(esp_service_source_t source) const
{
	switch (source)
	{
	case ESP_GATT_SERVICE_FROM_REMOTE_DEVICE:
	{
		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Service information from remote device");
		break;
	}
	case ESP_GATT_SERVICE_FROM_NVS_FLASH:
	{
		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Service information from NVS flash");
	}
	default:
	{
		ESP_LOGW(CLIENT_BLUETOOTH_HANDLER_TAG, "unknown service source");
		break;
	}
	}
}

/**
 * @brief Handle registration event
 */
bool ClientBluetoothHandler::HandleRegistrationEvent(esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
	// Check if registration was successful
	if (param->reg.status != ESP_GATT_OK)
	{
		ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Reg app failed, app_id %04x, status %d\n", param->reg.app_id, param->reg.status);
		return false;
	}

	// Find profile based on ID
	auto profile = std::find_if(mProfilesMap.begin(), mProfilesMap.end(), [&](const std::pair<uint8_t, Component::Bluetooth::ClientGattcProfile> &itr)
															{
        if (itr.first == param->reg.app_id) {
            return true;
        }

        return false; });

	// If profile with specific ID exist, register Gatts interface to profile structure
	if (profile != mProfilesMap.end())
	{
		ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Register Gatts interfae [%d] to profile with ID: %d", gattc_if, profile->first);
		profile->second.gattc_if = gattc_if;
		return true;
	}

	return false;
}

/**
 * @brief Handle scanning result event
 */
void ClientBluetoothHandler::HandleScanResultEvent(esp_ble_gap_cb_param_t *scanResult)
{
	ESP_LOGD(CLIENT_BLUETOOTH_HANDLER_TAG, "[%s] Search event: %s.", __func__, Component::Bluetooth::EnumToString(scanResult->scan_rst.search_evt).c_str());

	switch (scanResult->scan_rst.search_evt)
	{
	case (ESP_GAP_SEARCH_INQ_RES_EVT):
	{
		uint8_t adv_name_len{0};
		uint8_t *adv_name = esp_ble_resolve_adv_data(scanResult->scan_rst.ble_adv, ESP_BLE_AD_TYPE_NAME_CMPL, &adv_name_len);

		if (adv_name)
		{
			if (mRemoteDevice.size() == adv_name_len && (strncmp((char *)adv_name, mRemoteDevice.c_str(), adv_name_len) == 0))
			{
				ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Searched device %s found", mRemoteDevice.c_str());
				if (IsConnected())
					break;

				if (const auto controller = mBluetoothController.lock())
				{
					ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Trying to connect to %s", mRemoteDevice.c_str());
					controller->StopScanning();
					controller->OpenConnection(mProfilesMap.at(GREENHOUSE_PROFILE).gattc_if, scanResult->scan_rst.bda, scanResult->scan_rst.ble_addr_type, true);
				}
			}
		}
		break;
	}
	default:
		break;
	}
}

/**
 * @brief Handle service information
 */
void ClientBluetoothHandler::HandleServiceInformation(esp_gatt_if_t gattc_if, uint16_t connectionID)
{
	uint16_t count{0};

	const auto controller = mBluetoothController.lock();
	if (!controller)
	{
		ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "No controller to use");
		return;
	}

	controller->GetAttributeCount(gattc_if, connectionID,
																ESP_GATT_DB_CHARACTERISTIC,
																mProfilesMap.at(GREENHOUSE_PROFILE).service_start_handle,
																mProfilesMap.at(GREENHOUSE_PROFILE).service_end_handle,
																0, &count);

	if (!count)
		return;

	esp_gattc_char_elem_t *element = (esp_gattc_char_elem_t *)malloc(sizeof(esp_gattc_char_elem_t) * count);
	if (!element)
	{
		ESP_LOGE(CLIENT_BLUETOOTH_HANDLER_TAG, "Allocate memmory for element failed");
		return;
	}

	controller->GetCharacteristicByUUID(gattc_if, connectionID,
																			mProfilesMap.at(GREENHOUSE_PROFILE).service_start_handle,
																			mProfilesMap.at(GREENHOUSE_PROFILE).service_end_handle,
																			Component::Bluetooth::remote_filter_char_uuid,
																			element,
																			&count);

	if (count > 0 && (element[0].properties & ESP_GATT_CHAR_PROP_BIT_WRITE))
	{
		mProfilesMap.at(GREENHOUSE_PROFILE).char_handle = element[0].char_handle;
	}

	// free char_elem_result
	free(element);
}

/**
 * @brief Handle discoonection event
 */
void ClientBluetoothHandler::HandleDisconnection(esp_gatt_conn_reason_t reason)
{
	ESP_LOGI(CLIENT_BLUETOOTH_HANDLER_TAG, "Client was disconnected with reason = 0x%x", reason);

	auto profile = mProfilesMap.find(GREENHOUSE_PROFILE);
	if (profile == mProfilesMap.end())
		return;

	const auto controller = mBluetoothController.lock();
	if (!controller)
		return;

	// Close virtual connection
	controller->CloseConnection(profile->second.gattc_if, profile->second.conn_id);

	switch (reason)
	{
	case (esp_gatt_conn_reason_t::ESP_GATT_CONN_TIMEOUT):
	{
		controller->StartScanning(30);
		break;
	}
	default:
		break;
	}

	SetConnectionStatus(false);
}

/**
 * @brief Set connection status
 */
void ClientBluetoothHandler::SetConnectionStatus(bool currentState)
{
	mConnected = currentState;
}