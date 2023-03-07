/**
 * Definition of Client bluetooth controller
 *
 * @author Dominik Regec
 */
#ifndef CLIENT_BLUETOOTH_CONTROLLER
#define CLIENT_BLUETOOTH_CONTROLLER

/* Common components */
#include "Bluetooth/BaseBluetoothController.hpp"

/* STD library includes */
#include <vector>

/* Define class log tag */
#define CLIENT_BLUETOOTH_CONTROLLER_TAG "ClientBluetoothController"

namespace Greenhouse
{
	namespace Bluetooth
	{
		class ClientBluetoothControlller : public Component::Bluetooth::BaseBluetoothController
		{
		public:
			/* Typedef for incialization bluetooth return code*/
			using INIT_BLE_STATUS = Component::Bluetooth::INIT_BLUETOOTH_RV;

			/**
			 * @brief Class contrustor
			 */
			explicit ClientBluetoothControlller();

			/**
			 * @brief Class destructor
			 */
			~ClientBluetoothControlller();

			/**
			 * @brief Overridden method to register gap and gatts callback for client controller
			 */
			virtual INIT_BLE_STATUS RegisterCallbacks(void) override;

			/**
			 * @brief Start device scanning in periodic sequences
			 *
			 * @param[in] duration : It stores the time at which the scan is performed
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t StartScanning(uint32_t duration);

			/**
			 * @brief Stop device scanning
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t StopScanning();

			/**
			 * @brief Set scan parameters
			 *
			 * @param[in] params : Optional scanning parameters [nullptr]
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t SetScanParameters(esp_ble_scan_params_t *params = nullptr);

			/**
			 * @brief Open a virtual connection to remote device
			 *
			 * @param[in] gattc_if          : Gatt client access interface.
			 * @param[in] removeAddress     : Remote device bluetooth device address.
			 * @param[in] removeAddressType : Remote device bluetooth device the address type.
			 * @param[in] isDirect          : Direct connection
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t OpenConnection(esp_gatt_if_t gattc_if, esp_bd_addr_t remoteAddress, esp_ble_addr_type_t removeAddressType, bool isDirect);

			/**
			 * @brief Close a virtual connection to remote device
			 *
			 * @param[in] gattc_if          : Gatt client access interface.
			 * @param[in] connectionID      : Connection ID
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t CloseConnection(esp_gatt_if_t gattc_if, uint16_t connectionID);

			/**
			 * @brief Configure the MTU size in the GATT channel.
			 *
			 * @param[in] gattc_if          : Gatt client access interface
			 * @param[in] connectionID      : Connection ID
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t Send_MTU_Request(esp_gatt_if_t gattc_if, uint16_t connectionID);

			/**
			 * @brief Serach service from local cache
			 *
			 * @param[in] gattc_if          : Gatt client access interface
			 * @param[in] connectionID      : Connection ID
			 * @param[in] serviceFiler      : a UUID of interested service
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t SearchService(esp_gatt_if_t gattc_if, uint16_t connectionID, esp_bt_uuid_t *serviceFilter);

			/**
			 * @brief Get attribute count
			 *
			 * @param[in]  gattc_if          : Gatt client access interface
			 * @param[in]  connectionID      : Connection ID
			 * @param[in]  type              : The attribute type.
			 * @param[in]  startHandle       : The attribute start handle
			 * @param[in]  endHandle         : The attribute end handle
			 * @param[in]  charHandle        : The characteristic handle
			 * @param[out] count             : The number of attribute has been found
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t GetAttributeCount(esp_gatt_if_t gattc_if, uint16_t connectionID, esp_gatt_db_attr_type_t type,
																	uint16_t startHandle, uint16_t endHandle, uint16_t charHandle, uint16_t *count);

			/**
			 * @brief Get characteristic by UUID
			 *
			 * @param[in]  gattc_if               : Gatt client access interface
			 * @param[in]  connectionID           : Connection ID
			 * @param[in]  startHandle            : The attribute start handle
			 * @param[in]  endHandle              : The attribute end handle
			 * @param[in]  characteristicUUID     : The characteristic UUID
			 * @param[in]  characteristicElement  : The pointer to the characteristic in the service
			 * @param[inout] count                : Input the number of descriptor want to find, it will output the number of descriptor
			 *                                      has been found in the gattc cache with the given characteristic.
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t GetCharacteristicByUUID(esp_gatt_if_t gattc_if, uint16_t connectionID, uint16_t startHandle, uint16_t endHandle,
																				esp_bt_uuid_t characteristicUUID, esp_gattc_char_elem_t *characteristicElement, uint16_t *count);

			/**
			 * @brief Write characteristic value
			 *
			 * @param[in]  gattc_if               : Gatt client access interface
			 * @param[in]  connectionID           : Connection ID
			 * @param[in]  characterissticHandle  : The characteristic value to write
			 * @param[in]  value               		: The data to be written
			 * @param[in]  writeType     					: The type of attribute write operation
			 * @param[in]  authenticationRequest  : Authentication request
			 *
			 * @return esp_err_t    ESP_OK  : success
			 *                      Other   : failed
			 */
			esp_err_t WriteCharacteristic(esp_gatt_if_t gattc_if, uint16_t connectionID, uint16_t characterissticHandle, std::vector<uint8_t> &value,
																		esp_gatt_write_type_t writeType, esp_gatt_auth_req_t authenticationRequest);

		private:
			/* Default scan parameters */
			esp_ble_scan_params_t ble_scan_params = {
					.scan_type = BLE_SCAN_TYPE_ACTIVE,
					.own_addr_type = BLE_ADDR_TYPE_PUBLIC,
					.scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
					.scan_interval = 0x50,
					.scan_window = 0x30,
					.scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE};
		};
	} // namespace Bluetooth
} // namespace Greenhouse

#endif // CLIENT_BLUETOOTH_CONTROLLER