#ifndef GREENHOUSE_DEFINITIONS
#define GREENHOUSE_DEFINITIONS

/* SDK config file */
#include "sdkconfig.h"

/* STD library */
#include <string>
#include <vector>

/******************************************************/
/******                 NETWORK                   *****/
/******************************************************/

/**********           MQTT TOPIS            ***********/
// PUBLISH
#define INFO "Greenhouse/info"
#define SENSOR_DATA "Greenhouse/SensorData"

// SUBSCRIBE
// Defines must be stored in greenhouse_topics data structure to be applied
#define WINDOW "Greenhouse/window"
#define WINDOW_ID "Greenhouse/window/" + std::to_string(CONFIG_Greenhouse_ID)

//---------------------------------------------------------------------------------//

// The data structure should only contain definitions from the subscribe block
static std::vector<std::string> greenhouse_topics = {
    WINDOW,
    WINDOW_ID};

#endif