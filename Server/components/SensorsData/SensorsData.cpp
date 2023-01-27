/* Project specific includes */
#include "SensorsData.hpp"

using namespace Greenhouse;

/**
 * @brief Class constructor
 */
SensorsData::SensorsData(Position position, float temperature, int8_t humanity, int8_t co2)
    : mPosition(position), mTemperature(temperature), mHumanity(humanity), mCO2(co2)
{
    mMeasureTime = Component::Manager::TimeManager::GetInstance()->GetRawTime();
}

/**
 * @brief Class destructor
 */
SensorsData::~SensorsData()
{
}

/**
 * @brief Get Position
 */
SensorsData::Position SensorsData::GetPosition() const
{
    return mPosition;
}

/**
 * @brief Get temperature
 */
float SensorsData::GetTemperature() const
{
    return mTemperature.Get();
}

/**
 * @brief Get humanity
 */
int8_t SensorsData::GetHumanity() const
{
    return mHumanity.Get();
}

/**
 * @brief Get CO2
 */
int8_t SensorsData::GetCO2() const
{
    return mCO2.Get();
}

/**
 * @brief Get measure time
 *
 * @return time_t
 */
time_t SensorsData::GetMeasureTime() const
{
    return mMeasureTime;
}