namespace Sensor
{

// SHT4x Commands
#define MEASURE_T_RH_HIGH_PRECISION 0xFD   // Measure T & RH with highest precision (high repeatability)
#define MEASURE_T_RH_MEDIUM_PRECISION 0xF6 // Measure T & RH with medium precision (medium repeatability)
#define MEASURE_T_RH_LOW_PRECISION 0xE0    // Measure T & RH with lowest precision (low repeatability)

#define READ_SERIAL 0x89            // Read serial number
#define SOFT_RESET 0x94             // Soft Reset
#define ACTIVE_HEATER_POWER_1S 0x39 // Activate highest heater power for 1s
} // namespace Sensor
