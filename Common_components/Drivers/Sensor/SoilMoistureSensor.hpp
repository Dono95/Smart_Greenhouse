#ifndef SOIL_MOISTURE_SENSOR_H
#define SOIL_MOISTURE_SENSOR_H

/* ESP library */
#include <driver/dac_common.h>
#include <driver/adc.h>

#define SOIL_MOISTURE_SENSOR_TAG "SoilMoistureSensor"

#define IN_MIN 2902
#define IN_MAX 1260
#define OUT_MIN 0
#define OUT_MAX 100

namespace Component
{
  namespace Driver
  {
    namespace Sensor
    {
      class SoilMoistureSensor
      {
      public:
        /**
         * @brief Class constructor
         *
         * @param[in] adc_channel
         * @param[in] attenuation
         */
        explicit SoilMoistureSensor(adc1_channel_t adc_channel, adc_atten_t attenuation);

        /**
         * @brief Class constructor
         */
        explicit SoilMoistureSensor(adc2_channel_t adc_channel, adc_bits_width_t width, adc_atten_t attenuation);

        /**
         * @brief Class destructor
         */
        ~SoilMoistureSensor();

        /**
         * @brief Read data from sensor
         *
         * @return float      : Measured data in percentage
         */
        float Measure() const;

        /**
         * @brief Read raw data from sensor
         *
         * @return uint16_t   : Raw data from sensor
         */
        uint16_t MeasureRawData() const;

      private:
        enum class ADC_TYPE
        {
          ADC1,
          ADC2
        };

        /* GPIO of moisure sensor with ADC*/
        gpio_num_t mSensorPin;

        /* ADC type used */
        ADC_TYPE mADC_type;

        /* Only one of channel type is defined */

        /* ADC1 channel */
        adc1_channel_t *mADC1_channel;

        /* ADC2 channel */
        adc2_channel_t *mADC2_channel;

        /* ADC bits width */
        adc_bits_width_t mWidth;
      };
    } // namespace Sensor

  } // namespace Driver
} // namespace Component

#endif // SOIL_MOISTURE_SENSOR_H