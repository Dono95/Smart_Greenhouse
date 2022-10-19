#ifndef I2C_I_H
#define I2C_I_H

//#include "driver/i2c.h"
#include "driver/i2c.h"
#include <vector>

#define NUMBER_OF_READ_BYTES_DEFAULT 10

namespace Interface
{
    enum class I2C_Result
    {
        I2C_ERROR,
        WRITE_DATA_SUCCESSFUL,
        READ_DATA_SUCCESSFUL,
        INVALID_ARGUMENT,
    };

    inline const char *EnumToString(I2C_Result value)
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
            return "Unknown enum value";
        }
    }

    class I2C_I
    {
    public:
        I2C_I(I2C_I &other) = delete;
        void operator=(const I2C_I &) = delete;

        ~I2C_I();

        static I2C_I *GetInstance();

        I2C_Result WriteByte(const uint8_t slaveAddress, const uint8_t byte) const;
        I2C_Result WriteData(uint8_t slaveAddress, std::vector<uint8_t> &data);

        I2C_Result ReadByte(uint8_t slaveAddress, uint8_t *byte);
        I2C_Result ReadData(uint8_t slaveAddress, std::vector<uint8_t> &data, uint8_t numberOfReadBytes = NUMBER_OF_READ_BYTES_DEFAULT);

    protected:
        explicit I2C_I();

    private:
        int m_i2c_master_port = I2C_NUM_0;
        i2c_config_t mConfigI2C;
    };
} // namespace Interface

#endif /* I2C_I_H */