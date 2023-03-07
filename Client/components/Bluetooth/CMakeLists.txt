message(STATUS "Building component: Client bluetooth")

# Set source file to variable SOURCES
set(SOURCES
./ClientBluetoothController.cpp
./ClientBluetoothHandler.cpp
./ConnectionHolder.cpp)

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS "." "../../main" "../"
                                REQUIRES Common_components)