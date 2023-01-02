/* Project specific includes */
#include "SensorsData.hpp"

using namespace Greenhouse;

/**
 * @brief Class constructor
 */
SensorsData::SensorsData(float temperature, uint8_t humanity)
    : mTemperature(temperature), mHumanity(humanity)
{
}

/**
 * @brief Class destructor
 */
SensorsData::~SensorsData()
{
}

/**
 * @brief Get temperature
 *
 * @return float
 */
float SensorsData::GetTemperature() const
{
    return mTemperature.Get();
}

/**
 * @brief Get humanity
 *
 * @return uint8_t
 */
uint8_t SensorsData::GetHumanity() const
{
    return mHumanity.Get();
}