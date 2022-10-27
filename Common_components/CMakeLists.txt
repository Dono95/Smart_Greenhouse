message(STATUS "Common components")

# Set source file to variable SOURCES
set(SOURCES
./Bluetooth/BaseBluetoothController.cpp)

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS "."
                                REQUIRES bt)





