message(STATUS "Building component: Bluetooth")

# Set source file to variable SOURCES
set(SOURCES
./ClientBluetoothController.cpp
./ClientBluetoothHandler.cpp)

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS "." "../../main"
                                REQUIRES Common_components)