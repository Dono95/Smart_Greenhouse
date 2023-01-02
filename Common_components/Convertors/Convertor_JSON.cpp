/* Project specific includes */
#include "Convertors/Convertor_JSON.hpp"

/* ESP log library*/
#include <esp_log.h>

#define JSON_SEPERATOR ":"

using namespace Component::Convertor;

Convertor_JSON *Convertor_JSON::mConvertor{nullptr};
std::mutex Convertor_JSON::mConvertorMutex;

/*********************************************
 *              PRIVATE API                  *
 ********************************************/

/**
 * @brief Class constructor
 */
Convertor_JSON::Convertor_JSON()
{
}

/**
 * @brief Class destructor
 */
Convertor_JSON::~Convertor_JSON()
{
}

/**
 * @brief Add double quotes to string literal
 *
 * @param[in] raw   : Raw string
 *
 * @return std::string  : Raw string with added double quotes
 */
std::string Convertor_JSON::StringWithDoubleQuotes(const char *raw) const
{
    if (!raw)
        return {};

    return std::string("\"\"").insert(1, raw);
}

/*********************************************
 *              PUBLIC API                   *
 ********************************************/

/**
 * @brief Static method to get instance of GreenhouseManager
 */
Convertor_JSON *Convertor_JSON::GetInstance()
{
    std::lock_guard<std::mutex> lock(mConvertorMutex);
    if (!mConvertor)
        mConvertor = new Convertor_JSON();

    return mConvertor;
}

/**
 * @brief Convert JSON structure to String
 *
 * @param[in] root  : Root of cJSON strcutre
 *
 * @return std::string
 */
std::string Convertor_JSON::ToString(const cJSON *root)
{
    //////////////// !!!!!! CHECK FOR NULLPTR IMPORT TO DO !!!!! 02/01/2023

    if (!root)
        return {};

    std::string jsonString{"{}"};
    auto child = root->child;

    for (uint16_t i = 0; i < cJSON_GetArraySize(root); ++i)
    {
        if (!child)
            continue;

        switch (child->type)
        {
        case (cJSON_Number):
        {
            jsonString.insert(jsonString.size() - 1, StringWithDoubleQuotes(child->string) +
                                                         JSON_SEPERATOR +
                                                         ToString(child->valuedouble) + ",");
            break;
        }
        case (cJSON_String):
        {
            jsonString.insert(jsonString.size() - 1, StringWithDoubleQuotes(child->string) +
                                                         JSON_SEPERATOR +
                                                         StringWithDoubleQuotes(child->valuestring) + ",");
            break;
        }
        default:
            ESP_LOGW(CONVERTOR_JSON_TAG, "Unhandled JSON tag %d", child->type);
            break;
        }

        child = child->next;
    }

    // Remove comma after last JSON value
    jsonString.erase(jsonString.size() - 2, 1);

    return jsonString;
}

/**
 * @brief Convert double to String with defined precision
 */
std::string Convertor_JSON::ToString(const double number, uint8_t precision)
{
    if (precision > MAX_NUMBER_PRECISION)
        precision = MAX_NUMBER_PRECISION;

    // Converstion to string
    std::string numberString(std::to_string(number));

    // Try to find floating point in string number
    auto floatingPoint = numberString.find('.');
    if (floatingPoint == std::string::npos)
        return {};

    // Erase string with define precision
    numberString.erase(floatingPoint + precision + 1);

    // Check if last character is floating point (Precision set to 0) then remove last character
    if (numberString.back() == '.')
        numberString.pop_back();

    return numberString;
}