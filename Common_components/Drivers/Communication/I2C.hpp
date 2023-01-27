#ifndef I2C_H
#define I2C_H

/* ESP IDF driver libraries */
#include "driver/i2c.h"

/* STD library */
#include <stdint.h>
#include <vector>

#define I2C_TAG "I2C driver"

// I2C timeout
#define TIMEOUT_I2C_MS 1000
//
#define NUMBER_OF_READ_BYTES_DEFAULT 10

#define I2C_RESULT_TO_STRING(resultValue) Component::Driver::Communication::EnumToString(resultValue)

namespace Component
{
    namespace Driver
    {
        namespace Communication
        {
            enum class I2C_Result
            {
                I2C_ERROR,
                I2C_OK,
                WRITE_DATA_SUCCESSFUL,
                READ_DATA_SUCCESSFUL,
                INVALID_ARGUMENT,
            };

            static const char *EnumToString(I2C_Result value)
            {
                switch (value)
                {
                case (I2C_Result::I2C_ERROR):
                    return "I2C_Error";
                case (I2C_Result::WRITE_DATA_SUCCESSFUL):
                    return "I2C_Write_Data_Successful";
                case (I2C_Result::READ_DATA_SUCCESSFUL):
                    return "I2C_Read_Data_Successful";
                case (I2C_Result::INVALID_ARGUMENT):
                    return "I2C_Invalid_Argument";
                default:
                    return "Unimplemented value";
                }
            }

            class I2C
            {
            public:
                /**
                 * @brief Class constructor
                 *
                 * @param[in] sda   : SDA pin
                 * @param[in] scl   : SCL pin
                 */
                explicit I2C(const uint8_t sda, const uint8_t scl);

                /**
                 * @brief Class destructor
                 */
                ~I2C();

                /**
                 * Set master mode
                 *
                 * @param[in] mode  : Master mode
                 * @param[in] port  : I2C port
                 * @param[in] clock : Clock frequency
                 */
                void SetMode(i2c_mode_t mode, i2c_port_t port, uint32_t clock);

                /**
                 * Set slave mode
                 *
                 * @param[in] mode                      : Slave mode
                 * @param[in] port                      : I2C port
                 * @param[in] enable_10bit_slaveAddress : Enable 10 bit slave address
                 * @param[in] slaveAddress              : Slave address
                 * @param[in] maximumSpeed              : Expected clock speed from SCl
                 */
                void SetMode(i2c_mode_t mode, i2c_port_t port, uint8_t enable_10bit_slaveAddress,
                             uint16_t slaveAddress, uint32_t maximumSpeed);

                /**
                 * @brief Activate I2C driver
                 *
                 * @param[in] slave_rx_buffer_length    : Receiving buffer size. Ignored in master mode
                 * @param[in] slave_tx_buffer_length    : Sending buffer size. Ignored in master mode
                 * @param[in] interruptFlags            : Flag used to allocate the interrupt
                 */
                esp_err_t Activate(size_t slave_rx_buffer_length = 0, size_t slave_tx_buffer_length = 0,
                                   int interruptFlags = 0) const;

                /**
                 * @brief Deativate I2C driver
                 */
                esp_err_t Deactivate() const;

                /**
                 * @brief Write data
                 *
                 * @param[in] slaveAddress  : Slave address
                 * @param[in] byte          : Data to send
                 *
                 * @return I2C_Result       : WRITE_DATA_SUCCESSFUL - When all data was sent sussessfull
                 */
                I2C_Result Write(const uint8_t slaveAddress, const uint8_t byte) const;

                /**
                 * @brief Write data
                 *
                 * @param[in] slaveAddress  : Slave address
                 * @param[in] bytes         : Data to send
                 * @param[in] count         : Number of bytes to send. Range <1, 4> with default set to 1
                 *
                 * @return I2C_Result       : WRITE_DATA_SUCCESSFUL - When all data was sent sussessfull
                 */
                I2C_Result Write(const uint8_t slaveAddress, uint32_t bytes, const uint8_t count) const;

                /**
                 * @brief Write data
                 *
                 * @param[in] slaveAddress  : Slave address
                 * @param[in] data          : Data to send
                 *
                 * @return I2C_Result       : WRITE_DATA_SUCCESSFUL - When all data was sent sussessfull
                 */
                I2C_Result Write(const uint8_t slaveAddress, std::vector<uint8_t> &data) const;

                /**
                 * @brief Read data
                 *
                 * @param[in] slaveAddress  : Slave address
                 * @param[out] byte         : Received data
                 *
                 * @return I2C_Result       : READ_DATA_SUCCESSFUL - When all data was received sussessfull
                 */
                I2C_Result Read(const uint8_t slaveAddress, uint8_t *byte);

                /**
                 * @brief Read data
                 *
                 * @param[in] slaveAddress  : Slave address
                 * @param[out] data         : Received data
                 * @param[in] bytesToRead   : Bytes to read
                 *
                 * @return I2C_Result       : READ_DATA_SUCCESSFUL - When all data was received sussessfull
                 */
                I2C_Result Read(const uint8_t slaveAddress, std::vector<uint8_t> &data, uint8_t bytesToRead);

            private:
                // Config file of I2C communication interface
                i2c_config_t mConfig;

                // I2C port number
                i2c_port_t m_I2C_port;
            };
        } // namespace Communication

    } // namespace Driver
} // namespace Component

#endif /* I2C_H2*/