#ifndef CONVERTOR_JSON_H
#define CONVERTOR_JSON_H

/* STD library includes */
#include <mutex>
#include <cstdint>

/* ESP JSON library */
#include <cJSON.h>

/* SDK */
#include "sdkconfig.h"

#define CONVERTOR_JSON_TAG "Convertor JSON"

#define MAX_NUMBER_PRECISION 5

namespace Component
{
    namespace Convertor
    {
        class Convertor_JSON
        {
        public:
            /**
             * @brief Static method to get instance of Convertor_JSON
             *
             * @return Convertor_JSON    : Pointer to Convertor_JSON manager
             */
            static Convertor_JSON *GetInstance();

            /**
             * @brief Convert JSON structure to String
             *
             * @param[in] root  : Root of cJSON strcutre
             *
             * @return std::string
             */
            std::string ToString(const cJSON *root);

            /**
             * @brief Convert double to String with defined precision
             *
             * @param[in] number  : Number
             *
             * @return std::string
             */
            std::string ToString(const double number, uint8_t precision = CONFIG_JSON_Number_Precision);

        private:
            /**
             * @brief Class constructor
             */
            explicit Convertor_JSON();

            /**
             * @brief Class destructor
             */
            ~Convertor_JSON();

            /**
             * @brief Add double quotes to string literal
             *
             * @param[in] raw   : Raw string
             *
             * @return std::string  : Raw string with added double quotes
             */
            std::string StringWithDoubleQuotes(const char *raw) const;

            // Singleton instance of JSON converter
            static Convertor_JSON *mConvertor;

            // Singleton mutex to protect instance from multithread
            static std::mutex mConvertorMutex;
        };
    } // namespace Convertor
} // namespace Component

#endif // CONVERTOR_JSON_H