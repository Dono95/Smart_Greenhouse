#include "I2C_I.hpp"

#include <cstring>

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
}

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