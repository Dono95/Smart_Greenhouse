message(STATUS "Common components")

# Set source file to variable SOURCES
set(SOURCES
./Bluetooth/BaseBluetoothController.cpp
./Drivers/Network/WiFiDriver.cpp)

set(DIRECTORIES
"."
"./Drivers/Network")

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS ${DIRECTORIES}
                                REQUIRES bt)





