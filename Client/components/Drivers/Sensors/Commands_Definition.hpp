/* STD library includes */
#include <map>

namespace Sensor
{

/*************************************/
/**              SHT4x               */
/*************************************/
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

/*************************************/
/**              SCD4x               */
/*************************************/
// Basic commands
#define START_PERIODIC_MEASUREMENT 0x21B1
#define READ_MEASUREMENT 0xEC05
#define STOP_PERIODIC_MEASUREMENT 0x3F86

// On-chip output signal compensation
#define SET_TEMPERATURE_OFFSET 0x241d
#define GET_TEMPERATURE_OFFSET 0x2318
#define SET_SENSOR_ALTITUDE 0x2427
#define GET_SENSOR_ALTITUDE 0x2322
#define SET_AMBIENT_PRESSURE 0xE000

// Field calibration
#define PERFORM_FORCED_RECALIBRATION 0x362F
#define SET_AUTOMATIC_SELF_CALIBRATION_ENABLED 0x2416
#define GET_AUTOMATIC_SELF_CALIBRATION_ENABLED 0x2313

// Low power
#define START_LOW_POWER_PERIODIC_MEASUREMENT 0x21AC
#define GET_DATA_READY_STATUS 0xE4B8

// Advanced features
#define PERSIST_SETTINGS 0x3615
#define GET_SERIAL_NUMBER 0x3682
#define PERFORM_SELF_TEST 0x3639
#define PERFORM_FACTORY_RESET 0x3632
#define REINIT 0x3646

// Low power single shot (SCD41 only)
#define MEASURE_SINGLE_SHOT 0x219D
#define MEASURE_SINGLE_SHOT_RHT_ONLY 0z2196

} // namespace Sensor
