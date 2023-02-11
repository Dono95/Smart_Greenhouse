message(STATUS "Common components")

# Set source file to variable SOURCES
set(SOURCES
./Bluetooth/BaseBluetoothController.cpp
./Drivers/Network/WiFiDriver.cpp
./Drivers/Communication/I2C.cpp
./Convertors/Convertor_JSON.cpp
./Managers/TimeManager.cpp
./Drivers/Sensor/WaterLevelSensor.cpp
./Drivers/Motor/StepMotor.cpp
./Drivers/Active/WaterPump.cpp
./Utility/Indicator/RGB.cpp)

set(DIRECTORIES
"."
"./Drivers/Network"
"./Drivers/Communication"
"./Drivers/Sensor"
"./Drivers/Motor"
"./Drivers/Active"
"./Utility/Indicator")
# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS ${DIRECTORIES}
                                REQUIRES bt json)





