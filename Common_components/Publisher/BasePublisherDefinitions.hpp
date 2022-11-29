#ifndef BASE_PUBLISHER_DEFINITIONS
#define BASE_PUBLISHER_DEFINITIONS

/* STL includes  */
#include <string>

namespace Component
{
    namespace Publisher
    {
        enum class Events
        {
            BLUETOOTH_DATA_RECEIVED // Received new bluetooth data
        };

        static std::string EnumToString(Events event)
        {
            switch (event)
            {
            case (Events::BLUETOOTH_DATA_RECEIVED):
                return "Bluetooth data received";

            default:
                return "";
            }
        }
    } // namespace Publisher
} // namespace Component

#endif // BASE_PUBLISHER_DEFINITIONS