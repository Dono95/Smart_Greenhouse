#ifndef SERVER_BLUETOOTH_CONTROLLER
#define SERVER_BLUETOOTH_CONTROLLER

/* Common components */
#include "Bluetooth/BaseBluetoothController.hpp"

/* STL includes */
#include <string>
#include <vector>

#define SERVER_BLUETOOTH_CONTROLLER_TAG "ServerBluetoothController"

namespace Greenhouse
{
    namespace Bluetooth
    {
        class ServerBluetoothController : public Component::Bluetooth::BaseBluetoothController
        {
        public:
            /* Typedef for incialization bluetooth return code*/
            using INIT_BLE_STATUS = Component::Bluetooth::INIT_BLUETOOTH_RV;

            /**
             * @brief Class constructor
             */
            explicit ServerBluetoothController();

            /**
             * @brief Class destructor
             */
            ~ServerBluetoothController();

            /**
             * @brief Overridden method to register gap and gatts callback for client controller
             */
            virtual INIT_BLE_STATUS RegisterCallbacks(void) override;

            /**
             * @brief Method to ser bluetooth device name
             *
             * @param[in] deviceName    : Device name to set
             */
            void SetDeviceName(const std::string &deviceName);

            /**
             * @brief Method to set config advertising data
             *
             * @param[in]   adv_data    : Pointer to Advertising data structure
             *
             * @return esp_err_t    ESP_OK  : Setting was succesful
             *                      Other   : Something failed
             */
            esp_err_t SetConfigAdvData(esp_ble_adv_data_t *adv_data);

            /**
             * @brief Method to set scan response data
             *
             * @param[in] scan_rsp_data     : Pointer to response data structure
             *
             * @return esp_err_t    ESP_OK  : Setting was succesful
             *                      Other   : Something failed
             */
            esp_err_t SetScanResponseData(esp_ble_adv_data_t *scan_rsp_data);

            /**
             * @brief Method to start advertising
             *
             * @param[in] advParameter      : Pointer to advetising parameters
             *
             * @return esp_err_t    ESP_OK  : Start was successful
             *                      Othre   : Otherwise
             */
            esp_err_t StartAdvertising(esp_ble_adv_params_t *advParameter);

            /**
             * @brief Method to create bluetooth service
             *
             * @param[in] gatts_if – [in] GATT server access interface
             * @param[in] service_id - service ID
             * @param[in] num_handle - number of handle requested for this service
             *
             * @return esp_err_t    ESP_OK  : Service creation was successful
             *                      Other   : Otherwise
             */
            esp_err_t CreateService(esp_gatt_if_t gatts_if, esp_gatt_srvc_id_t *service_id, uint16_t num_handle);

            /**
             * @brief Method to start service
             *
             * @param[in] serviceHandle     : Service handle to start
             *
             * @return esp_err_t    ESP_OK  : Service handle started
             *                      Other   : Service handle failed to start
             */
            esp_err_t StartService(uint16_t serviceHandle);

            /**
             * @brief Method to update connection parameteres
             *
             * @param[in] gatts_if – [in] GATT server access interface
             *
             * @return esp_err_t    ESP_OK  : Connection parameteres update was successful
             *                      Other   : Otherwise
             */
            esp_err_t UpdateConParameteres(esp_ble_conn_update_params_t *conParameteres);

            /**
             * @brief Method to add characteristic to service
             *
             * @param[in] serviceHandle         : Service handle to which this included service is to be added
             * @param[in] characteristicUUID    : Characteristic UUID
             * @param[in] permission            : Characteristic value declaration attribute permission
             * @param[in] property              : Characteristic Properties
             * @param[in] characteristicValue   : Characteristic value
             * @param[in] control               : Attribute response control byte
             *
             * @return esp_err_t    ESP_OK  : Characteristic is added to service
             *                      Other   : Unable to add chaacteristic to service
             */
            esp_err_t AddCharacteristicToService(uint16_t serviceHandle, esp_bt_uuid_t *characteristicUUID,
                                                 esp_gatt_perm_t permission, esp_gatt_char_prop_t property,
                                                 esp_attr_value_t *characteristicValue, esp_attr_control_t *control = nullptr);

            /**
             * @brief Method to retreive atribute values
             *
             * @param[in] attr_handle           : Attribute handle.
             * @param[in] values                : Reference to vector of attribute values
             *
             * @return esp_err_t             : Status of getting attribute values
             */
            esp_err_t GetAttributeValues(uint16_t attr_handle, std::vector<uint8_t> &values);

            /**
             * @brief Method to add characteristic descriptor
             *
             * @param[in] serviceHandle         : Service handle to which this characteristic descriptor is to be added.
             * @param[in] descriptorUUID        : Descriptor UUID
             * @param[in] permission            : Descriptor access permission
             * @param[in] charDescriptorVAlue   : Characteristic descriptor value
             * @param[in] control               : Attribute response control byte
             *
             * @return esp_err_t    ESP_OK  : Characteristic descriptor is added
             *                      Other   : Unable to add chaacteristic descriptor
             */
            esp_err_t AddCharacteristicDescriptor(uint16_t serviceHandle, esp_bt_uuid_t *descriptorUUID,
                                                  esp_gatt_perm_t permission, esp_attr_value_t *charDescriptorVAlue = nullptr, esp_attr_control_t *control = nullptr);

            /**
             * @brief Method to send response to client
             *
             * @param[in] gatts_if      : GATT server access interface
             * @param[in] connectionID  : Connection ID
             * @param[in] transferID    : Transfer ID
             * @param[in] status        : Status
             * @param[in] rsp           : Response data
             *
             * @return esp_err_t    ESP_OK  : Characteristic descriptor is added
             *                      Other   : Unable to add chaacteristic descriptor
             */
            esp_err_t SendResponse(esp_gatt_if_t gatts_if, uint32_t connectionID, uint32_t transferID, esp_gatt_status_t status, esp_gatt_rsp_t *rsp = nullptr);

        private:
        };
    } // namespace Bluetooth
} // namespace Greenhouse

#endif // SERVER_BLUETOOTH_CONTROLLER