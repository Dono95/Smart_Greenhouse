message(STATUS "Drivers")

# Set source file to variable SOURCES
set(SOURCES
./Sensors/SHT4x.cpp)

set(DIRECTORIES
"."
"../")  # Common coponents

# Register components with include header files
idf_component_register(SRCS ${SOURCES}
                                INCLUDE_DIRS ${DIRECTORIES}
                                REQUIRES bt)





