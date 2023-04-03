/* Project specific includes */
#include "SoilMoistureSensor.hpp"

/* ESP library */
#include <driver/gpio.h>

/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* ESP log library */
#include <esp_log.h>

using namespace Component::Driver::Sensor;

/**
 * @brief Class constructor
 *
 * @note No tested for now DR: 13/02/2023 08:31
 */
SoilMoistureSensor::SoilMoistureSensor(adc1_channel_t adc_channel, adc_atten_t attenuation)
    : mADC_type(ADC_TYPE::ADC1), mADC1_channel(new adc1_channel_t(adc_channel)), mADC2_channel(nullptr)
{
  esp_err_t r = adc1_pad_get_io_num(adc_channel, &mSensorPin);

  ESP_LOGI(SOIL_MOISTURE_SENSOR_TAG, "Inicialization ADC1 on GPIO: %d", static_cast<int>(mSensorPin));
  adc1_config_channel_atten(adc_channel, attenuation);

  // Timeout for configuration
  vTaskDelay(1000);
}

/**
 * @brief Class constructor
 */
SoilMoistureSensor::SoilMoistureSensor(adc2_channel_t adc_channel, adc_bits_width_t width, adc_atten_t attenuation)
    : mADC_type(ADC_TYPE::ADC2), mADC1_channel(nullptr), mADC2_channel(new adc2_channel_t(adc_channel)), mWidth(width)
{

  esp_err_t r = adc2_pad_get_io_num(adc_channel, &mSensorPin);
  assert(r == ESP_OK);

  ESP_LOGI(SOIL_MOISTURE_SENSOR_TAG, "Inicialization ADC2 on GPIO: %d", static_cast<int>(mSensorPin));
  adc2_config_channel_atten(adc_channel, attenuation);

  // Timeout for configuration
  vTaskDelay(1000);
}

/**
 * @brief Class destructor
 */
SoilMoistureSensor::~SoilMoistureSensor()
{
  if (mADC1_channel)
  {
    delete mADC1_channel;
    mADC1_channel = nullptr;
  }

  if (mADC2_channel)
  {
    delete mADC2_channel;
    mADC2_channel = nullptr;
  }
}

/**
 * @brief Read data from sensor
 */
float SoilMoistureSensor::Measure() const
{
  float raw_data = MeasureRawData();
  auto sensor_output = (((raw_data - SENSOR_MIN) / (SENSOR_MAX - SENSOR_MIN)) * 100);

  if (sensor_output < OUTPUT_MIN)
    return OUTPUT_MAX;
  else if (sensor_output > OUTPUT_MAX)
    return OUTPUT_MIN;
  else
    return 100 - sensor_output;
}

/**
 * @brief Read raw data from sensor
 */
uint16_t SoilMoistureSensor::MeasureRawData() const
{
  switch (mADC_type)
  {
  case ADC_TYPE::ADC1:
    return adc1_get_raw(*mADC1_channel);
  case ADC_TYPE::ADC2:
  {
    int raw_data{0};
    esp_err_t result = adc2_get_raw(*mADC2_channel, mWidth, &raw_data);

    if (result != ESP_OK)
    {
      ESP_LOGE(SOIL_MOISTURE_SENSOR_TAG, "Reading error from ADC2 on GPIO %d", static_cast<int>(mSensorPin));
      return 0;
    }

    return raw_data;
  }
  default:
    break;
  }

  return 0;
}