/* Project specific includes */
#include "ServerBluetoothController.hpp"
#include "GreenhouseManager.hpp"

/* ESP logs library */
#include "esp_log.h"

using namespace Greenhouse::Bluetooth;

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Class constructor
 */
ServerBluetoothController::ServerBluetoothController()
{
}

/**
 * @brief Class destructor
 */
ServerBluetoothController::~ServerBluetoothController()
{
}

/**
 * @brief Overridden method to register gap and gatts callback for client controller
 */
ServerBluetoothController::INIT_BLE_STATUS ServerBluetoothController::RegisterCallbacks(void)
{
    auto registerResult = esp_ble_gap_register_callback([](esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
                                                        {
        const auto manager = GreenhouseManager::GetInstance();
        if (manager)
            manager->GetHandler()->HandleGapEvent(event, param); });

    if (registerResult)
    {
        ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "Gap register error, error code = %x", registerResult);
        return INIT_BLE_STATUS::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gatts_register_callback([](esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
                                                     {
        const auto manager = GreenhouseManager::GetInstance();
        if (manager)
            manager->GetHandler()->HandleGattsEvent(event, gatts_if, param); });

    if (registerResult)
    {
        ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "gatts register error, error code = %x", registerResult);
        return INIT_BLE_STATUS::RV_REGISTER_CALLBACK_FAILED;
    }

    registerResult = esp_ble_gatts_app_register(GREENHOUSE_PROFILE);
    if (registerResult)
    {
        ESP_LOGE(SERVER_BLUETOOTH_HANDLER_TAG, "gatts app register error, error code = %x", registerResult);
        return INIT_BLE_STATUS::RV_REGISTER_APP_FAILED;
    }

    return INIT_BLE_STATUS::RV_BLUETOOTH_INIT_OK;
}

/**
 * @brief Method to ser bluetooth device name
 */
void ServerBluetoothController::SetDeviceName(const std::string &deviceName)
{
    esp_err_t result = esp_ble_gap_set_device_name(deviceName.c_str());
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Set device name failed with error code: %x", result);
}

/**
 * @brief Method to set config advertising data
 */
esp_err_t ServerBluetoothController::SetConfigAdvData(esp_ble_adv_data_t *adv_data)
{
    if (!adv_data)
    {
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "No data to set. Nullptr send to method: %s", __func__);
        return ESP_ERR_INVALID_ARG;
    }

    auto result = esp_ble_gap_config_adv_data(adv_data);
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Set config adv data failed with code %x", result);

    return result;
}

/**
 * @brief Method to set scan response data
 */
esp_err_t ServerBluetoothController::SetScanResponseData(esp_ble_adv_data_t *scan_rsp_data)
{
    if (!scan_rsp_data)
    {
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "No data to set. Nullptr send to method: %s", __func__);
        return ESP_ERR_INVALID_ARG;
    }

    auto result = esp_ble_gap_config_adv_data(scan_rsp_data);
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Set scan response data failed with code %x", result);

    return result;
}

/**
 * @brief Method to start advertising
 */
esp_err_t ServerBluetoothController::StartAdvertising(esp_ble_adv_params_t *advParameter)
{
    if (!advParameter)
        return ESP_ERR_INVALID_ARG;

    return esp_ble_gap_start_advertising(advParameter);
}

/**
 * @brief Method to create bluetooth service
 */
esp_err_t ServerBluetoothController::CreateService(esp_gatt_if_t gatts_if, esp_gatt_srvc_id_t *service_id, uint16_t num_handle)
{
    if (!service_id)
        return ESP_ERR_INVALID_ARG;

    auto result = esp_ble_gatts_create_service(gatts_if, service_id, num_handle);
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Unable to create service.");

    return result;
}

/**
 * @brief Method to start service
 */
esp_err_t ServerBluetoothController::StartService(uint16_t serviceHandle)
{
    auto result = esp_ble_gatts_start_service(serviceHandle);
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Starting service [%d] failed.", serviceHandle);

    return result;
}

/**
 * @brief Method to update connection parameteres
 */
esp_err_t ServerBluetoothController::UpdateConParameteres(esp_ble_conn_update_params_t *conParameteres)
{
    if (!conParameteres)
        return ESP_ERR_INVALID_ARG;

    auto result = esp_ble_gap_update_conn_params(conParameteres);
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Unable to update connection parameters!");

    return result;
}

/**
 * @brief Method to add characteristic to service
 */
esp_err_t ServerBluetoothController::AddCharacteristicToService(uint16_t serviceHandle, esp_bt_uuid_t *characteristicUUID,
                                                                esp_gatt_perm_t permission, esp_gatt_char_prop_t property,
                                                                esp_attr_value_t *characteristicValue, esp_attr_control_t *control)
{
    if (!characteristicValue || !characteristicUUID)
    {
        ESP_LOGW(SERVER_BLUETOOTH_CONTROLLER_TAG, "One of provided arguments in function [%s] is invalid(nullptr). Unable to add characteristic to service[%d]", __func__, serviceHandle);
        return ESP_ERR_INVALID_ARG;
    }

    auto result = esp_ble_gatts_add_char(serviceHandle, characteristicUUID, permission, property, characteristicValue, control);
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Adding characteristic to service failed. Error code: [%d]", result);

    return result;
}

/**
 * @brief Method to retreive atribute values
 */
esp_err_t ServerBluetoothController::GetAttributeValues(uint16_t attr_handle, std::vector<uint8_t> &values)
{
    uint16_t length = 0;
    const uint8_t *arrayValues;

    esp_err_t getAttr = esp_ble_gatts_get_attr_value(attr_handle, &length, &arrayValues);
    if (getAttr == ESP_FAIL)
    {
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Illegal handler passed.");
        return getAttr;
    }

    for (uint16_t i = 0; i < length; ++i)
        values.emplace_back(arrayValues[i]);

    return ESP_OK;
}

/**
 * @brief Method to add characteristic descriptor
 */
esp_err_t ServerBluetoothController::AddCharacteristicDescriptor(uint16_t serviceHandle, esp_bt_uuid_t *descriptorUUID,
                                                                 esp_gatt_perm_t permission, esp_attr_value_t *charDescriptorVAlue, esp_attr_control_t *control)
{
    if (!descriptorUUID)
    {
        ESP_LOGW(SERVER_BLUETOOTH_CONTROLLER_TAG, "No set descriptor UUID. Unable to add characteristic descriptor.");
        return ESP_ERR_INVALID_ARG;
    }

    auto result = esp_ble_gatts_add_char_descr(serviceHandle, descriptorUUID, permission, charDescriptorVAlue, control);
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Adding characteristic descriptor failed. Error code: [%d]", result);

    return result;
}

/**
 * @brief Method to send response to client
 */
esp_err_t ServerBluetoothController::SendResponse(esp_gatt_if_t gatts_if, uint32_t connectionID, uint32_t transferID, esp_gatt_status_t status, esp_gatt_rsp_t *rsp)
{
    auto result = esp_ble_gatts_send_response(gatts_if, connectionID, transferID, status, rsp);
    if (result)
        ESP_LOGE(SERVER_BLUETOOTH_CONTROLLER_TAG, "Unable to send response. Error code: [%d]", result);

    return result;
}

/*********************************************
 *              PRIVATE API                  *
 ********************************************/