/* STD library includes */
#include <map>

namespace Sensor
{

// SHT4x Commands
#define MEASURE_T_RH_HIGH_PRECISION 0xFD   // Measure T & RH with highest precision (high repeatability)
#define MEASURE_T_RH_MEDIUM_PRECISION 0xF6 // Measure T & RH with medium precision (medium repeatability)
#define MEASURE_T_RH_LOW_PRECISION 0xE0    // Measure T & RH with lowest precision (low repeatability)

#define READ_SERIAL 0x89            // Read serial number
#define SOFT_RESET 0x94             // Soft Reset
#define ACTIVE_HEATER_POWER_1S 0x39 // Activate highest heater power for 1s

    // Map includes timeouts based on required measurement accurancy
    // See datasheet https://github.com/Dono95/Smart_Greenhouse/blob/Drivers_for_hardware_components/Datasheet/Sensors/Datasheet_SHT4x-1917879.pdf Page 7
    // Timeouts increased by 0.5 in (MEASURE_T_RH_LOW_PRECISION, MEASURE_T_RH_MEDIUM_PRECISION)
    // due to avoiding the use of "float" data type
    static std::map<uint8_t, uint8_t> precisionTimeouts = {
        {MEASURE_T_RH_LOW_PRECISION, 3},
        {MEASURE_T_RH_MEDIUM_PRECISION, 6},
        {MEASURE_T_RH_HIGH_PRECISION, 10},
    };

} // namespace Sensor
