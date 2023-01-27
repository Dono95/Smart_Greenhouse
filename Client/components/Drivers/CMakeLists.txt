message(STATUS "Drivers")

# Set source file to variable SOURCES
set(SOURCES
./Sensors/Sensor.cpp
./Sensors/SHT4x.cpp
./Sensors/SCD4x.cpp)

set(DIRECTORIES
"."
"../"
"../../")  # Common coponents

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS ${DIRECTORIES}
                                REQUIRES bt Common_components)





