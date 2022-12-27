#include "I2C.hpp"

/* C libraries */
#include <cstring>

/* ESP log library */
#include <esp_log.h>

using namespace Component::Driver::Communication;

/**
 * @brief Class constructor
 *
 */
I2C::I2C(const uint8_t sda, const uint8_t scl)
{
    // Clear config structure
    memset(&mConfig, 0, sizeof(mConfig));

    mConfig = {
        .mode = I2C_MODE_MAX,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
    };
}

/**
 * @brief Class destructor
 */
I2C::~I2C()
{
}

/**
 * Set master mode
 */
void I2C::SetMode(i2c_mode_t mode, i2c_port_t port, uint32_t clock)
{
    // Check if mode is set to master
    if (mode != i2c_mode_t::I2C_MODE_MASTER)
    {
        ESP_LOGE(I2C_TAG, "Trying to set invalid mode to I2C");
        return;
    }

    m_I2C_port = port;

    // Set master configuration values
    mConfig.mode = mode;
    mConfig.master.clk_speed = clock;

    if (i2c_param_config(m_I2C_port, &mConfig) != ESP_OK)
        ESP_LOGE(I2C_TAG, "Configuration I2C bus with given configuration parameters failed");
}

/**
 * Set slave mode
 */
void I2C::SetMode(i2c_mode_t mode, i2c_port_t port, uint8_t enable_10bit_slaveAddress,
                  uint16_t slaveAddress, uint32_t maximumSpeed)
{
    // Check if mode is set to master
    if (mode != i2c_mode_t::I2C_MODE_SLAVE)
    {
        ESP_LOGE(I2C_TAG, "Trying to set invalid mode to I2C");
        return;
    }

    m_I2C_port = port;

    // Set master configuration values
    mConfig.mode = mode;
    mConfig.slave.addr_10bit_en = enable_10bit_slaveAddress;
    mConfig.slave.slave_addr = slaveAddress;
    mConfig.slave.maximum_speed = maximumSpeed;

    if (i2c_param_config(m_I2C_port, &mConfig) != ESP_OK)
        ESP_LOGE(I2C_TAG, "Configuration I2C bus with given configuration parameters failed");
}

/**
 * @brief Activate I2C driver
 */
esp_err_t I2C::Activate(size_t slave_rx_buffer_length, size_t slave_tx_buffer_length,
                        int interruptFlags) const
{
    if (mConfig.mode == i2c_mode_t::I2C_MODE_MASTER)
    {
        slave_rx_buffer_length = 0;
        slave_tx_buffer_length = 0;
    }

    return i2c_driver_install(m_I2C_port, mConfig.mode, slave_rx_buffer_length, slave_tx_buffer_length, interruptFlags);
}

/**
 * @brief Deativate I2C driver
 */
esp_err_t I2C::Deactivate() const
{
    return i2c_driver_delete(m_I2C_port);
}

/**
 * @brief Write data
 */
I2C_Result I2C::Write(const uint8_t slaveAddress, const uint8_t byte) const
{
    if (!i2c_master_write_to_device(m_I2C_port, slaveAddress, &byte, 1, TIMEOUT_I2C_MS / portTICK_RATE_MS))
        return I2C_Result::WRITE_DATA_SUCCESSFUL;

    ESP_LOGE(I2C_TAG, "Unable to send data to slave with address 0x%x", slaveAddress);
    return I2C_Result::I2C_ERROR;
}

/**
 * @brief Write data
 */
I2C_Result I2C::Write(const uint8_t slaveAddress, std::vector<uint8_t> &data) const
{
    uint8_t *array_bytes = new uint8_t[data.size()];

    std::copy(data.begin(), data.end(), array_bytes);

    uint8_t status = i2c_master_write_to_device(m_I2C_port, slaveAddress, array_bytes, data.size(), TIMEOUT_I2C_MS / portTICK_RATE_MS);

    delete[] array_bytes;

    if (!status)
        return I2C_Result::WRITE_DATA_SUCCESSFUL;

    return I2C_Result::I2C_ERROR;
}

/**
 * @brief Read data from specific slave
 */
I2C_Result I2C::Read(const uint8_t slaveAddress, uint8_t *byte)
{
    if (!i2c_master_read_from_device(m_I2C_port, slaveAddress, byte, 1, TIMEOUT_I2C_MS / portTICK_RATE_MS))
        return I2C_Result::READ_DATA_SUCCESSFUL;

    return I2C_Result::I2C_ERROR;
}

/**
 * @brief Read data
 */
I2C_Result I2C::Read(const uint8_t slaveAddress, std::vector<uint8_t> &data, uint8_t bytesToRead)
{
    if (!bytesToRead)
        return I2C_Result::INVALID_ARGUMENT;

    uint8_t *array_data = new uint8_t[bytesToRead];

    int status = i2c_master_read_from_device(m_I2C_port, slaveAddress, array_data, bytesToRead, TIMEOUT_I2C_MS / portTICK_RATE_MS);

    data.insert(data.begin(), array_data, array_data + bytesToRead);
    delete[] array_data;

    if (!status)
        return I2C_Result::READ_DATA_SUCCESSFUL;

    return I2C_Result::I2C_ERROR;
}

/*#include <cstring>

#define TIMEOUT_MS 1000

using namespace Interface;

I2C_I::I2C_I()
{
    memset(&mConfigI2C, 0, sizeof(mConfigI2C));
    mConfigI2C = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
    };
    mConfigI2C.master.clk_speed = (uint32_t)400000;

    i2c_param_config(m_i2c_master_port, &mConfigI2C);
    ESP_ERROR_CHECK(i2c_driver_install(m_i2c_master_port, mConfigI2C.mode, 0, 0, 0));
}

I2C_I::~I2C_I()
{
}

I2C_I *I2C_I::GetInstance()
{
    static I2C_I mInstance;
    return &mInstance;
}

I2C_Result I2C_I::WriteByte(const uint8_t slaveAddress, const uint8_t byte) const
{
    if (!i2c_master_write_to_device(m_i2c_master_port, slaveAddress, &byte, 1, TIMEOUT_MS / portTICK_RATE_MS))
        return I2C_Result::WRITE_DATA_SUCCESSFUL;

    return I2C_Result::I2C_ERROR;
}

I2C_Result I2C_I::WriteData(uint8_t slaveAddress, std::vector<uint8_t> &data)
{
    uint8_t *array_bytes = new uint8_t[data.size()];

    std::copy(data.begin(), data.end(), array_bytes);

    uint8_t status = i2c_master_write_to_device(m_i2c_master_port, slaveAddress, array_bytes, data.size(), TIMEOUT_MS / portTICK_RATE_MS);

    delete[] array_bytes;

    if (!status)
        return I2C_Result::WRITE_DATA_SUCCESSFUL;

    return I2C_Result::I2C_ERROR;
}

I2C_Result I2C_I::ReadByte(uint8_t slaveAddress, uint8_t *byte)
{
    if (!i2c_master_read_from_device(m_i2c_master_port, slaveAddress, byte, 1, TIMEOUT_MS / portTICK_RATE_MS))
        return I2C_Result::READ_DATA_SUCCESSFUL;

    return I2C_Result::I2C_ERROR;
}

I2C_Result I2C_I::ReadData(uint8_t slaveAddress, std::vector<uint8_t> &data, uint8_t numberOfReadBytes)
{
    if (!numberOfReadBytes)
        return I2C_Result::INVALID_ARGUMENT;

    uint8_t *array_data = new uint8_t[numberOfReadBytes];

    int status = i2c_master_read_from_device(m_i2c_master_port, slaveAddress, array_data, numberOfReadBytes, TIMEOUT_MS / portTICK_RATE_MS);

    data.insert(data.begin(), array_data, array_data + numberOfReadBytes);
    delete[] array_data;

    if (!status)
        return I2C_Result::READ_DATA_SUCCESSFUL;

    return I2C_Result::I2C_ERROR;
}*/

/*uint8_t I2C_I::Write()
{

    /// address must be 0x44
    uint8_t data[18];
    data[0] = 0x89;

    int status = i2c_master_write_to_device(m_i2c_master_port, 0x44, data, 1, TIMEOUT_MS / portTICK_RATE_MS);
    printf("Status %d\n", status);
    vTaskDelay(100);
    status = i2c_master_read_from_device(m_i2c_master_port, 0x44, data, 6, TIMEOUT_MS / portTICK_RATE_MS);
    printf("Status po read %d\n", status);

    for (uint8_t i = 0; i < 6; ++i)
        printf("Data[%d]: %d\n", i, data[i]);

    return 0;
}*/